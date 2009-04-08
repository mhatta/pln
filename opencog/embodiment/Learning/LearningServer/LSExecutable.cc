/*
 * opencog/embodiment/Learning/LearningServer/LSExecutable.cc
 *
 * Copyright (C) 2007-2008 Nil Geisweiller
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <exception>
#include <cstdlib>
#include "util/files.h"
#include "util/Logger.h"
#include "EmbodimentConfig.h"
#include "LS.h"

using namespace LearningServer;
using namespace opencog;

void ls_unexpected_handler()
{
    throw;
}

int main(int argc, char *argv[])
{

    config(Control::EmbodimentConfig::embodimentCreateInstance, true);

    // if exists load file with configuration parameters
    // IMPORTANT: this file should be the same for all executables that create
    // a systemParameter object.

    if (fileExists(config().get("CONFIG_FILE").c_str())) {
        config().load(config().get("CONFIG_FILE").c_str());
    }

    // setting unexpected handler in case a different exception from the
    // especified ones is throw in the code
    std::set_unexpected(ls_unexpected_handler);

    server(LS::derivedCreateInstance);

    static_cast<LS&>(server()).init(config().get("LS_ID"),
                                    config().get("LS_IP"),
                                    config().get_int("LS_PORT"));

    try  {
        static_cast<LS&>(server()).CogServer::serverLoop();

    } catch (std::bad_alloc) {
        logger().log(Logger::ERROR,
                     "LSExec - LS raised a bad_alloc exception.");
    } catch (StandardException se) {
        logger().log(Logger::ERROR,
                     "OPC executable - An exceptional situation occured"
                     " with the following message '%s'"
                     ". Check log for more information.",
                     se.getMessage());
    } catch (...) {
        logger().log(Logger::ERROR,
                     "LSExec - An exceptional situation occured."
                     " Check log for more information.");
    }

    return (0);
}
