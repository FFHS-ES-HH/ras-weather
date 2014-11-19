/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Lcd.cpp is free software copyrighted by David Daniel.
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
#include    "view/Lcd.hpp"

namespace piw { namespace view {

    using device::UidRegistry;

    /**
     * Constructs a new Lcd.
     */
    Lcd::Lcd (IPConnection* connection, const UidRegistry& registry)
    {
        lcd_20x4_create (
                &lcd_,
                registry.getUid (LCD_20X4_DEVICE_IDENTIFIER).c_str (),
                connection);
    }

    Lcd::~Lcd ()
    {
        lcd_20x4_destroy (&lcd_);
    }

    Lcd& Lcd::backlightOn ()
    {
        lcd_20x4_backlight_on (&lcd_);

        return *this;
    }

    Lcd& Lcd::backlightOff ()
    {
        lcd_20x4_backlight_off (&lcd_);

        return *this;
    }

    Lcd& Lcd::write (unsigned line, unsigned column, const std::string& text)
    {
        lcd_20x4_write_line (&lcd_, line, column, text.c_str ());

        return *this;
    }

    Lcd& Lcd::clear ()
    {
        lcd_20x4_clear_display (&lcd_);

        return *this;
    }
}}

