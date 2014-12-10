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

#include    <string>
#include    <chrono>

namespace piw { namespace app {

    struct Configuration
    {
        Configuration& merge (const Configuration&, bool = false);

        std::chrono::milliseconds pollInterval;
        double temperatureSensitivity;
        double barometerSensitivity;
        double humiditySensitivity;
        double illuminanceSensitivity;
        std::string host;
        std::uint16_t port;
        int button;
        std::string dbPath;
        std::chrono::seconds saveInterval;
        std::chrono::seconds lcdTimeout;
    };
}}

#endif /* PIW_CONFIGURATION_INC */

