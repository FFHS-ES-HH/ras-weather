/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Dimensions.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_VIEW_DIMENSIONS_INC
#define PIW_VIEW_DIMENSIONS_INC

namespace piw { namespace view {

    class Dimensions
    {
        public:
            Dimensions (unsigned, unsigned, unsigned);

            unsigned line () const;

            unsigned begin () const;
            unsigned end () const;

            unsigned first () const;
            unsigned last () const;

            unsigned width () const;

            static Dimensions byPosition (unsigned);

        private:
            unsigned line_;
            unsigned begin_;
            unsigned end_;
    };

    inline unsigned Dimensions::line () const
    { return line_; }

    inline unsigned Dimensions::begin () const
    { return begin_; }

    inline unsigned Dimensions::end () const
    { return end_; }

    inline unsigned Dimensions::first () const
    { return begin_; }

    inline unsigned Dimensions::last () const
    { return end_ - 1; }

    inline unsigned Dimensions::width () const
    { return end_ - begin_; }
}}

#endif /* PIW_VIEW_DIMENSIONS_INC */

