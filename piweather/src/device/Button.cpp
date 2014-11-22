/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Button.cpp is free software copyrighted by David Daniel.
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
#include    "device/Button.hpp"

#include    <stdexcept>

namespace piw { namespace device {

    Button::Button (
            IPConnection* connection,
            const UidRegistry& registry,
            std::uint8_t which) :
        button_ (which)
    {
        lcd_20x4_create (
                &lcd_,
                registry.getUid (LCD_20X4_DEVICE_IDENTIFIER).c_str (),
                connection);

        lcd_20x4_register_callback (
                &lcd_,
                LCD_20X4_CALLBACK_BUTTON_PRESSED,
                reinterpret_cast<void*> (&Button::call),
                this);
    }

    Button::~Button ()
    {
        lcd_20x4_destroy (&lcd_);
    }

    void Button::call (std::uint8_t button, void* user_data)
    {
        Button* self = static_cast<Button*> (user_data);

        if (button == self->button_) {
            self->notifyObservers ();
        }
    }
}}

