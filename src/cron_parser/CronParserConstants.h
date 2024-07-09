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
#ifndef CRONPARSERCONSTANTS_H_
#define CRONPARSERCONSTANTS_H_
#include "Platform.h"

#if (DUPLITRACE_PLATFORM == DUPLITRACE_PLATFORM_WINDOWS_MSVC)
  #if defined(_MSVC_LANG) && _MSVC_LANG == 201703L
#else
  #if __cplusplus < 201703L
#endif
    #error Requires C++ 17 standard or above
  #endif
#endif

namespace duplitrace { namespace cronparser {

using cronparser_int  = uint8_t;

enum class cron_field
{
    seconds,
    minutes,
    hours,
    day_of_month,
    month,
    day_of_week,
};

/* The minimum value for the seconds field in a cron expression */
const cronparser_int CRONPARSER_MINIMUM_SECONDS = 0;

/* The maximum value for the seconds field in a cron expression */
const cronparser_int CRONPARSER_MAXIMUM_SECONDS = 59;

/* The minimum value for the minutes field in a cron expression */
const cronparser_int CRONPARSER_MINIMUM_MINUTES = 0;

/* The maximum value for the minutes field in a cron expression */
const cronparser_int CRONPARSER_MAXIMUM_MINUTES = 59;

/* The minimum value for the hours field in a cron expression */
const cronparser_int CRONPARSER_MINIMUM_HOURS = 0;

/* The maximum value for the hours field in a cron expression */
const cronparser_int CRONPARSER_MAXIMUM_HOURS = 23;

/* The minimum value for the day of the week field in a cron expression */
const cronparser_int CRONPARSER_MINIMUM_DAYS_OF_WEEK = 0;

/* The maximum value for the day of the week field in a cron expression */
const cronparser_int CRONPARSER_MAXIMUM_DAYS_OF_WEEK = 6;

/* The minimum value for the day of the month field in a cron expression */
const cronparser_int CRONPARSER_MINIMUM_DAYS_OF_MONTH = 1;

/* The maximum value for the day of the month field in a cron expression */
const cronparser_int CRONPARSER_MAXIMUM_DAYS_OF_MONTH = 31;

/* The minimum value for the month field in a cron expression */
const cronparser_int CRONPARSER_MINIMUM_MONTHS = 1;

/* The maximum value for the month field in a cron expression */
const cronparser_int CRONPARSER_MAXIMUM_MONTHS = 12;

/*
 Days of the week are typically represented as:
 0 - Sunday
 1 - Monday
 2 - Tuesday
 3 - Wednesday
 4 - Thursday
 5 - Friday
 6 - Saturday
*/
const std::vector<std::string> DAY_OF_WEEK = {
    "SUN",
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT"
};

/*
 Months are typically represented as:
 1 - January
 2 - February
 3 - March
 4 - April
 5 - May
 6 - June
 7 - July
 8 - August
 9 - September
 10 - October
 11 - November
 12 - December
*/
const std::vector<std::string> MONTH = {
    "NONE",
    "JAN",
    "FEB",
    "MAR",
    "APR",
    "MAY",
    "JUN",
    "JUL",
    "AUG",
    "SEP",
    "OCT",
    "NOV",
    "DEC"
};

}  // namespace cronparser
}  // namespace duplitrace

#endif  // CRONPARSERCONSTANTS_H_
