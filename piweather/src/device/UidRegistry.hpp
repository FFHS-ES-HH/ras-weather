/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * uidregistry.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_DEVICE_UIDREGISTRY_INC
#define PIW_DEVICE_UIDREGISTRY_INC

#include    <map>
#include    <cstdint>
#include    <memory>

#include    <ip_connection.h>

namespace piw { namespace device {

    struct EnumerationState;

    class UidRegistry
    {
        public:
            UidRegistry (IPConnection*);

            const std::string& getUid (std::uint16_t) const;

        private:
            std::map<std::uint16_t, std::string> uids_;
            std::unique_ptr<EnumerationState> state_;
    };
}}

#endif /* PIW_DEVICE_UIDREGISTRY_INC */

