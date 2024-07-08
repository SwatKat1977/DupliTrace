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
#include <stdexcept>
#include "ConfigSetup.h"

namespace items
{
    namespace serviceFramework
    {
        ConfigSetup::ConfigSetup(SectionsMap &setupItems)
            : m_items(setupItems)
        {
        }

        /*
        Get a list of sections available.

        returns:
            List of strings that represent the sections available.
        */
        SectionKeysList ConfigSetup::Sections()
        {
            std::vector<std::string> sections;
            for (auto it = m_items.begin(); it != m_items.end(); it++) {
                sections.push_back(it->first);
            }

            return sections;
        }

        /*
        Get a list of items within a given sections.

        returns:
            List of list of configuration items.
        */
        SectionList ConfigSetup::Section(std::string sectionName)
        {
            auto it = m_items.find(sectionName);

            if (it == m_items.end())
            {
                throw std::invalid_argument("Invalid section");
            }

            return it->second;
        }

    }   // namespace serviceFramework
}   // namespace items
