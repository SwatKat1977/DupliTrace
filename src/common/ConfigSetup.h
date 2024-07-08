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
#ifndef CONFIGSETUP_H_
#define CONFIGSETUP_H_
#include <map>
#include <string>
#include <vector>
#include "ConfigSetupItem.h"

namespace duplitrace { namespace common {

using SectionList = std::map<std::string, ConfigSetupItem>;
using SectionsMap = std::map<std::string, SectionList>;
using SectionKeysList = std::vector<std::string>;

// Class that defines the configuaration Format
class ConfigSetup {
 public:
    explicit ConfigSetup(SectionsMap &setupItems);

    SectionKeysList Sections();

    SectionList Section(std::string name);

 private:
    SectionsMap items_;
};

}   // namespace common
}   // namespace duplitrace

#endif  // CONFIGSETUP_H_
