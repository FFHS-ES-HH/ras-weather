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

#include    <string>

namespace piw { namespace view {

    IpView::IpView (device::Lcd& l, sensors::IpAddress& addr) :
        lcd (l),
        address (addr)
    {}

    void IpView::valueChanged ()
    {
        typedef device::Lcd::Metrics Metrics;

        std::wstring addr = address.get ();

        if (addr.size () > Metrics::Columns) {
            addr.erase (Metrics::Columns);
        }

        const unsigned column = (Metrics::Columns - addr.size ()) / 2;

        lcd.write (0, 4, L"IP Addresse:");
        lcd.write (2, column, addr);
    }
}}

