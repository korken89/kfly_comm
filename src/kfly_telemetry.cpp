/****************************************************************************
*
* Copyright (C) 2016 Emil Fresk.
* All rights reserved.
*
* This file is part of the KFly Telemetry library.
*
* GNU Lesser General Public License Usage
* This file may be used under the terms of the GNU Lesser
* General Public License version 3.0 as published by the Free Software
* Foundation and appearing in the file LICENSE included in the
* packaging of this file.  Please review the following information to
* ensure the GNU Lesser General Public License version 3.0 requirements
* will be met: http://www.gnu.org/licenses/lgpl-3.0.html.
*
* If you have questions regarding the use of this file, please contact
* Emil Fresk at emil.fresk@gmail.com.
*
****************************************************************************/

#include <iostream>
#include "KFlyTelemetry/kfly_telemetry.h"

namespace KFlyTelemetry
{
    /*********************************
     * Private members
     ********************************/

    void KFlyTelemetry::executeCallbacks(const std::vector<uint8_t> &payload)
    {
        /* Execute callbacks. */
        std::lock_guard<std::mutex> locker(_id_cblock);

        for (auto &cb : callbacks)
            cb.second(payload);
    }

    void KFlyTelemetry::ParseKFlyPacket(const std::vector<uint8_t> &payload)
    {
        (void) payload;
    }

    void KFlyTelemetry::generatePacket(const KFly_Command cmd,
            const std::vector<uint8_t> &payload)
    {
        (void) cmd;
        (void) payload;
    }
    /*********************************
     * Public members
     ********************************/

    KFlyTelemetry::KFlyTelemetry() : _slip_parser()
    {
        /* Initialize the ID counter. */
        _id = 0;

        /* Register the KFly packet parser to the SLIP output. */
        _slip_parser.registerCallback([&] (const std::vector<uint8_t> &payload)
        {
            this->ParseKFlyPacket(payload);
        });
    }

    KFlyTelemetry::~KFlyTelemetry()
    {
    }

    unsigned int KFlyTelemetry::registerCallback(kfly_callback callback)
    {
        std::lock_guard<std::mutex> locker(_id_cblock);

        /* Add the callback to the list. */
        callbacks.emplace(_id, callback);

        return _id++;
    }

    bool KFlyTelemetry::unregisterCallback(const unsigned int id)
    {
        std::lock_guard<std::mutex> locker(_id_cblock);

        /* Delete the callback with correct ID. */
        if (callbacks.erase(id) > 0)
            return true;
        else
            /* No match, return false. */
            return false;
    }

    void KFlyTelemetry::parse(const uint8_t data)
    {
        (void) data;
    }

    void KFlyTelemetry::parse(const std::vector<uint8_t> &payload)
    {
        for (auto &data : payload)
            parse(data);
    }
}
