/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * LcdView.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_VIEW_LCDVIEW_INC
#define PIW_VIEW_LCDVIEW_INC

#include    <device/Lcd.hpp>
#include    "view/Dimensions.hpp"

#include    <utility>

namespace piw { namespace view {

    class LcdView
    {
        public:
            virtual ~LcdView () {}

            virtual void onError (const std::exception&);

            const Dimensions& dimensions () const;

        protected:
            LcdView (device::Lcd&, Dimensions);

            device::Lcd& lcd ();
            const device::Lcd& lcd () const;

            void write (std::wstring);

        private:
            device::Lcd& lcd_;
            Dimensions dimensions_;
    };

    inline LcdView::LcdView (device::Lcd& lcd, Dimensions dimensions) :
        lcd_ (lcd),
        dimensions_ (std::move (dimensions))
    {}

    inline device::Lcd& LcdView::lcd ()
    { return lcd_; }

    inline const device::Lcd& LcdView::lcd () const
    { return lcd_; }

    const Dimensions& LcdView::dimensions () const
    { return dimensions_; }
}}

#endif /* PIW_VIEW_LCDVIEW_INC */

