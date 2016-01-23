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

    void KFlyTelemetry::executeCallbacks(KFly_Command cmd,
                                         const std::vector<uint8_t> &payload)
    {
        /* Execute callbacks. */
        std::lock_guard<std::mutex> locker(_id_cblock);

        for (auto &cb : callbacks)
            cb.second(cmd, payload);
    }

    void KFlyTelemetry::ParseKFlyPacket(const std::vector<uint8_t> &payload)
    {
        unsigned int expected_size = (unsigned int) payload[0];
        unsigned int length = payload.size();
        uint16_t crc = payload[length - 2] | (payload[length - 1] >> 8);

        std::vector<uint8_t> data(payload);
        data.pop_back();
        data.pop_back();

        if (expected_size + 2 != length)
        {
            /* Size error. */
            return;
        }
        else if (CRC16_CCITT::generateCRC(data) != crc)
        {
            /* CRC error. */
            return;
        }
        else
        {

        }
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

    KFlyTelemetry::KFlyTelemetry() : _id(0), _slip_parser()
    {
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
        std::lock_guard<std::mutex> locker(_slip_lock);

        _slip_parser.parse(data);
    }

    void KFlyTelemetry::parse(const std::vector<uint8_t> &payload)
    {
        std::lock_guard<std::mutex> locker(_slip_lock);

        _slip_parser.parse(payload);
    }
}
