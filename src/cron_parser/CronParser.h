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
#ifndef CRONPARSER_H_
#define CRONPARSER_H_
#include <bitset>
#include <stdexcept>
#include "Platform.h"
#include "Utilities.h"
#include "CronParserConstants.h"

#if (DUPLITRACE_PLATFORM == DUPLITRACE_PLATFORM_WINDOWS_MSVC)
  #if defined(_MSVC_LANG) && _MSVC_LANG == 201703L
#else
  #if __cplusplus < 201703L
#endif
    #error Requires C++ 17 standard or above
  #endif
#endif

namespace duplitrace { namespace cronparser {

class  BadCronExpression : public std::runtime_error
{
 public:
    explicit BadCronExpression(std::string_view msg) :
        std::runtime_error(msg.data ()) {
    }
};

using BitsetSeconds = std::bitset<60>;
using BitsetMinutes = std::bitset<60>;
using BitsetHours = std::bitset<24>;
using BitsetDaysOfWeek = std::bitset<7>;
using BitsetDaysOfMonth = std::bitset<31>;
using BitsetMonths = std::bitset<12>;

class CronExpression {
 public:
     CronExpression(std::string_view expression);

     std::string Expression() { return expression_string_; }

     BitsetSeconds SecondsBits() { return seconds_; }
     BitsetMinutes MinutesBits() { return minutes_; }
     BitsetHours HoursBits() { return hours_; }
     BitsetDaysOfWeek DaysOfWeekBits() { return days_of_week_; }
     BitsetDaysOfMonth DaysOfMonthBits() { return days_of_month_;  }
     BitsetMonths MonthsBit() { return months_; }

 private:
     BitsetSeconds seconds_;
     BitsetMinutes minutes_;
     BitsetHours hours_;
     BitsetDaysOfWeek days_of_week_;
     BitsetDaysOfMonth days_of_month_;
     BitsetMonths months_;
     std::string     expression_string_;

     cronparser_int ToCronParserInt (std::string_view text);

     std::pair<cronparser_int, cronparser_int> CreateIntRange(
         std::string_view field,
         cronparser_int const minval,
         cronparser_int const maxval);

     template <size_t SIZE>
     void SetCronField (std::string_view value, std::bitset<SIZE>& target,
         cronparser_int const minValue,
         cronparser_int const maxValue);

     void SetDaysOfWeek(std::string value, std::bitset<7>& target);
     void SetDaysOfMonth(std::string value, std::bitset<31>& target);
     void SetMonths(std::string value, std::bitset<12>& target);

     std::string ReplaceOrdinals(std::string text,
                                 std::vector<std::string> const& replacement);
};

}  // namespace cronparser
}  // namespace duplitrace

#endif  // CRONPARSER_H_
