/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * IpView.cpp is free software copyrighted by David Daniel.
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
#include    "view/IpView.hpp"

#include    <device/IpAddress.hpp>

#include    <string>

namespace piw { namespace view {

    void IpView::display ()
    {
        using device::Lcd;
        typedef Lcd::Metrics Metrics;

        device::IpAddress ip;
        std::wstring address = ip.get ();

        if (address.size () > Metrics::Columns) {
            address.erase (Metrics::Columns);
        }

        const unsigned column = (Metrics::Columns - address.size ()) / 2;

        lcd.write (0, 5, L"IP Address");
        lcd.write (2, column, address);
    }
}}

