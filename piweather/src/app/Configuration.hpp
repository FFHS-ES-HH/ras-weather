/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Configuration.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_CONFIGURATION_INC
#define PIW_CONFIGURATION_INC

#include    <utility>
#include    <string>

#include    <ip_connection.h>

namespace piw { namespace app {

    struct Configuration
    {
        Configuration ();

        // unsigned pollInterval () const;
        // Configuration& pollInterval (unsigned);
        // unsigned temperatureSensitivity () const;
        // Configuration& temperatureSensitivity (unsigned);
        // double barometerSensitivity () const;
        // Configuration& barometerSensitivity (double);
        // double humiditySensitivity () const;
        // Configuration& humiditySensitivity (double);
        // double illuminanceSensitivity () const;
        // Configuration& illuminanceSensitivity (double);
        // const std::string& host () const;
        // Configuration& host (std::string);
        // unsigned port () const;
        // Configuration& port (unsigned);
        // const std::string& dbPath () const;
        // Configuration& dbPath (std::string);
        // Configuration& button (unsigned);
        // unsigned button () const;

        // private:
            // unsigned pollInterval_;
            // double temperatureSensitivity_;
            // double barometerSensitivity_;
            // double humiditySensitivity_;
            // double illuminanceSensitivity_;
            // std::string host_;
            // unsigned port_;
            // unsigned button_;
            // std::string dbPath_;

        Configuration& merge (const Configuration&, bool = true);

        unsigned pollInterval;
        double temperatureSensitivity;
        double barometerSensitivity;
        double humiditySensitivity;
        double illuminanceSensitivity;
        std::string host;
        unsigned port;
        unsigned button;
        std::string dbPath;
    };

    // inline unsigned Configuration::pollInterval () const
    // { return pollInterval_; }

    // inline Configuration& Configuration::pollInterval (unsigned i)
    // {
        // pollInterval_ = i;
        // return *this;
    // }

    // inline double Configuration::barometerSensitivity () const
    // { return barometerSensitivity_; }

    // inline Configuration& Configuration::barometerSensitivity (double sensitivity)
    // {
        // barometerSensitivity_ = sensitivity;
        // return *this;
    // }

    // inline double Configuration::humiditySensitivity () const
    // { return humiditySensitivity_; }

    // inline Configuration& Configuration::humiditySensitivity (double sensitivity)
    // {
        // humiditySensitivity_ = sensitivity;
        // return *this;
    // }

    // inline double Configuration::illuminanceSensitivity () const
    // { return illuminanceSensitivity_; }

    // inline Configuration& Configuration::illuminanceSensitivity (double sensitivity)
    // {
        // illuminanceSensitivity_ = sensitivity;
        // return *this;
    // }

    // inline const std::string& Configuration::host () const
    // { return host_; }

    // inline Configuration& Configuration::host (std::string host)
    // {
        // host_ = std::move (host);
        // return *this;
    // }

    // inline unsigned Configuration::port () const
    // { return port_; }

    // inline Configuration& Configuration::port (unsigned port)
    // {
        // port_ = port;
        // return *this;
    // }

    // inline const std::string& Configuration::dbPath () const
    // { return dbPath_; }

    // inline Configuration& Configuration::dbPath (std::string path)
    // {
        // dbPath_ = std::move (path);
        // return *this;
    // }

    // inline unsigned Configuration::temperatureSensitivity () const
    // { return temperatureSensitivity_; }

    // inline Configuration& Configuration::temperatureSensitivity (unsigned sensitivity)
    // {
        // temperatureSensitivity_ = sensitivity;
        // return *this;
    // }

    // inline Configuration& Configuration::button (unsigned which)
    // {
        // button_ = which;
        // return *this;
    // }

    // inline unsigned Configuration::button () const
    // { return button_; }
}}

#endif /* PIW_CONFIGURATION_INC */

