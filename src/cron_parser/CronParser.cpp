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

    Code is based on croncpp by Mariusbancila:
        https://github.com/mariusbancila/croncpp
*/
#include "CronParser.h"

namespace duplitrace { namespace cronparser {

const char SPECIAL_CHARACTER_ASTERISK = '*';
const char SPECIAL_CHARACTER_COMMA = ',';
const char SPECIAL_CHARACTER_HYPHEN = '-';

CronExpression::CronExpression(std::string_view expression) {
    if (expression.empty())
        throw BadCronExpression("Invalid empty cron expression");

    auto fields = common::StringSplit(expression, ' ');
    fields.erase (
        std::remove_if (std::begin (fields), std::end (fields),
            [](std::string_view s) {return s.empty (); }),
        std::end (fields));

    if (fields.size () != 6)
        throw BadCronExpression("cron expression must have six fields");

    SetCronField(fields[0], seconds_, CRONPARSER_MINIMUM_SECONDS,
                 CRONPARSER_MAXIMUM_SECONDS);
    SetCronField(fields[1], minutes_, CRONPARSER_MINIMUM_MINUTES,
                 CRONPARSER_MAXIMUM_MINUTES);
    SetCronField(fields[2], hours_, CRONPARSER_MINIMUM_HOURS,
                 CRONPARSER_MAXIMUM_HOURS);

    SetDaysOfWeek(fields[5], days_of_week_);
    SetDaysOfMonth(fields[3], days_of_month_);
    SetMonths(fields[4], months_);

    expression_string_ = expression;
}

bool CronExpression::operator==(const CronExpression &right) {
    return
        seconds_ == right.seconds_ &&
        minutes_ == right.minutes_ &&
        hours_ == right.hours_ &&
        days_of_week_ == right.days_of_week_ &&
        days_of_month_ == right.days_of_month_ &&
        months_ == right.months_;
}

bool CronExpression::operator!=(const CronExpression &right) {
    return !(*this == right);
}

std::tm CronExpression::getNextTriggerTime(const std::tm& start_time) {
    std::tm next_time = start_time;

    // Start checking from the next second
    next_time.tm_sec += 1;

    while (true) {

        // If seconds exceed the maximum allowed value
        // (CRONPARSER_BITFIELD_VALUE_SECONDS), reset to 0 and increment
        // minutes.
        if (next_time.tm_sec >= CRONPARSER_BITFIELD_VALUE_SECONDS) {
            next_time.tm_sec = 0;
            next_time.tm_min += 1;
        }

        // If minutes exceed the maximum allowed value
        // (CRONPARSER_BITFIELD_VALUE_MINUTES), reset to 0 and increment hours.
        if (next_time.tm_min >= CRONPARSER_BITFIELD_VALUE_MINUTES) {
            next_time.tm_min = 0;
            next_time.tm_hour += 1;
        }

        // If hours exceed the maximum allowed value
        // (CRONPARSER_BITFIELD_VALUE_HOURS), reset to 0 and increment the day
        // of the month.
        if (next_time.tm_hour >= CRONPARSER_BITFIELD_VALUE_HOURS) {
            next_time.tm_hour = 0;
            next_time.tm_mday += 1;
        }

        // Handle non-existent dates (e.g., April 31) and leap years (e.g.,
        // February 30).
        if (next_time.tm_mday > CRONPARSER_BITFIELD_VALUE_DAYS_OF_MONTH ||
            (next_time.tm_mon == 1 && next_time.tm_mday == 30 &&
            !((next_time.tm_year + 1900) % 4 == 0 &&
            ((next_time.tm_year + 1900) % 100 != 0 ||
            (next_time.tm_year + 1900) % 400 == 0)))) {
            next_time.tm_mday = 1;
            next_time.tm_mon += 1;
        }

        // If months exceed the maximum allowed value
        // (CRONPARSER_BITFIELD_VALUE_MONTHS), reset to 0 and increment the
        // year.
        if (next_time.tm_mon >= CRONPARSER_BITFIELD_VALUE_MONTHS) {
            next_time.tm_mon = 0;
            next_time.tm_year += 1;
        }

        // Normalise the time structure after any changes
        std::mktime (&next_time);

        // Check if the current next_time values match the cron expression
        // fields (seconds, minutes, hours, days of the month, months, and days
        // of the week).The.test () method is used to check if the specific
        // field value is valid according to the cron expression.
        // * seconds_.test (next_time.tm_sec) : Checks if the current second is
        //   valid.
        // * minutes_.test (next_time.tm_min) : Checks if the current minute is
        //   valid.
        // * hours_.test (next_time.tm_hour) : Checks if the current hour is
        //   valid.
        // * days_of_week_.test (next_time.tm_mday - 1) : Checks if the current
        //   day of the month is valid (subtracting 1 because tm_mday is
        //   1 - based).
        // * months_.test (next_time.tm_mon) : Checks if the current month is
        //    valid.
        // * days_of_week_.test (next_time.tm_wday) : Checks if the current day
        //   of the week is valid.
        if (seconds_.test(next_time.tm_sec) &&
            minutes_.test(next_time.tm_min) &&
            hours_.test(next_time.tm_hour) &&
            days_of_week_.test(static_cast<size_t>(next_time.tm_mday) -1) &&
            months_.test(next_time.tm_mon) &&
            days_of_week_.test (next_time.tm_wday)) {
            break;
        }

        next_time.tm_sec += 1;
    }

    return next_time;
}

cronparser_int CronExpression::ToCronParserInt(std::string_view text) {
    try {
        return static_cast<cronparser_int>(std::stoul (text.data ()));
    }
    catch (std::exception const& ex) {
        throw BadCronExpression(ex.what ());
    }
}

std::pair<cronparser_int, cronparser_int> CronExpression::CreateIntRange (
        std::string_view field,
        cronparser_int const minval,
        cronparser_int const maxval) {
    cronparser_int firstValue = 0;
    cronparser_int lastValue = 0;

    if (field.size() == 1 && field[0] == SPECIAL_CHARACTER_ASTERISK) {
        firstValue = minval;
        lastValue = maxval;
    }
    else if (!common::StringContains(field, SPECIAL_CHARACTER_HYPHEN)) {
        firstValue = ToCronParserInt(field);
        lastValue = firstValue;
    }
    else {
        auto parts = common::StringSplit(field, SPECIAL_CHARACTER_HYPHEN);
        if (parts.size () != 2)
            throw BadCronExpression("Specified range requires two fields");

        firstValue = ToCronParserInt(parts[0]);
        lastValue = ToCronParserInt(parts[1]);
    }

    if (firstValue > maxval || lastValue > maxval) {
        throw BadCronExpression("Specified range exceeds maximum");
    }
    if (firstValue < minval || lastValue < minval) {
        throw BadCronExpression("Specified range is less than minimum");
    }
    if (firstValue > lastValue) {
        throw BadCronExpression("Specified range start exceeds range end");
    }

    return { firstValue, lastValue };
}

template <size_t SIZE>
void CronExpression::SetCronField (std::string_view value, std::bitset<SIZE>& target,
    cronparser_int const minValue,
    cronparser_int const maxValue) {
    // Check that cron field doesn't end with a comma.
    if (value.length() > 0 &&
        value[value.length() - 1] == SPECIAL_CHARACTER_COMMA) {
        throw BadCronExpression("Value cannot end with comma");
    }

    // Split the string with the ',' delimiter, if the fields are empty then
    // generate an exception.
    auto fields = common::StringSplit (value, SPECIAL_CHARACTER_COMMA);
    if (fields.empty ()) {
        throw BadCronExpression("Cron expression cannot be parsed");
    }

    for (auto const& field : fields) {
        if (!common::StringContains(field, '/')) {
            auto [first, last] = CreateIntRange(field, minValue, maxValue);

            for (cronparser_int i = first - minValue; i <= last - minValue; ++i) {
                target.set (i);
            }
        }
        else {
            auto parts = common::StringSplit(field, '/');
            if (parts.size () != 2)
                throw BadCronExpression ("Incrementer must have two fields");

            auto [first, last] = CreateIntRange(parts[0], minValue, maxValue);

            if (!common::StringContains(parts[0], '-')) {
                last = maxValue;
            }

            auto delta = ToCronParserInt(parts[1]);
            if (delta <= 0)
                throw BadCronExpression("Incrementer must be a positive value");

            for (cronparser_int i = first - minValue; i <= last - minValue;
                i += delta) {
                target.set (i);
            }
        }
    }
}

void CronExpression::SetDaysOfWeek(std::string value, std::bitset<7>& target) {
    auto days = common::ToUpper(value);
    auto days_replaced = ReplaceOrdinals(days, DAY_OF_WEEK);

    if (days_replaced.size () == 1 && days_replaced[0] == '?')
        days_replaced[0] = '*';

    SetCronField(days_replaced, target, CRONPARSER_MINIMUM_DAYS_OF_WEEK,
                 CRONPARSER_MAXIMUM_DAYS_OF_WEEK);
}

void CronExpression::SetDaysOfMonth(std::string value,
                                    std::bitset<31>& target) {
    if (value.size () == 1 && value[0] == '?')
        value[0] = '*';

    SetCronField(value, target, CRONPARSER_MINIMUM_DAYS_OF_MONTH,
                 CRONPARSER_MAXIMUM_DAYS_OF_MONTH);
}

void CronExpression::SetMonths(std::string value, std::bitset<12>& target) {
    auto month = common::ToUpper(value);
    auto month_replaced = ReplaceOrdinals(month, MONTH);

    SetCronField(month_replaced, target, CRONPARSER_MINIMUM_MONTHS,
                 CRONPARSER_MAXIMUM_MONTHS);
}

std::string CronExpression::ReplaceOrdinals(
    std::string text, std::vector<std::string> const& replacement) {
    for (size_t i = 0; i < replacement.size (); ++i)
    {
        auto pos = text.find (replacement[i]);
        if (std::string::npos != pos)
            text.replace (pos, 3, std::to_string (i));
    }

    return text;
}

}  // namespace cronparser
}  // namespace duplitrace
