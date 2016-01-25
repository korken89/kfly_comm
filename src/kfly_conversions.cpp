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
    std::shared_ptr<KFlyTelemetryPayload::BasePayloadStruct>
        KFlyTelemetry::payloadToStruct(const std::vector<uint8_t> &payload)
    {
        /* Extract the command. */
        KFly_Command cmd = static_cast<KFly_Command>( payload[0] );

        /* Create the return. */
        std::shared_ptr<KFlyTelemetryPayload::BasePayloadStruct> ret;

        /* Do appropriate operation for each command. */
        switch (cmd)
        {
        case KFly_Command::ACK:

            ret = std::make_shared<KFlyTelemetryPayload::BasePayloadStruct>();
            break;

        case KFly_Command::GetRunningMode:

            ret = std::make_shared<KFlyTelemetryPayload::GetRunningModeStruct>(
                payload
            );
            break;

        case KFly_Command::GetDeviceInfo:

            ret = std::make_shared<KFlyTelemetryPayload::GetDeviceInfoStruct>(
                payload
            );
            break;

        case KFly_Command::GetControllerLimits:

            ret = std::make_shared<KFlyTelemetryPayload::ControllerLimitsStruct>(
                payload
            );
            break;

        case KFly_Command::GetArmSettings:

            ret = std::make_shared<KFlyTelemetryPayload::ArmSettingsStruct>(
                payload
            );
            break;

        case KFly_Command::GetRateControllerData:

            ret = std::make_shared<KFlyTelemetryPayload::ControllerDataStruct>(
                payload
            );
            break;

        case KFly_Command::GetAttitudeControllerData:

            ret = std::make_shared<KFlyTelemetryPayload::ControllerDataStruct>(
                payload
            );
            break;

        case KFly_Command::GetChannelMix:

            ret = std::make_shared<KFlyTelemetryPayload::ChannelMixStruct>(
                payload
            );
            break;

        case KFly_Command::GetRCCalibration:

            ret = std::make_shared<KFlyTelemetryPayload::RCCalibrationStruct>(
                payload
            );
            break;

        case KFly_Command::GetRCValues:

            ret = std::make_shared<KFlyTelemetryPayload::GetRCValuesStruct>(
                payload
            );
            break;

        case KFly_Command::GetSensorData:

            ret = std::make_shared<KFlyTelemetryPayload::GetSensorDataStruct>(
                payload
            );
            break;

        case KFly_Command::GetRawSensorData:

            ret = std::make_shared<KFlyTelemetryPayload::GetRawSensorDataStruct>(
                payload
            );
            break;

        case KFly_Command::GetSensorCalibration:

            ret = std::make_shared<KFlyTelemetryPayload::SensorCalibrationStruct>(
                payload
            );
            break;

        default:

            ret = std::make_shared<KFlyTelemetryPayload::BasePayloadStruct>();
            ret->id = KFly_Command::None;

            return ret;

        }

        /* Set the command and return. */
        ret->id = cmd;

        return ret;
    }
};
