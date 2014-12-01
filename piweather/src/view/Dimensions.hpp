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
            Dimensions (unsigned, unsigned, unsigned, unsigned);

            unsigned topLeftX () const;
            unsigned topLeftY () const;
            unsigned bottomRightX () const;
            unsigned bottomRightY () const;
            unsigned width () const;
            unsigned height () const;

        private:
            unsigned topLeftX_;
            unsigned topLeftY_;
            unsigned bottomRightX_;
            unsigned bottomRightY_;
    };

    inline Dimensions::Dimensions (
            unsigned topLeftX,
            unsigned topLeftY,
            unsigned bottomRightX,
            unsigned bottomRightY) :

        topLeftX_ (topLeftX),
        topLeftY_ (topLeftY),
        bottomRightX_ (bottomRightX),
        bottomRightY_ (bottomRightY)
    {}

    inline unsigned Dimensions::topLeftX () const
    { return topLeftX_; }

    inline unsigned Dimensions::topLeftY () const
    { return topLeftY_; }

    inline unsigned Dimensions::bottomRightX () const
    { return bottomRightX_; }

    inline unsigned Dimensions::bottomRightY () const
    { return bottomRightY_; }

    inline unsigned Dimensions::width () const
    { return bottomRightX_ - topLeftX_; }

    inline unsigned Dimensions::height () const
    { return bottomRightY_ - topLeftY_; }
}}

#endif /* PIW_VIEW_DIMENSIONS_INC */

