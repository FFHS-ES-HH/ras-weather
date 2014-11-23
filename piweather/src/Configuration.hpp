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

namespace piw {

    class Configuration
    {
        public:
            Configuration (int argc, char** argv);

            unsigned pollInterval () const;
            double barometerSensitivity () const;
            double humiditySensitivity () const;
            double illuminanceSensitivity () const;
            const std::string& host () const;
            unsigned port () const;

        private:
            unsigned pollInterval_;
            double barometerSensitivity_;
            double humiditySensitivity_;
            double illuminanceSensitivity_;
            std::string host_;
            unsigned port_;
    };

    inline unsigned Configuration::pollInterval () const
    { return pollInterval_; }

    inline double Configuration::barometerSensitivity () const
    { return barometerSensitivity_; }

    inline double Configuration::humiditySensitivity () const
    { return humiditySensitivity_; }

    inline double Configuration::illuminanceSensitivity () const
    { return illuminanceSensitivity_; }

    inline const std::string& Configuration::host () const
    { return host_; }

    inline unsigned Configuration::port () const
    { return port_; }
}

#endif /* PIW_CONFIGURATION_INC */

