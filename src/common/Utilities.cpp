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
#include <algorithm>
#include <sstream>
#include <string>
#include "Utilities.h"

namespace duplitrace { namespace common {


constexpr bool StringContains(std::string_view str, char const ch) noexcept {
    return std::string_view::npos != str.find_first_of(ch);
}

std::vector<std::string> StringSplit(std::string_view text,
                                     char const delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream (text.data());

    while (std::getline (tokenStream, token, delimiter)) {
        tokens.push_back (token);
    }

    return tokens;
}

std::string ToUpper(std::string str) {
    std::transform (std::begin(str), std::end(str),
        std::begin(str), [](char const c) {
            return static_cast<char>(std::toupper (c));
        });

    return str;
}

}   // namespace common
}   // namespace duplitrace
