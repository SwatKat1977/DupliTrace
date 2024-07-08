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
#include <string>
#include "INIReader.h"
#include "ConfigSetup.h"

namespace items
{
    namespace serviceFramework
    {
        class ConfigItemValue
        {
        public:
            ConfigItemValue ()
                : m_intValue (0), m_strValue ("")
            {
            }

            ConfigItemDataType GetItemType () { return m_itemType; }
            void SetItemType (ConfigItemDataType dataType) { m_itemType = dataType; }

            int GetIntValue () { return m_intValue; }
            void SetIntValue (int value) { m_intValue = value; }

            std::string GetStringValue () { return m_strValue; }
            void SetStringValue (std::string value) { m_strValue = value; }

        private:
            ConfigItemDataType m_itemType;
            int m_intValue;
            std::string m_strValue;

        };

        using ConfigItemValueItem = std::map<std::string, ConfigItemValue>;
        using ConfigItemMap = std::map<std::string, ConfigItemValueItem>;

        class ConfigManager
        {
        public:
            ConfigManager ();

            void Configure (ConfigSetup* layout, std::string configFile = "", bool fileRequired = false);

            bool processConfig ();

            int GetIntEntry (std::string sectionName, std::string itemName);

            std::string GetStringEntry (std::string sectionName, std::string itemName);

        private:
            std::string m_configFile;
            bool m_hasConfigFile;
            bool m_configFileRequired;
            ConfigSetup* m_layout;
            ConfigItemMap m_configItems;
            INIReader* m_configReader;

            bool ReadConfiguration ();

            int* ReadInt (std::string section,
                ConfigSetupItem fmt);

            std::string ReadStr (std::string section,
                ConfigSetupItem fmt);
        };

    }   // namespace serviceFramework
}   // namespace items

#endif  // CONFIGMANAGER_H_
