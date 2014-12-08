/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * LcdView.cpp is free software copyrighted by David Daniel.
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
#include    "view/LcdView.hpp"

namespace piw { namespace view {

    void LcdView::onError (const std::exception&)
    {
        write (L"??");
    }

    void LcdView::write (std::wstring text)
    {
        const Dimensions& dim = dimensions ();

        if (text.size () > dim.width ()) {
            text.erase (dim.width ());
        }

        lcd ().write (dim.line (), dim.begin (), text);
    }
}}
