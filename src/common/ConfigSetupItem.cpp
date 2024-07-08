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
#include "ConfigSetupItem.h"

namespace duplitrace { namespace common {

ConfigSetupItem::ConfigSetupItem(
    std::string itemName,
    ConfigItemDataType dataType) : item_name_(itemName),
                                   is_required_(false),
                                   item_type_(dataType),
                                   is_unset_(true),
                                   default_value_int_(0),
                                   default_value_str_("") {
}

std::string ConfigSetupItem::ItemName() {
    return item_name_;
}

ConfigItemDataType ConfigSetupItem::Type() {
    return item_type_;
}

bool ConfigSetupItem::IsUnset() {
    return is_unset_;
}

bool ConfigSetupItem::IsRequired() {
    return is_required_;
}

int ConfigSetupItem::DefaultIntValue() {
    return default_value_int_;
}

std::string ConfigSetupItem::DefaultStringValue() {
    return default_value_str_;
}

IntList ConfigSetupItem::ValidIntValues() {
    return valid_values_int_;
}

StringList ConfigSetupItem::ValidStringValues() {
    return valid_values_str_;
}

ConfigSetupItem &ConfigSetupItem::IsRequired(bool state) {
    is_required_ = state;
    return *this;
}

ConfigSetupItem &ConfigSetupItem::DefaultValue(int defaultValue) {
    default_value_int_ = defaultValue;
    is_unset_ = false;
    return *this;
}

ConfigSetupItem &ConfigSetupItem::DefaultValue(std::string defaultValue) {
    default_value_str_ = defaultValue;
    is_unset_ = false;
    return *this;
}

ConfigSetupItem &ConfigSetupItem::ValidValues(IntList validValue) {
    valid_values_int_ = validValue;
    return *this;
}

ConfigSetupItem &ConfigSetupItem::ValidValues(StringList validValue) {
    valid_values_str_ = validValue;
    return *this;
}

}   // namespace common
}   // namespace duplitrace
