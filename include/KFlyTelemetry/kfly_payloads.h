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

#ifndef _KFLY_PAYLOADS_H
#define _KFLY_PAYLOADS_H

#include <cstdint>
#include <ctime>
#include <string>
#include <cstring>
#include <vector>
#include <stdexcept>
#include "KFlyTelemetry/kfly_commands.h"
#include "KFlyTelemetry/kfly_enums.h"

namespace KFlyTelemetryPayload
{

template<class T>
static std::vector<uint8_t> serialize(const T *data)
{
    auto ptr = reinterpret_cast<const uint8_t *>(data);
    auto buffer = std::vector<uint8_t>{ptr, ptr + sizeof(T)};

    return buffer;
}

template<class T>
static std::vector<uint8_t> serialize(const T *data, size_t size)
{
    auto ptr = reinterpret_cast<const uint8_t *>(data);
    auto buffer = std::vector<uint8_t>{ptr, ptr + size};

    return buffer;
}

template<class T>
static void deserialize(T *data, const std::vector<uint8_t> &datagram)
{
    if (sizeof(T) == datagram.size())
        std::memcpy(reinterpret_cast<uint8_t *>(data),
                    datagram.data(),
                    sizeof(T));
    else
        throw std::invalid_argument( "Payload too small" );
}


/*****************************************************************************/
/* NOTE!!! All structures bellow must be inside the #pragmas !!!             */
/*****************************************************************************/

#pragma pack(push,1)


/* @brief Base structure that allows generic usage of functions. */
struct BasePayloadStruct
{
    /* @brief ID for the casting later. */
    KFlyTelemetry::KFly_Command id;

    BasePayloadStruct()
    {
    }

    /**
     * @brief  Default payload generation.
     *
     * @return The vector containing the empty byte string.
     */
    virtual const std::vector<uint8_t> toPayload()
    {
        std::vector<uint8_t> v;
        return v;
    }
};

/* @brief Running mode (bootloader or firmware). */
struct GetRunningModeStruct : BasePayloadStruct
{
    /* @brief 'B' for bootloader, 'P' for program. */
    char sel;

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    GetRunningModeStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<GetRunningModeStruct>(this, payload);
    }
};

/* @brief Manage the subscriptions to messages. */
struct ManageSubscriptionStruct : BasePayloadStruct
{
    /* @brief Port that the messages will be published on. */
    Ports port;

    /* @brief Command to subscribe to. */
    KFlyTelemetry::KFly_Command cmd;

    /* @brief Selector for subscribe or unsubscribe. */
    bool subscribe;

    /* @brief Milliseconds between publishes, curretly no event option. */
    uint32_t delta_ms;

    ManageSubscriptionStruct(Ports port, KFlyTelemetry::KFly_Command cmd,
                       bool subscribe, uint32_t delta_ms) :
        port(port), cmd(cmd), subscribe(subscribe), delta_ms(delta_ms)
    {
        id = KFlyTelemetry::KFly_Command::ManageSubscriptions;
    }

    /**
     * @brief Serializes the struct.
     * @return  The serialized struct.
     */
    const std::vector<uint8_t> toPayload()
    {
        return serialize<ManageSubscriptionStruct>(this);
    }
};

/* @brief Version strings and unique identifiers. */
struct GetDeviceInfoStruct : BasePayloadStruct
{
    /* @brief The MCU's unique ID. */
    uint8_t unique_id[12];

    /* @brief Bootloader version string. */
    std::string bootloader_version;

    /* @brief Firmware version string. */
    std::string firmware_version;

    /* @brief User ID string. */
    std::string user_string;

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    GetDeviceInfoStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<GetDeviceInfoStruct>(this, payload);
    }
};

/* @brief Set the user ID string. */
struct SetDeviceIDStruct : BasePayloadStruct
{
    /* @brief Value of the user ID string. */
    std::string user_string;

    SetDeviceIDStruct(std::string user_string) : user_string(user_string)
    {
        id = KFlyTelemetry::KFly_Command::SetDeviceID;
    }

    /**
     * @brief Serializes the struct.
     * @return  The serialized struct.
     */
    const std::vector<uint8_t> toPayload()
    {
        return serialize<SetDeviceIDStruct>(this);
    }
};

/* @brief All limits in the control system. */
struct ControllerLimitsStruct : BasePayloadStruct
{
    /* @brief Limits in radians/s, when in rate mode. */
    struct
    {
        float roll, pitch, yaw;
    } max_rate;

    /* @brief Limits in radians/s, when in attitude mode. */
    struct
    {
        float roll, pitch, yaw;
    } max_rate_attitude;

    /* @brief Limits in radians, when in attitude mode or higher. */
    struct
    {
        float roll, pitch;
    } max_angle;

    /* @brief Limits of velocity. */
    struct
    {
        float horizontal, vertical;
    } max_velocity;

    ControllerLimitsStruct()
    {
        id = KFlyTelemetry::KFly_Command::SetControllerLimits;
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly excluding header.
     */
    ControllerLimitsStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<ControllerLimitsStruct>(this, payload);
    }

    /**
     * @brief Serializes the struct.
     * @return  The serialized struct.
     */
    const std::vector<uint8_t> toPayload()
    {
        return serialize<ControllerLimitsStruct>(this);
    }
};

/* @brief Arming settings. */
struct ArmSettingsStruct : BasePayloadStruct
{
     /* @brief   Stick threshold for the arm/disarm logic to react. */
    float stick_threshold;

    /* @brief   Minimum throttle when armed (to spin propellers when armed). */
    float armed_min_throttle;

    /* @brief   Stick direction to arm the controllers. */
    Arming_Stick_Direction stick_direction;

    /* @brief   Time (in seconds) needed to hold the sticks in correct
     *          position in order to arm the system. */
    uint8_t arm_stick_time;

    /* @brief   Time (in seconds) needed to disarm the controllers if no
     *          throttle has been given. */
    uint8_t arm_zero_throttle_timeout;

    ArmSettingsStruct()
    {
        id = KFlyTelemetry::KFly_Command::SetArmSettings;
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    ArmSettingsStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<ArmSettingsStruct>(this, payload);
    }

    /**
     * @brief Serializes the struct.
     * @return  The serialized struct.
     */
    const std::vector<uint8_t> toPayload()
    {
        return serialize<ArmSettingsStruct>(this);
    }
};

/* @brief Generic controller data structure. */
struct ControllerDataStruct : BasePayloadStruct
{
    /* @brief Roll controller limits and gains. */
    struct
    {
        float P_gain, I_gain;
    } roll_controller;

    /* @brief Pitch controller limits and gains. */
    struct
    {
        float P_gain, I_gain;
    } pitch_controller;

    /* @brief Yaw controller limits and gains. */
    struct
    {
        float P_gain, I_gain;
    } yaw_controller;

    ControllerDataStruct(KFlyTelemetry::KFly_Command cmd)
    {
        if ((cmd == KFlyTelemetry::KFly_Command::SetRateControllerData) ||
            (cmd == KFlyTelemetry::KFly_Command::SetAttitudeControllerData))
        {
            id = cmd;
        }
        else
            throw std::invalid_argument( "Not a valid command" );
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    ControllerDataStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<ControllerDataStruct>(this, payload);
    }

    /**
     * @brief Serializes the struct.
     * @return  The serialized struct.
     */
    const std::vector<uint8_t> toPayload()
    {
        return serialize<ControllerDataStruct>(this);
    }
};

/* @brief Affine channel mixing matrix. */
struct ChannelMixStruct : BasePayloadStruct
{
    /* @brief Weights to the throttle, pitch, roll, yaw. */
    float weights[8][4];

    /* @brief Offset of the outputs, used for servos. */
    float offset[8];

    ChannelMixStruct()
    {
        id = KFlyTelemetry::KFly_Command::SetChannelMix;
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    ChannelMixStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<ChannelMixStruct>(this, payload);
    }

    /**
     * @brief Serializes the struct.
     * @return  The serialized struct.
     */
    const std::vector<uint8_t> toPayload()
    {
        return serialize<ChannelMixStruct>(this);
    }
};

/* @brief Calibration structure for the RC inputs. */
struct RCCalibrationStruct : BasePayloadStruct
{
    /* @brief If the receiver is in CPPM or PWM mode. */
    RCInput_Mode mode;

    /* @brief Each channels' input role. */
    RCInput_Role role[12];

    /* @brief Each channels' input type. */
    RCInput_Type type[12];

    /* @brief Flag to reverse a channel. */
    bool ch_reverse[12];

    /* @brief The top value of the RC input (generally around 2000). */
    uint16_t ch_top[12];

    /* @brief The center value of the RC input (generally around 1500). */
    uint16_t ch_center[12];

    /* @brief The bottom value of the RC input (generally around 1000). */
    uint16_t ch_bottom[12];

    RCCalibrationStruct()
    {
        id = KFlyTelemetry::KFly_Command::SetRCCalibration;
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    RCCalibrationStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<RCCalibrationStruct>(this, payload);
    }
};

/* @brief Get the values and status of the RC input. */
struct GetRCValuesStruct : BasePayloadStruct
{
    /* @brief Active connection indicator. */
    bool active_connection;

    /* @brief Number of active inputs (all 12 might not be used). */
    uint16_t num_connections;

    /* @brief The input value of each channel. */
    uint16_t channel_value[12];

    /* @brief The signal strength in percent. */
    uint16_t rssi;

    /* @brief The frequency of the RSSI PWM. */
    uint16_t rssi_frequency;

    GetRCValuesStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<GetRCValuesStruct>(this, payload);
    }
};

/* @brief Calibrated sensor data structure. */
struct GetIMUDataStruct : BasePayloadStruct
{
    /* @brief Accelerometer data in x, y and z in G. */
    float accelerometer[3];

    /* @brief Gyroscope data in x, y and z in rad/s. */
    float gyroscope[3];

    /* @brief Magnetometer data in x, y and z in normalized units. */
    float magnetometer[3];

    /* @brief The temperature of the IMU in deg/C. */
    float temperature;

    GetIMUDataStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<GetIMUDataStruct>(this, payload);
    }
};

/* @brief Raw sensor data, used for calibration or logging. */
struct GetRawIMUDataStruct : BasePayloadStruct
{
    /* @brief Accelerometer data in x, y and z in the interal format. */
    int16_t accelerometer[3];

    /* @brief Gyroscope data in x, y and z in the interal format. */
    int16_t gyroscope[3];

    /* @brief Magnetometer data in x, y and z in the interal format. */
    int16_t magnetometer[3];

    /* @brief Temperature of the IMU interal format. */
    int16_t temperature;

    /* @brief Pressure in the interal format. */
    uint32_t pressure;

    GetRawIMUDataStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<GetRawIMUDataStruct>(this, payload);
    }
};

/* @brief IMU calibration structure that takes the internal format and
 *        converts it into the true values. */
struct IMUCalibrationStruct : BasePayloadStruct
{
    /* @brief Accelerometer bias in x, y and z. */
    float accelerometer_bias[3];

    /* @brief Accelerometer gain in x, y and z. */
    float accelerometer_gain[3];

    /* @brief Magnetometer bias in x, y and z. */
    float magnetometer_bias[3];

    /* @brief Magnetometer gain in x, y and z. */
    float magnetometer_gain[3];

    /* @brief UNIX timestamp in seconds from 1970. */
    uint32_t timestamp;

    IMUCalibrationStruct()
    {
        id = KFlyTelemetry::KFly_Command::SetIMUCalibration;
        timestamp = static_cast<uint32_t>( std::time(0) );
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    IMUCalibrationStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<IMUCalibrationStruct>(this, payload);
    }

    /**
     * @brief Serializes the struct.
     * @return  The serialized struct.
     */
    const std::vector<uint8_t> toPayload()
    {
        return serialize<IMUCalibrationStruct>(this);
    }
};

/* @brief Estimation Attitude structure, holds the attitude estimation states. */
struct GetEstimationAttitudeStruct : BasePayloadStruct
{
    /* @brief Quaternion containing attitude. */
    float qw, qx, qy, qz;

    /* @brief Angular rates in [rad/s]. */
    float angular_rate[3];

    /* @brief Angular rate biases in [rad/s]. */
    float rate_bias[3];

    GetEstimationAttitudeStruct()
    {
        id = KFlyTelemetry::KFly_Command::GetEstimationAttitude;
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    GetEstimationAttitudeStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<GetEstimationAttitudeStruct>(this, payload);
    }
};

struct GetEstimationAllStatesStruct : BasePayloadStruct
{
};

/* @brief Experiment structure. */
struct ExperimentStruct : BasePayloadStruct
{
    float acc_z;
    float angular_rate[3];
    float torque_ref[3];
    float throttle_ref;
    uint16_t cnt;

    ExperimentStruct()
    {
        id = KFlyTelemetry::KFly_Command::Experiment;
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    ExperimentStruct(const std::vector<uint8_t> &payload)
    {
        deserialize<ExperimentStruct>(this, payload);
    }
};

/* @brief Computer control data. */
struct ComputerControlReferenceStruct : BasePayloadStruct
{
    /* @brief Flight mode to differentiate the following data. */
    FlightMode mode;

    union
    {
        /* @brief Direct motor control data. */
        uint16_t direct_control[8];

        /* @brief Indirect motor control data. */
        struct
        {
            float roll, pitch, yaw;
            float throttle;
        } indirect_control;

        /* @brief Rate control data. */
        struct
        {
            float roll, pitch, yaw;
            float throttle;
        } rate;

        /* @brief Attitude control data. */
        struct
        {
            float w, x, y, z;
            float throttle;
        } attitude;
    };

    ComputerControlReferenceStruct()
    {
        id = KFlyTelemetry::KFly_Command::ComputerControlReference;
    }

    /**
     * @brief Serializes the struct.
     * @return  The serialized struct.
     */
    const std::vector<uint8_t> toPayload()
    {
        if (mode == FlightMode::ATTITUDE_MODE)
            return serialize<ComputerControlReferenceStruct>(this);
        else
            return serialize<ComputerControlReferenceStruct>(this, 16);
    }
};

/* @brief Motion capture frame to KFly data, used for the internal estimation. */
struct MotionCaptureFrameStruct : BasePayloadStruct
{
    /* @brief Frame number. */
    uint32_t framenumber;

    /* @brief Position in meters. */
    float x, y, z;

    /* @brief Attitude in quaternions. */
    float qw, qx, qy, qz;

    MotionCaptureFrameStruct()
    {
        id = KFlyTelemetry::KFly_Command::MotionCaptureMeasurement;
    }

    /**
     * @brief Serializes the struct.
     * @return  The serialized struct.
     */
    const std::vector<uint8_t> toPayload()
    {
        return serialize<MotionCaptureFrameStruct>(this);
    }
};


/*****************************************************************************/
/* NOTE!!! All structures bellow must be inside the #pragmas !!!             */
/*****************************************************************************/

#pragma pack(pop)

} // namespace KFlyTelemetryPayload

#endif
