/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * uidregistry.cpp is free software copyrighted by David Daniel.
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
#include    "UidRegistry.hpp"

#include    <atomic>

namespace piw { namespace device {

    typedef std::map<std::uint16_t, std::string> UidMap;

    namespace {
        const size_t NrOfBricklets = 4;
    }

    struct EnumerationState
    {
        EnumerationState (UidMap&);

        UidMap& map;
        std::atomic_size_t counter;
    };

    EnumerationState::EnumerationState (UidMap& m) :
        map (m),
        counter (NrOfBricklets)
    {}

    namespace {

        void walk_bricklet (
                const char* uid,
                const char* connected_uid,
                char position,
                uint8_t hardware_version[3],
                uint8_t firmware_version[3],
                uint16_t device_identifier,
                uint8_t enumeration_type,
                void* user_data) 
        {
            EnumerationState* state = static_cast<EnumerationState*> (user_data);
        }

    }

    UidRegistry::UidRegistry (IPConnection* connection) :
        uids_ (),
        state_ (new EnumerationState (uids_))
    {
        ipcon_register_callback (
                connection,
                IPCON_CALLBACK_ENUMERATE,
                reinterpret_cast<void*> (&walk_bricklet),
                state_.get ());

        ipcon_enumerate (connection);
    }
}}
