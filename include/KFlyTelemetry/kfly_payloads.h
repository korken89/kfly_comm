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
#include <string>
#include <vector>
#include <stdexcept>
#include "KFlyTelemetry/kfly_commands.h"

namespace KFlyTelemetryPayload
{

union floatConvert
{
    float f;
    uint8_t b[4];
};

union u32Convert
{
    uint32_t u32;
    uint8_t b[4];
};

union u16Convert
{
    uint16_t u16;
    uint8_t b[2];
};

union i16Convert
{
    int16_t i16;
    uint8_t b[2];
};

static float bytes2float(const uint8_t data[4])
{
    /* Conversion union */
    floatConvert b2f;

    for (int i = 0; i < 4; i++)
        b2f.b[i] = data[i];

    return b2f.f;
}

static void float2bytes(const float f, uint8_t data[4])
{
    /* Conversion union */
    floatConvert f2b;

    f2b.f = f;

    for (int i = 0; i < 4; i++)
        data[i] = f2b.b[i];
}

/* @brief The available flight modes via computer control. */
enum class FlightMode : uint8_t
{
    /* @brief Control each motor individually. */
    MOTOR_DIRECT_MODE,

    /* @brief Control each motor through the mixing matrix. */
    MOTOR_INDIRECT_MODE,

    /* @brief Control via rate references. */
    RATE_MODE,

    /* @brief Control via attitude references. */
    ATTITUDE_MODE
};

/* @brief Port selector, select from the USB or the UART ports.  */
enum class Ports : uint8_t
{
    /* @brief USB port. */
    PORT_USB = 0,

    /* @brief Aux 1 port. */
    PORT_AUX1 = 1,

    /* @brief Aux 2 port. */
    PORT_AUX2 = 2,

    /* @brief Aux 3 port. */
    PORT_AUX3 = 3
};

/* @brief   Input capture channel role selector. */
enum class RCInput_Role : uint8_t
{
    /* @brief Unused role selector. */
    ROLE_OFF = 0,

    /* @brief Throttle role selector. */
    ROLE_THROTTLE = 1,

    /* @brief Pitch role selector. */
    ROLE_PITCH = 2,

    /* @brief Roll role selector. */
    ROLE_ROLL = 3,

    /* @brief Yaw role selector. */
    ROLE_YAW = 4,

    /* @brief Flight mode role selector. */
    ROLE_FLIGHT_MODE = 6,

    /* @brief Aux 1 role selector. */
    ROLE_AUX1 = 7,

    /* @brief Aux 2 role selector. */
    ROLE_AUX2 = 8,

    /* @brief Aux 3 role selector. */
    ROLE_AUX3 = 9
};

/* @brief Input capture channel type. */
enum class RCInput_Type : uint8_t
{
    /* @brief Analog type input. */
    TYPE_ANALOG = 1,

    /* @brief 3-state type input. */
    TYPE_3_STATE = 2,

    /* @brief On/off type input. */
    TYPE_ON_OFF = 3
};

/* @brief Input capture channel type. */
enum class RCInput_Mode : uint32_t
{
    /* @brief Analog type input. */
    MODE_CPPM_INPUT = 1,

    /* @brief 3-state type input. */
    MODE_PWM_INPUT = 2
};

/* @brief Base structure that allows generic usage of functions. */
struct BasePayloadStruct
{
    /* @brief ID for the casting later. */
    KFlyTelemetry::KFly_Command id;
};

/* @brief Running mode (bootloader or firmware). */
struct GetRunningMode : BasePayloadStruct
{
    /* @brief 'B' for bootloader, 'P' for program. */
    char sel;

    GetRunningMode(const std::vector<uint8_t> &payload) : sel(payload[2]) {}
};

/* @brief Manage the subscriptions to messages. */
struct ManageSubscription : BasePayloadStruct
{
    /* @brief Port that the messages will be published on. */
    Ports port;

    /* @brief Command to subscribe to. */
    KFlyTelemetry::KFly_Command cmd;

    /* @brief Milliseconds between publishes, curretly no event option. */
    uint32_t delta_ms;
};

/* @brief Version strings and unique identifiers. */
struct GetDeviceInfo : BasePayloadStruct
{
    /* @brief The MCU's unique ID. */
    uint8_t unique_id[12];

    /* @brief Bootloader version string. */
    std::string bootloader_version;

    /* @brief Firmware version string. */
    std::string firmware_version;

    /* @brief User ID string. */
    std::string user_string;

    GetDeviceInfo(const std::vector<uint8_t> &payload)
    {
        if (payload.size() < 17)
            throw std::invalid_argument( "Payload too small" );
        else
        {
            unsigned int i;

            for (i = 0; i < 12; i++)
                unique_id[i] = payload[i + 2];

            while ((payload[i] != 0) && (payload.size() - 2 < i))
                bootloader_version.append( 1, payload[i++ + 2] );

            i++;

            while ((payload[i] != 0) && (payload.size() - 2 < i))
                firmware_version.append( 1, payload[i++ + 2] );

            i++;

            while ((payload[i] != 0) && (payload.size() - 2 < i))
                user_string.append( 1, payload[i++ + 2] );
        }
    }
};

/* @brief Set the user ID string. */
struct SetDeviceID : BasePayloadStruct
{
    /* @brief Value of the user ID string. */
    std::string user_string;
};

/* @brief All limits in the control system. */
struct ControllerLimits : BasePayloadStruct
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

    ControllerLimits(const std::vector<uint8_t> &payload)
    {
        if (payload.size() != 42)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            /* Skip the header. */
            int i = 2;

            max_rate.roll = bytes2float( &payload[i] );
            i += 4;
            max_rate.pitch = bytes2float( &payload[i] );
            i += 4;
            max_rate.yaw = bytes2float( &payload[i] );
            i += 4;

            max_rate_attitude.roll = bytes2float( &payload[i] );
            i += 4;
            max_rate_attitude.pitch = bytes2float( &payload[i] );
            i += 4;
            max_rate_attitude.yaw = bytes2float( &payload[i] );
            i += 4;

            max_angle.roll = bytes2float( &payload[i] );
            i += 4;
            max_angle.pitch = bytes2float( &payload[i] );
            i += 4;

            max_velocity.horizontal = bytes2float( &payload[i] );
            i += 4;
            max_velocity.vertical = bytes2float( &payload[i] );
        }
    }
};

/* @brief Generic controller data structure. */
struct ControllerData : BasePayloadStruct
{
    /* @brief Roll controller limits and gains. */
    struct
    {
        float P_gain, I_gain, I_limit;
    } roll_controller;

    /* @brief Pitch controller limits and gains. */
    struct
    {
        float P_gain, I_gain, I_limit;
    } pitch_controller;

    /* @brief Yaw controller limits and gains. */
    struct
    {
        float P_gain, I_gain, I_limit;
    } yaw_controller;
};

/* @brief Affine channel mixing matrix. */
struct ChannelMix : BasePayloadStruct
{
    /* @brief Weights to the throttle, pitch, roll, yaw. */
    float weights[8][4];

    /* @brief Offset of the outputs, used for servos. */
    float offset[8];
};

/* @brief Calibration structure for the RC inputs. */
struct RCCalibration : BasePayloadStruct
{
    /* @brief If the receiver is in CPPM or PWM mode. */
    RCInput_Mode mode;

    /* @brief Each channels' input role. */
    RCInput_Role role[12];

    /* @brief Each channels' input type. */
    RCInput_Type type[12];

    /* @brief The top value of the RC input (generally around 2000). */
    uint16_t ch_top[12];

    /* @brief The center value of the RC input (generally around 1500). */
    uint16_t ch_center[12];

    /* @brief The bottom value of the RC input (generally around 1000). */
    uint16_t ch_bottom[12];
};

/* @brief Get the values and status of the RC input. */
struct GetRCValues : BasePayloadStruct
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
};

/* @brief Calibrated sensor data structure. */
struct GetSensorData : BasePayloadStruct
{
    /* @brief Accelerometer data in x, y and z in G. */
    float accelerometer[3];

    /* @brief Gyroscope data in x, y and z in rad/s. */
    float gyroscope[3];

    /* @brief Magnetometer data in x, y and z in normalized units. */
    float magnetometer[3];

    /* @brief The temperature of the IMU in deg/C. */
    float temperature;
};

/* @brief Raw sensor data, used for calibration or logging. */
struct GetRawSensorData : BasePayloadStruct
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
};

/* @brief Sensor calibration structure that takes the internal format and
 *        converts it into the true values. */
struct SensorCalibration : BasePayloadStruct
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
};


struct GetEstimationAllStates : BasePayloadStruct
{
};

/* @brief Computer control data. */
struct ComputerControlReference : BasePayloadStruct
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
};

/* @brief Vicon measurement to KFly data, used for the internal estimation. */
struct ViconMeasurement : BasePayloadStruct
{
    /* @brief Position in meters. */
    float x, y, z;

    /* @brief Attitude in quaternions. */
    float qw, qx, qy, qz;
};

}

#endif
