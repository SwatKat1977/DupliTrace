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
#ifndef CONFIGSETUPITEM_H_
#define CONFIGSETUPITEM_H_
#include <list>
#include <string>

namespace duplitrace { namespace common {

// Enumeration for configuration item data type
enum ConfigItemDataType {
    CONFIG_ITEM_TYPE_INTEGER = 0,
    CONFIG_ITEM_TYPE_STRING = 1
};

using StringList = std::list<std::string>;
using IntList = std::list<int>;

// Configuration layout class
class ConfigSetupItem {
 public:
    ConfigSetupItem(std::string itemName, ConfigItemDataType dataType);

    std::string ItemName();

    ConfigItemDataType Type();

    bool IsUnset();

    bool IsRequired();

    int DefaultIntValue();

    std::string DefaultStringValue();

    IntList ValidIntValues();

    StringList ValidStringValues();

    ConfigSetupItem &IsRequired(bool state);

    ConfigSetupItem &DefaultValue(int defaultValue);
    ConfigSetupItem &DefaultValue(std::string defaultValue);

    ConfigSetupItem &ValidValues(IntList validValue);
    ConfigSetupItem &ValidValues(StringList validValue);

 private:
    std::string item_name_;
    bool is_required_;
    ConfigItemDataType item_type_;
    bool is_unset_;

    IntList valid_values_int_;
    StringList valid_values_str_;

    int default_value_int_;
    std::string default_value_str_;
};

}   // namespace common
}   // namespace duplitrace

#endif  // CONFIGSETUPITEM_H_
