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
#ifndef PLATFORM_H_
#define PLATFORM_H_
#include <assert.h>
#include <string>

#define DUPLITRACE_PLATFORM_WINDOWS      1
#define DUPLITRACE_PLATFORM_WINDOWS_MSVC 2
#define DUPLITRACE_PLATFORM_LINUX        3

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(__ANDROID__)
#  if defined(_MSC_VER)
#    define DUPLITRACE_PLATFORM DUPLITRACE_PLATFORM_WINDOWS_MSVC
#  else
#    define DUPLITRACE_PLATFORM DUPLITRACE_PLATFORM_WINDOWS_CORE
#  endif
#else
#    define DUPLITRACE_PLATFORM DUPLITRACE_PLATFORM_LINUX
#endif

#define ItemsAssert(expr, mesg) assert((expr) && (mesg))

std::string GetEnv (const char* field);

#endif  // PLATFORM_H_
