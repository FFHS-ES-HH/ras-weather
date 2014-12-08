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
#include    <set>

#include    <ip_connection.h>

namespace piw { namespace device {

    struct EnumerationState;

    class UidRegistry
    {
        friend struct EnumerationState;

        public:
            UidRegistry (IPConnection*);
            ~UidRegistry ();

            UidRegistry (const UidRegistry&) = delete;
            UidRegistry& operator= (const UidRegistry&) = delete;

            static void registerDevice (std::uint16_t);

            const std::string& getUid (std::uint16_t) const;

        private:
            std::map<std::uint16_t, std::string> uids_;
            EnumerationState* state_;

        private:
            static std::set<std::uint16_t>& getDeviceIds ();
    };

    inline void UidRegistry::registerDevice (std::uint16_t id)
    { getDeviceIds ().insert (id); }

    struct DeviceRegistration
    {
        DeviceRegistration (std::uint16_t);
    };

    inline DeviceRegistration::DeviceRegistration (std::uint16_t id)
    { UidRegistry::registerDevice (id); }
}}

#endif /* PIW_DEVICE_UIDREGISTRY_INC */

