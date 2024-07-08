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
#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_
#include <map>
#include <string>
#include "iniReader.h"
#include "ConfigSetup.h"

namespace duplitrace { namespace common {

class ConfigItemValue {
 public:
    ConfigItemValue() : int_value_(0), str_value_("") {
    }

    ConfigItemDataType GetItemType() { return item_type_; }
    void SetItemType(ConfigItemDataType dataType) { item_type_ = dataType; }

    int GetIntValue() { return int_value_; }
    void SetIntValue(int value) { int_value_ = value; }

    std::string GetStringValue() { return str_value_; }
    void SetStringValue(std::string value) { str_value_ = value; }

 private:
    ConfigItemDataType item_type_;
    int int_value_;
    std::string str_value_;
};

using ConfigItemValueItem = std::map<std::string, ConfigItemValue>;
using ConfigItemMap = std::map<std::string, ConfigItemValueItem>;

class ConfigManager {
 public:
    ConfigManager();

    void Configure(ConfigSetup* layout, std::string configFile = "",
                   bool fileRequired = false);

    bool processConfig();

    int GetIntEntry(std::string sectionName, std::string itemName);

    std::string GetStringEntry(std::string sectionName, std::string itemName);

 private:
    std::string config_file_;
    bool has_config_file_;
    bool config_file_required_;
    ConfigSetup* layout_;
    ConfigItemMap config_items_;
    INIReader* config_reader_;

    bool ReadConfiguration();

    int* ReadInt(std::string section, ConfigSetupItem fmt);

    std::string ReadStr(std::string section, ConfigSetupItem fmt);
};

}   // namespace common
}   // namespace duplitrace

#endif  // CONFIGMANAGER_H_
