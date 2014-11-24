/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Application.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_APPLICATION_INC
#define PIW_APPLICATION_INC

#include    <memory>

#include    <ip_connection.h>

#include    <db/Database.hpp>
#include    <view/Lcd.hpp>

#include    <device/UidRegistry.hpp>
#include    <device/Button.hpp>

#include    "Configuration.hpp"

#include    <thread>

namespace piw {

    class Application
    {
        public:
            Application (const Configuration&);
            Application (const Application&) = delete;
            Application& operator= (const Application&) = delete;
            ~Application ();

            bool run ();

        private:

        private:
            db::Database db;
            device::UidRegistry uidRegistry;
            device::Button button;
            view::Lcd lcd;
            std::thread dbWriter;
    };
}

#endif /* PIW_APPLICATION_INC */

