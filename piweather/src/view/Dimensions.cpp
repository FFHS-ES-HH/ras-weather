/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Dimensions.cpp is free software copyrighted by David Daniel.
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
#include    "view/Dimensions.hpp"

#include    <device/Lcd.hpp>

#include    <stdexcept>

namespace piw { namespace view {

    namespace {

        unsigned checkLine (unsigned line)
        {
            if (line > Lcd::Metrics::Lines) {
                throw std::out_of_range {
                    "Invalid line given, the line must not exceed the number of "
                        "available lines on the lcd."};
            }
        }

        unsigned checkBegin (unsigned begin, unsigned end)
        {
            if (!(begin < end)) {
                throw std::out_of_range {
                    "Invalid range given, a dimensions `begin' must be smaller "
                        "than its `end'."};
            }

            if (end > (Lcd::Metrics::Columns + 2)) {
                throw std::out_of_range {
                    "Invalid `end' for a dimensions range given, `end' must not "
                        "exceed one past the last available column."};
            }

            return begin;
        }
    }

    Dimensions::Dimensions (unsigned line, unsigned begin, unsigned end) :
        line_ (line),
        begin_ (checkBegin (begin, end)),
        end_ (end)
    {}
}}
