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

const int MAX_YEARS_AHEAD = 5;

template <size_t N>
int findNextSetBit(const std::bitset<N>& bits, int start, int max) {
    for (int i = start + 1; i < max; ++i) {
        if (bits.test(i)) return i;
    }
    for (int i = 0; i <= start; ++i) {
        if (bits.test(i)) return i;
    }
    return 0; // fallback
}

template <size_t N>
int getLowestSetBit(const std::bitset<N>& bits) {
    for (int i = 0; i < static_cast<int>(bits.size()); ++i) {
        if (bits.test(i)) return i;
    }
    return 0;
}

CronExpression::CronExpression(std::string_view expression) {
    if (expression.empty())
        throw BadCronExpression("Invalid empty cron expression");

    auto fields = common::StringSplit(expression, ' ');
    fields.erase(
        std::remove_if(std::begin(fields), std::end(fields),
            [](std::string_view s) {return s.empty (); }),
        std::end(fields));

    if (fields.size() != 6)
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

bool CronExpression::operator==(const CronExpression &right) const {
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
    std::tm candidate = start_time;
    candidate.tm_sec += 1;

    const int start_year = candidate.tm_year;

    while (true) {
        std::mktime(&candidate);  // Normalize time

        if (candidate.tm_year >= start_year + MAX_YEARS_AHEAD) {
            throw BadCronExpression("No cron match found within 5 years");
        }

        // Try to jump to next valid second
        if (!seconds_.test(candidate.tm_sec)) {
            int next_sec = findNextSetBit(seconds_, candidate.tm_sec, 60);
            if (next_sec <= candidate.tm_sec) {
                candidate.tm_min += 1;
            }
            candidate.tm_sec = next_sec;
            continue;
        }

        // Try to jump to next valid minute
        if (!minutes_.test(candidate.tm_min)) {
            int next_min = findNextSetBit(minutes_, candidate.tm_min, 60);
            if (next_min <= candidate.tm_min) {
                candidate.tm_hour += 1;
            }
            candidate.tm_min = next_min;
            candidate.tm_sec = getLowestSetBit(seconds_);
            continue;
        }

        // Try to jump to next valid hour
        if (!hours_.test(candidate.tm_hour)) {
            int next_hour = findNextSetBit(hours_, candidate.tm_hour, 24);
            if (next_hour <= candidate.tm_hour) {
                candidate.tm_mday += 1;
            }
            candidate.tm_hour = next_hour;
            candidate.tm_min = getLowestSetBit(minutes_);
            candidate.tm_sec = getLowestSetBit(seconds_);
            continue;
        }

        // Try to jump to next valid month
        if (!months_.test(candidate.tm_mon)) {
            int next_month = findNextSetBit(months_, candidate.tm_mon, 12);
            if (next_month <= candidate.tm_mon) {
                candidate.tm_year += 1;
            }
            candidate.tm_mon = next_month;
            candidate.tm_mday = 1;
            candidate.tm_hour = getLowestSetBit(hours_);
            candidate.tm_min = getLowestSetBit(minutes_);
            candidate.tm_sec = getLowestSetBit(seconds_);
            continue;
        }

        // Day-of-month or day-of-week logic
        bool dom_is_star = days_of_month_.count() == 31; // all 31 bits set
        bool dow_is_star = days_of_week_.count() == 7;   // all 7 bits set

        bool dom_ok = days_of_month_.test(candidate.tm_mday - 1);
        bool dow_ok = days_of_week_.test(candidate.tm_wday);

        // Rules:
        // - If both are restricted → require both match
        // - If one is wildcard → only require the other to match
        // - If both are wildcards → always match

        bool match = false;
        if (!dom_is_star && !dow_is_star) {
            match = dom_ok && dow_ok;
        } else {
            match = dom_ok && dow_is_star || dow_ok && dom_is_star;
        }

        if (!match) {
            candidate.tm_mday += 1;
            candidate.tm_hour = getLowestSetBit(hours_);
            candidate.tm_min = getLowestSetBit(minutes_);
            candidate.tm_sec = getLowestSetBit(seconds_);
            continue;
        }

        return candidate;  // All fields match!
    }
}

cronparser_int CronExpression::ToCronParserInt(std::string_view text) {
    try {
        return static_cast<cronparser_int>(std::stoul (text.data ()));
    }
    catch (std::exception const& ex) {
        throw BadCronExpression(ex.what());
    }
}

template <size_t N>
void CronExpression::CreateIntRange(std::string_view field,
                                    cronparser_int minval,
                                    cronparser_int maxval,
                                    std::bitset<N>& target) {
    cronparser_int step = 1;
    std::string_view rangePart = field;

    // Handle "*/step" or "start-end/step"
    auto slashPos = field.find('/');
    if (slashPos != std::string_view::npos) {
        rangePart = field.substr(0, slashPos);
        std::string_view stepPart = field.substr(slashPos + 1);

        if (stepPart.empty())
            throw BadCronExpression("Step value missing after '/'");

        step = ToCronParserInt(stepPart);
        if (step < 1)
            throw BadCronExpression("Step must be >= 1");
    }

    cronparser_int firstValue = 0;
    cronparser_int lastValue = 0;

    if (rangePart.size() == 1 && rangePart[0] == SPECIAL_CHARACTER_ASTERISK) {
        firstValue = minval;
        lastValue = maxval;
    } else if (!common::StringContains(rangePart, SPECIAL_CHARACTER_HYPHEN)) {
        firstValue = ToCronParserInt(rangePart);
        lastValue = firstValue;
    } else {
        auto parts = common::StringSplit(rangePart, SPECIAL_CHARACTER_HYPHEN);
        if (parts.size() != 2)
            throw BadCronExpression("Range requires two values");

        firstValue = ToCronParserInt(parts[0]);
        lastValue = ToCronParserInt(parts[1]);
    }

    // Validation
    if (firstValue < minval || lastValue > maxval)
        throw BadCronExpression("Range out of bounds");

    if (firstValue > lastValue)
        throw BadCronExpression("Range start exceeds end");

    for (cronparser_int val = firstValue; val <= lastValue; val += step) {
        target.set(val - minval);
    }
}

template <size_t SIZE>
void CronExpression::SetCronField(std::string_view value,
                                  std::bitset<SIZE>& target,
                                  cronparser_int minValue,
                                  cronparser_int maxValue) {
    // Check that the field doesn't end with a comma
    if (!value.empty() && value.back() == SPECIAL_CHARACTER_COMMA) {
        throw BadCronExpression("Value cannot end with comma");
    }

    // Split on commas
    auto fields = common::StringSplit(value, SPECIAL_CHARACTER_COMMA);
    if (fields.empty()) {
        throw BadCronExpression("Cron expression cannot be parsed");
    }

    for (auto const& field : fields) {
        CreateIntRange(field, minValue, maxValue, target);
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
    for (size_t i = 0; i < replacement.size(); ++i) {
        auto pos = text.find(replacement[i]);
        if (std::string::npos != pos)
            text.replace(pos, 3, std::to_string(i));
    }

    return text;
}

}  // namespace cronparser
}  // namespace duplitrace
