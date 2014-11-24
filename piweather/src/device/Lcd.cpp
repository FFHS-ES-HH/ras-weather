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
#include    "device/Lcd.hpp"

#include    <stdexcept>

namespace piw { namespace device {

    namespace {

        struct LcdMetrics
        {
            static const unsigned Lines = 4;
            static const unsigned Columns = 20;
        };
    }

    Lcd::Lcd (IPConnection* connection, const device::UidRegistry& registry) :
        lcd_ (new LCD20x4)
    {
        lcd_20x4_create (
                lcd_.get (),
                registry.getUid (LCD_20X4_DEVICE_IDENTIFIER).c_str (),
                connection);
    }

    Lcd::~Lcd ()
    {
        lcd_20x4_destroy (lcd_.get ());
    }

    Lcd& Lcd::backlightOn ()
    {
        if (lcd_20x4_backlight_on (lcd_.get ()) < 0) {
            throw std::runtime_error ("Cannot switch on the backlight of the lcd.");
        }

        return *this;
    }

    Lcd& Lcd::backlightOff ()
    {
        if (lcd_20x4_backlight_off (lcd_.get ()) < 0) {
            throw std::runtime_error ("Cannot switch off the backlight of the lcd.");
        }

        return *this;
    }

    bool Lcd::isBacklightOn () const
    {
        bool isOn = false;

        if (lcd_20x4_is_backlight_on (lcd_.get (), &isOn) < 0) {
            throw std::runtime_error ("Cannot evaluate whether the backlight is on.");
        }

        return isOn;
    }

    Lcd& Lcd::write (unsigned line, unsigned column, const std::string& text)
    {
        if (line > LcdMetrics::Lines || column > LcdMetrics::Columns) {
            throw std::invalid_argument (
                    "The given position to write on the lcd is beyond borders.");
        }

        if (lcd_20x4_write_line (lcd_.get (), line, column, text.c_str ()) < 0) {
            throw std::runtime_error ("Cannot write the given text on the lcd.");
        }

        return *this;
    }

    Lcd& Lcd::clear ()
    {
        if (lcd_20x4_clear_display (lcd_.get ()) < 0) {
            throw std::runtime_error ("Cannot clear the display on the lcd.");
        }

        return *this;
    }
}}

