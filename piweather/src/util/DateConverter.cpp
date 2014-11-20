/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * DateConverter.cpp is free software copyrighted by David Daniel.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions.
 */
#include    "util/DateConverter.hpp"

#include    <ctime>

namespace piw { namespace util {

    typedef std::unique_lock<std::mutex> Lock;

    namespace {

        struct Format
        {
            static const char* const full;
        };

        const char* const Format::full = "%Y-%m-%dT%H:%I:%M.%S%z";
    }

    /**
     * Creates a suitable representation of the given date.
     * @return a string representation of the given date 
     */
    std::string DateConverter::toString (const std::chrono::system_clock::time_point& when) const
    {
        time_t as_time_t = std::chrono::system_clock::to_time_t (when);

        struct tm ltime;
        localtime_r (&as_time_t, &ltime);

        char buffer [32];
        std::strftime (buffer, sizeof buffer, Format::full, &ltime);

        return std::string (buffer);
    }
}}
