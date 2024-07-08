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
#ifndef LOGGERSETTINGS_H_
#define LOGGERSETTINGS_H_
#include <string>
#include "ConfigSetup.h"
#include "ConfigSetupItem.h"

namespace duplitrace {

const char LOGGING_SECTION[] = "logging";

const char LOGGING_LOG_LEVEL[] = "log_level";
const char LOGGING_LOG_LEVEL_DEBUG[] = "DEBUG";
const char LOGGING_LOG_LEVEL_INfO[] = "INFO";

const char LOGGING_LOG_TO_CONSOLE[] = "log_to_console";
const char LOGGING_LOG_TO_CONSOLE_YES[] = "YES";
const char LOGGING_LOG_TO_CONSOLE_NO[] = "NO";

const char LOGGING_LOG_FILENAME[] = "log_filename";

const char LOGGING_MAX_FILE_SIZE[] = "max_file_size";

const char LOGGING_MAX_FILE_COUNT[] = "max_file_count";

const char LOGGING_LOG_FORMAT[] = "log_format";
const char LOGGING_LOG_FORMAT_DEFAULT[] = "%Y-%m-%d %H:%M:%S %^%l%$ [%n] %v";
const int LOGGING_MAX_FILE_SIZE_DEFAULT = 1024;
const int LOGGING_MAX_FILE_ROTATE_COUNT_DEFAULT = 2;

const common::SectionList LoggerSettings = {
    {
        LOGGING_LOG_LEVEL,
        common::ConfigSetupItem(LOGGING_LOG_LEVEL,
                                common::CONFIG_ITEM_TYPE_STRING)
                .DefaultValue(LOGGING_LOG_LEVEL_INfO)
                .ValidValues(common::StringList{ LOGGING_LOG_LEVEL_DEBUG,
                                                 LOGGING_LOG_LEVEL_INfO })
    },
    {
        LOGGING_LOG_TO_CONSOLE,
        common::ConfigSetupItem(LOGGING_LOG_TO_CONSOLE,
                                common::CONFIG_ITEM_TYPE_STRING)
                .DefaultValue(LOGGING_LOG_TO_CONSOLE_NO)
                .ValidValues(common::StringList{ LOGGING_LOG_TO_CONSOLE_YES,
                                                 LOGGING_LOG_TO_CONSOLE_NO })
    },
    {
        LOGGING_LOG_FILENAME,
        common::ConfigSetupItem(LOGGING_LOG_FILENAME,
                                common::CONFIG_ITEM_TYPE_STRING)
                .DefaultValue("")
    },
    {
        LOGGING_MAX_FILE_SIZE,
        common::ConfigSetupItem(LOGGING_MAX_FILE_SIZE,
                                common::CONFIG_ITEM_TYPE_INTEGER)
                .DefaultValue(LOGGING_MAX_FILE_SIZE_DEFAULT)
    },
    {
        LOGGING_MAX_FILE_COUNT,
        common::ConfigSetupItem(LOGGING_MAX_FILE_COUNT,
                                common::CONFIG_ITEM_TYPE_INTEGER)
                .DefaultValue(LOGGING_MAX_FILE_ROTATE_COUNT_DEFAULT)
    },
    {
        LOGGING_LOG_FORMAT,
        common::ConfigSetupItem(LOGGING_LOG_FORMAT,
                                common::CONFIG_ITEM_TYPE_STRING)
                .DefaultValue(LOGGING_LOG_FORMAT_DEFAULT)
    }
};

#define GET_LOGGING_LOG_TO_CONSOLE config_manager_.GetStringEntry(\
            LOGGING_SECTION, LOGGING_LOG_TO_CONSOLE)

#define GET_LOGGING_LOG_FILENAME config_manager_.GetStringEntry(\
            LOGGING_SECTION, LOGGING_LOG_FILENAME)

#define GET_LOGGING_MAX_FILE_SIZE config_manager_.GetIntEntry(\
            LOGGING_SECTION, LOGGING_MAX_FILE_SIZE)

#define GET_LOGGING_MAX_FILE_COUNT config_manager_.GetIntEntry(\
            LOGGING_SECTION, LOGGING_MAX_FILE_COUNT)

#define GET_LOGGING_LOG_FORMAT config_manager_.GetStringEntry(\
            LOGGING_SECTION, LOGGING_LOG_FORMAT)

const int ONE_MEGABYTE = 1048576;

}   // namespace duplitrace

#endif  // LOGGERSETTINGS_H_
