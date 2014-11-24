/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Lcd.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_DEVICE_LCD_INC
#define PIW_DEVICE_LCD_INC

#include    <bricklet_lcd_20x4.h>

#include    <device/UidRegistry.hpp>

#include    <string>
#include    <memory>

namespace piw { namespace device {

    class Lcd
    {
        public:
            Lcd (IPConnection*, const device::UidRegistry&);
            ~Lcd ();

            Lcd& write (unsigned, unsigned, const std::string&);
            Lcd& backlightOn ();
            Lcd& backlightOff ();
            bool isBacklightOn () const;
            Lcd& clear ();

        private:
            std::unique_ptr<LCD20x4> lcd_;
    };
}}

#endif /* PIW_DEVICE_LCD_INC */

