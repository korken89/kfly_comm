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

#include "KFlyTelemetry/kfly_telemetry.h"

namespace KFlyTelemetry
{
    /*********************************
     * Private members
     ********************************/

    void KFlyTelemetry::executeCallbacks(
        std::shared_ptr<KFlyTelemetryPayload::BasePayloadStruct> payload)
    {
        /* Execute callbacks. */
        std::lock_guard<std::mutex> locker(_id_cblock);

        for (auto &cb : callbacks)
            cb.second(payload);
    }

    void KFlyTelemetry::parseKFlyPacket(const std::vector<uint8_t> &payload)
    {
        /* Extract command. */
        const uint8_t cmd = payload[0];

        /* Extract size and expected size. */
        const unsigned int expected_size = (unsigned int) payload[1];
        const unsigned int length = payload.size();

        /* Extract the CRC and remove it. */
        union {
            uint8_t b[2];
            uint16_t u16;
        } crc;

        crc.b[0] = payload[length - 2];
        crc.b[1] = payload[length - 1];

        /* Remove CRC. */
        std::vector<uint8_t> data(payload.begin(), payload.end() - 2);

        /* Calculate CRC. */
        uint16_t crc_calc = CRC16_CCITT::generateCRC(data);

        /* Remove header. */
        data.erase(data.begin(), data.begin() + 2);

        /* Check sizes and CRC. */
        if (expected_size + 4 != length)
        {
            /* Size error. */
            return;
        }
        else if (crc_calc != crc.u16)
        {
            /* CRC error. */
            return;
        }
        else
        {
            /* Send payload to further processing. */
            auto s = payloadToStruct(cmd, data);
            executeCallbacks(s);
        }
    }

    std::shared_ptr<BasePayloadStruct>
        KFlyTelemetry::payloadToStruct(const uint8_t cmd,
                                       const std::vector<uint8_t> &payload)
    {
        /* Extract the command. */
        const KFly_Command command = static_cast<KFly_Command>( cmd );

        /* Create the return. */
        std::shared_ptr<BasePayloadStruct> ret;

        /* Do appropriate operation for each command. */
        switch (command)
        {
        case KFly_Command::ACK:

            ret = std::make_shared<BasePayloadStruct>();
            break;

        case KFly_Command::GetRunningMode:

            ret = std::make_shared<GetRunningModeStruct>(payload);
            break;

        case KFly_Command::GetDeviceInfo:

            ret = std::make_shared<GetDeviceInfoStruct>(payload);
            break;

        case KFly_Command::GetControllerLimits:

            ret = std::make_shared<ControllerLimitsStruct>(payload);
            break;

        case KFly_Command::GetArmSettings:

            ret = std::make_shared<ArmSettingsStruct>(payload);
            break;

        case KFly_Command::GetRateControllerData:

            ret = std::make_shared<ControllerDataStruct>(payload);
            break;

        case KFly_Command::GetAttitudeControllerData:

            ret = std::make_shared<ControllerDataStruct>(payload);
            break;

        case KFly_Command::GetChannelMix:

            ret = std::make_shared<ChannelMixStruct>(payload);
            break;

        case KFly_Command::GetRCInputSettings:

            ret = std::make_shared<RCInputSettingsStruct>(payload);
            break;

        case KFly_Command::GetRCOutputSettings:

            ret = std::make_shared<RCOutputSettingsStruct>(payload);
            break;

        case KFly_Command::GetRCValues:

            ret = std::make_shared<GetRCValuesStruct>(payload);
            break;

        case KFly_Command::GetIMUData:

            ret = std::make_shared<GetIMUDataStruct>(payload);
            break;

        case KFly_Command::GetRawIMUData:

            ret = std::make_shared<GetRawIMUDataStruct>(payload);
            break;

        case KFly_Command::GetIMUCalibration:

            ret = std::make_shared<IMUCalibrationStruct>(payload);
            break;

        case KFly_Command::GetEstimationAttitude:

            ret = std::make_shared<GetEstimationAttitudeStruct>(payload);
            break;

        case KFly_Command::Experiment:

            ret = std::make_shared<ExperimentStruct>(payload);
            break;

        default:

            ret = std::make_shared<BasePayloadStruct>();
        }

        /* Set the command and return. */
        ret->id = command;

        return ret;
    }



    /*********************************
     * Public members
     ********************************/

    KFlyTelemetry::KFlyTelemetry() : _id(0), _slip_parser()
    {
        /* Register the KFly packet parser to the SLIP output. */
        _slip_parser.registerCallback([&] (const std::vector<uint8_t> &payload)
        {
            this->parseKFlyPacket(payload);
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

    const std::vector<uint8_t>
        KFlyTelemetry::generatePacket(BasePayloadStruct &payload, bool ack)
    {
        std::vector<uint8_t> packet, slip_packet;
        const std::vector<uint8_t> data_payload = payload.toPayload();

        union {
            uint8_t b[2];
            uint16_t u16;
        } crc;

        /* Construct the final packet: | CMD | SIZE | PAYLOAD | CRC | */
        if (ack)
            packet.emplace_back(static_cast<uint8_t>( payload.id ) | 0x80);
        else
            packet.emplace_back(static_cast<uint8_t>( payload.id ));

        packet.emplace_back(static_cast<uint8_t>( data_payload.size() ));

        if (data_payload.size() > 0)
            packet.insert( packet.end(),
                           data_payload.begin(),
                           data_payload.end());

        crc.u16 = CRC16_CCITT::generateCRC( packet );
        packet.insert(packet.end(), &crc.b[0], &crc.b[2]);

        SLIP::SLIP::encode( packet, slip_packet );

        return slip_packet;
    }

    const std::vector<uint8_t> KFlyTelemetry::generatePacket(
            const std::shared_ptr<BasePayloadStruct> &payload, bool ack)
    {
        std::vector<uint8_t> packet, slip_packet;
        const std::vector<uint8_t> data_payload = payload->toPayload();

        union {
            uint8_t b[2];
            uint16_t u16;
        } crc;

        /* Construct the final packet: | CMD | SIZE | PAYLOAD | CRC | */
        if (ack)
            packet.emplace_back(static_cast<uint8_t>( payload->id ) | 0x80);
        else
            packet.emplace_back(static_cast<uint8_t>( payload->id ));

        packet.emplace_back(static_cast<uint8_t>( data_payload.size() ));

        if (data_payload.size() > 0)
            packet.insert( packet.end(),
                           data_payload.begin(),
                           data_payload.end());

        crc.u16 = CRC16_CCITT::generateCRC( packet );
        packet.insert(packet.end(), &crc.b[0], &crc.b[2]);

        SLIP::SLIP::encode( packet, slip_packet );

        return slip_packet;
    }
}
