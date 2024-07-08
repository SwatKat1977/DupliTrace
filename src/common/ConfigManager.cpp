/*
This source file is part of DupliTrace
For the latest info, see https://github.com/SwatKat1977/DupliTrace

Copyright 2024 DupliTrace Development Team

    This program is free software : you can redistribute it and /or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see < https://www.gnu.org/licenses/>.
*/
#include <algorithm>
#include <iostream>
#include <string>
#include "ConfigManager.h"
#include "Platform.h"

#define UNDEFINED_INIT_VALUE "__UNSET__"

namespace duplitrace { namespace common {

// Constructor for the configuration manager class.
ConfigManager::ConfigManager() :
    config_file_(""),
    has_config_file_(false),
    config_file_required_(false),
    layout_(nullptr),
    config_reader_(nullptr) {
}

void ConfigManager::Configure(ConfigSetup* layout, std::string configFile,
                              bool fileRequired) {
    config_file_ = configFile;
    config_file_required_ = fileRequired;
    layout_ = layout;
}

bool ConfigManager::processConfig() {
    if (!config_file_.empty()) {
        config_reader_ = new INIReader(config_file_);

        if (config_reader_->ParseError() != 0) {
            std::cerr << "Unable to load config file '"
                << config_file_ << "'" << std::endl;
            return false;
        }

        has_config_file_ = true;
    }

    return ReadConfiguration();
}

// Get a configuration entry item value from a section.
int ConfigManager::GetIntEntry(std::string sectionName, std::string itemName) {
    auto section = config_items_.find(sectionName);
    if (section == config_items_.end()) {
        throw std::invalid_argument("Invalid section");
    }

    auto item = config_items_[section->first].find(itemName);
    if (item == config_items_[section->first].end()) {
        std::string exception = "Invalid config item "
            + sectionName
            + "::" + itemName;
        throw std::invalid_argument(exception);
    }

    return item->second.GetIntValue();
}

// Get a configuration entry item value from a section.
std::string ConfigManager::GetStringEntry(std::string sectionName,
                                          std::string itemName) {
    auto section = config_items_.find(sectionName);
    if (section == config_items_.end()) {
        throw std::invalid_argument("Invalid section");
    }

    auto item = config_items_[section->first].find(itemName);
    if (item == config_items_[section->first].end()) {
        std::string exception = "Invalid config item "
            + sectionName
            + "::" + itemName;
        throw std::invalid_argument(exception);
    }

    return item->second.GetStringValue();
}

bool ConfigManager::ReadConfiguration() {
    auto sections = layout_->Sections();

    for (auto sectionName = sections.begin(); sectionName != sections.end();
         sectionName++) {
        SectionList sectionItems = layout_->Section(*sectionName);

        for (auto sectionItem = sectionItems.begin();
             sectionItem != sectionItems.end();
             sectionItem++) {
            ConfigItemValue itemDataValue;

            switch (sectionItem->second.Type()) {
            case CONFIG_ITEM_TYPE_INTEGER: {
                itemDataValue.SetItemType(CONFIG_ITEM_TYPE_INTEGER);

                try {
                    int* itemValue = ReadInt(*sectionName,
                                             sectionItem->second);
                    if (itemValue) {
                        itemDataValue.SetIntValue(*itemValue);
                    }
                }
                catch (const std::exception& ex) {
                    std::cerr << "Read config item failed: "
                        << ex.what() << std::endl;
                    return false;
                }

                if (config_items_.find(*sectionName) == config_items_.end()) {
                    config_items_.insert({ *sectionName, {} });
                }

                config_items_[*sectionName].insert(
                     { sectionItem->second.ItemName(),
                       itemDataValue });
                break;
            }

            case CONFIG_ITEM_TYPE_STRING: {
                itemDataValue.SetItemType(CONFIG_ITEM_TYPE_STRING);

                try {
                    std::string itemValue = ReadStr(*sectionName,
                        sectionItem->second);
                    itemDataValue.SetStringValue(itemValue);
                }
                catch(const std::exception& ex) {
                    std::cerr << "Read config item failed: "
                        << ex.what() << std::endl;
                    return false;
                }

                if (config_items_.find(*sectionName) == config_items_.end()) {
                    config_items_.insert({ *sectionName, {} });
                }

                config_items_[*sectionName].insert(
                    { sectionItem->second.ItemName(),
                    itemDataValue });
                break;
            }
            }
        }
    }

    return true;
}

/*
Read a configuration option of type int, firstly it will check for
an enviroment variable (format is section_option), otherise try the
configuration file (if it exists). An ValueError exception is thrown
it's missing and marked as is_required or is not an int.
*/
int* ConfigManager::ReadInt(std::string section, ConfigSetupItem fmt) {
    int* intValue = nullptr;

    std::string envVariable = section + "_" + fmt.ItemName();
    std::transform(envVariable.begin(), envVariable.end(), envVariable.begin(),
                   ::toupper);

    std::string envValue = GetEnv(envVariable.c_str());

    if (!envValue.empty()) {
        try {
            intValue = new int (std::stoi(envVariable));
        }
        catch (std::invalid_argument const&) {
            std::string except = "Invalid int value (cannot parse int) for " +
                section + "::" + fmt.ItemName();
            throw std::out_of_range(except);
        }
        catch (std::out_of_range const&) {
            std::string except = "Invalid int value (out of range) for " +
                section + "::" + fmt.ItemName();
            throw std::out_of_range(except);
        }
    }

    // If no environment variable is found, check config file (if exits)
    if (!intValue && has_config_file_) {
        auto iniIntValue = config_reader_->Get(section, fmt.ItemName(),
                                               UNDEFINED_INIT_VALUE);

        if (iniIntValue != UNDEFINED_INIT_VALUE) {
            try {
                intValue = new int(std::stoi(iniIntValue));
            }
            catch (std::invalid_argument const&) {
                std::string except = "Cannot convert int value '" +
                    iniIntValue + "' for " +
                    section + "::" + fmt.ItemName();
                throw std::out_of_range(except);
            }
            catch (std::out_of_range const&) {
                std::string except = "Invalid int value (out of range) for " +
                    section + "::" + fmt.ItemName();
                throw std::out_of_range(except);
            }
        }
    }

    if (!intValue) {
        if (fmt.IsUnset() && fmt.IsRequired()) {
            std::string except = "Missing required config option " +
                section + "::" + fmt.ItemName();
            throw std::invalid_argument(except);
        } else if (!fmt.IsUnset()) {
            intValue = new int(fmt.DefaultIntValue ());
        }
    }

    if (intValue && fmt.ValidIntValues().size()) {
        auto validElements = fmt.ValidIntValues();

        if (std::find(validElements.begin(),
                      validElements.end(),
                      *intValue) == validElements.end()) {
            std::string exceptStr = "Config item " + section +
                "::" + fmt.ItemName() +
                " value '" +
                std::to_string(*intValue) +
                "' is not valid!";
            throw std::out_of_range(exceptStr);
        }
    }

    return intValue;
}

/*
Read a configuration option of type string, firstly it will check for
an enviroment variable (format is section_option), otherise try the
configuration file (if it exists). An ValueError exception is thrown
it's missing and marked as is_required.
*/
std::string ConfigManager::ReadStr(std::string section, ConfigSetupItem fmt) {
    bool isSet = false;
    std::string strValue;

    std::string envVariable = section + "_" + fmt.ItemName();
    std::transform(envVariable.begin(), envVariable.end(),
                   envVariable.begin(), ::toupper);

    std::string envValue = GetEnv(envVariable.c_str());

    if (!envValue.empty()) {
        strValue = std::string(envValue);
        isSet = true;
    }

    // If no environment variable is found, check config file (if exits)
    if (!isSet && has_config_file_) {
        auto iniStrValue = config_reader_->Get(section,
                                               fmt.ItemName(),
                                               UNDEFINED_INIT_VALUE);

        if (iniStrValue != UNDEFINED_INIT_VALUE) {
            strValue = iniStrValue;
            isSet = true;
        }
    }

    if (!isSet) {
        if (fmt.IsUnset() && fmt.IsRequired()) {
            std::string except = "Missing required config option " +
                section + "::" + fmt.ItemName();
            throw std::invalid_argument(except);
        }

        strValue = fmt.DefaultStringValue();
    }

    auto validElements = fmt.ValidStringValues();

    if (isSet && validElements.size()) {
        if (std::find (validElements.begin(),
            validElements.end(),
            strValue) == validElements.end()) {
            std::string exceptStr = "Config item " +
                section +
                "::" +
                fmt.ItemName() +
                " value '" +
                strValue +
                "' is not valid!";
            throw std::out_of_range(exceptStr);
        }
    }

    return strValue;
}

}   // namespace common
}   // namespace duplitrace
