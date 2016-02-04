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

#ifndef _KFLY_TELEMETRY_H
#define _KFLY_TELEMETRY_H

/* Data includes */
#include <vector>
#include <cstdint>
#include <map>
#include <memory>

/* Threading includes */
#include <mutex>

/* Library includes */
#include "cppSLIP/slip.h"

/* KFly includes */
#include "KFlyTelemetry/kfly_commands.h"
#include "KFlyTelemetry/kfly_payloads.h"
#include "KFlyTelemetry/crc.h"

using namespace KFlyTelemetryPayload;

namespace KFlyTelemetry
{

/** @brief Definition of the message callback. */
typedef std::function<void(
const std::shared_ptr<KFlyTelemetryPayload::BasePayloadStruct>)> kfly_callback;

class KFlyTelemetry
{
private:

    /** @brief Mutex for the ID counter and the callback list. */
    std::mutex _id_cblock;

    /** @brief Vector holding the registered callbacks. */
    std::map<unsigned int, kfly_callback> callbacks;

    /** @brief ID counter for the removal of subscriptions. */
    unsigned int _id;

    /** @brief SLIP parser for the system. */
    SLIP::SLIP _slip_parser;

    /** @brief Lock for the SLIP parser. */
    std::mutex _slip_lock;

    /**
     * @brief   Calls all the registered callbacks with the data payload.
     *
     * @param[in] payload   The payload to be sent.
     */
    void executeCallbacks(std::shared_ptr<BasePayloadStruct> payload);

    /**
     * @brief   Parses a payload and, if correct, runs executeCallbacks.
     *
     * @param[in] payload   The payload to be parsed.
     */
    void parseKFlyPacket(const std::vector<uint8_t> &payload);

    /**
     * @brief   Checks the command to apply the proper structure.
     *
     * @param[in] cmd       Command byte from the packet.
     * @param[in] payload   The payload to be parsed, without header or CRC.
     *
     * @return A BasePayloadStruct that holds the parsed message.
     */
    std::shared_ptr<BasePayloadStruct>
        payloadToStruct(const uint8_t cmd,
                        const std::vector<uint8_t> &payload);


public:
    KFlyTelemetry();

    ~KFlyTelemetry();

    /**
     * @brief   Register a callback.
     *
     * @param[in] callback  The function to register.
     * @note    Shall be of the form void(const std::vector<uint8_t> &).
     *
     * @return  Return the ID of the callback, is used for unregistration.
     */
    unsigned int registerCallback(kfly_callback callback);

    /**
     * @brief   Unregister a callback from the queue.
     *
     * @param[in] id    The ID supplied from @p registerCallback.
     *
     * @return  Return true if the ID was deleted.
     */
    bool unregisterCallback(const unsigned int id);

    /**
     * @brief   Input function for a KFly message, goes to the SLIP parser.
     *
     * @param[in] payload   The payload to be parsed.
     */
    void parse(const uint8_t data);

    /**
     * @brief   Input function for a KFly message, goes to the SLIP parser.
     *
     * @param[in] payload   The payload to be parsed.
     */
    void parse(const std::vector<uint8_t> &payload);

    /**
     * @brief   Converts a BasePayloadStruct to a byte message for transmission.
     *
     * @param[in] payload   The BasePayloadStruct payload to be converted.
     * @param[in] ack       If true, then an ack is requested.
     *
     * @return A vector that holds the generated message.
     */
    static const std::vector<uint8_t> generatePacket(BasePayloadStruct &payload,
                                                     bool ack);

    /**
     * @brief   Converts a BasePayloadStruct to a byte message for transmission.
     *
     * @param[in] payload   The BasePayloadStruct payload to be converted
     *                      passed by shared pointer..
     *
     * @return A vector that holds the generated message.
     */
    static const std::vector<uint8_t> generatePacket(
            const std::shared_ptr<BasePayloadStruct> &payload,
            bool ack);
};

} // namespace KFlyTelemetry

#endif
