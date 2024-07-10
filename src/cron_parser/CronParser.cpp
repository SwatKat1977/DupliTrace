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
