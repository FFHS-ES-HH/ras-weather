/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Application.cpp is free software copyrighted by David Daniel.
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
#include    "Application.hpp"

namespace piw {

    /**
     * Constructs a new Application.
     */
    Application::Application (const Configuration& config) :
        db (config.dbPath ()),
        connection (config.host (), config.port ()),
        uidRegistry (connection.get ()),
        button (connection.get (), uidRegistry, 1), /* TODO choose correct button */
        lcd (connection.get (), uidRegistry),
        dbWriter ()
    {}

    Application::~Application ()
    {}

    /**
     * Application::run
     * @return  
     */
    bool Application::run ()
    {
        bool success = true;

        return success;
    }
}

