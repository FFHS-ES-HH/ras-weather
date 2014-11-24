/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Configuration.cpp is free software copyrighted by David Daniel.
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
#include    "Configuration.hpp"

#include    <stdexcept>

#ifdef      CONFIG_H
#include    <config.h>
#endif

namespace piw {

    /**
     * Constructs a new Configuration.
     */
    Configuration::Configuration () :
        pollInterval_ (500),
        barometerSensitivity_ (),
        humiditySensitivity_ (),
        illuminanceSensitivity_ (),
        host_ ("localhost"),
        port_ (4223),
        dbPath_ (PIW_DB_PATH),
        connection_ ()
    {
        ipcon_create (&connection_);
    }

    Configuration::~Configuration ()
    {
        disconnect ();
        ipcon_destroy (connection_.get ());
    }

    IPConnection* Configuration::connection () const
    {
        if (!connected_) {
            throw std::runtime_error ("No connection available.");
        }

        return &connection_;
    }

    Configuration& Configuration::connect ()
    {
        if (!connected_) {
            if (ipcon_connect (&conection_, host_.c_str (), port_) < 0) {
                throw std::runtime_error ("Cannot connect to the device.");
            }
            connected_ = true;
        }

        return *this;
    }

    void Configuration::disconnect ()
    {
        if (connected_) {
            ipcon_disconnect (&connection_);
            connected_ = false;
        }
    }
}

