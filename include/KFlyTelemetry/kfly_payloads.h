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
#include <vector>
#include <stdexcept>
#include "KFlyTelemetry/kfly_commands.h"

namespace KFlyTelemetryPayload
{

/* @brief   Union for converting between float and bytes. */
union floatConvert
{
    float f;
    uint8_t b[4];
};

/* @brief   Union for converting between uint32_t and bytes. */
union u32Convert
{
    uint32_t u32;
    uint8_t b[4];
};

/* @brief   Union for converting between int32_t and bytes. */
union i32Convert
{
    uint32_t i32;
    uint8_t b[4];
};

/* @brief   Union for converting between uint16_t and bytes. */
union u16Convert
{
    uint16_t u16;
    uint8_t b[2];
};

/* @brief   Union for converting between int16_t and bytes. */
union i16Convert
{
    int16_t i16;
    uint8_t b[2];
};

/**
 * @brief Converts 4 bytes to a float.
 *
 * @param[in] data  The array of bytes.
 *
 * @return The converted float.
 */
static float bytes2float(const uint8_t data[4])
{
    /* Conversion union */
    floatConvert b2f;

    for (int i = 0; i < 4; i++)
        b2f.b[i] = data[i];

    return b2f.f;
}

/**
 * @brief Converts a float to 4 bytes.
 *
 * @param[in]  f    The float to be converted.
 * @param[out] data The array of converted bytes.
 */
static void float2bytes(const float f, uint8_t data[4])
{
    /* Conversion union */
    floatConvert f2b;

    f2b.f = f;

    for (int i = 0; i < 4; i++)
        data[i] = f2b.b[i];
}

/**
 * @brief Converts 4 bytes to a uint32_t.
 *
 * @param[in] data  The array of bytes to be converted.
 *
 * @return The converted uint32_t.
 */
static uint32_t bytes2u32(const uint8_t data[4])
{
    /* Conversion union */
    u32Convert c;

    for (int i = 0; i < 4; i++)
        c.b[i] = data[i];

    return c.u32;
}

/**
 * @brief Converts a uint32_t to 4 bytes.
 *
 * @param[in]  u32  The uint32_t to be converted.
 * @param[out] data The array of converted bytes.
 */
static void u322bytes(const uint32_t u32, uint8_t data[4])
{
    /* Conversion union */
    u32Convert c;

    c.u32 = u32;

    for (int i = 0; i < 4; i++)
        data[i] = c.b[i];
}

/**
 * @brief Converts 4 bytes to a int32_t.
 *
 * @param[in] data  The array of bytes to be converted.
 *
 * @return The converted int32_t.
 */
static int32_t bytes2i32(const uint8_t data[4])
{
    /* Conversion union */
    i32Convert c;

    for (int i = 0; i < 4; i++)
        c.b[i] = data[i];

    return c.i32;
}

/**
 * @brief Converts a int32_t to 4 bytes.
 *
 * @param[in]  i32  The int32_t to be converted.
 * @param[out] data The array of converted bytes.
 */
static void i322bytes(const int32_t i32, uint8_t data[4])
{
    /* Conversion union */
    i32Convert c;

    c.i32 = i32;

    for (int i = 0; i < 4; i++)
        data[i] = c.b[i];
}

/**
 * @brief Converts 2 bytes to a uint16_t.
 *
 * @param[in] data  The array of bytes to be converted.
 *
 * @return The converted uint16_t.
 */
static uint16_t bytes2u16(const uint8_t data[2])
{
    /* Conversion union */
    u16Convert c;

    for (int i = 0; i < 2; i++)
        c.b[i] = data[i];

    return c.u16;
}

/**
 * @brief Converts a uint16_t to 2 bytes.
 *
 * @param[in]  u16  The uint16_t to be converted.
 * @param[out] data The array of converted bytes.
 */
static void u162bytes(const uint16_t u16, uint8_t data[2])
{
    /* Conversion union */
    u16Convert c;

    c.u16 = u16;

    for (int i = 0; i < 2; i++)
        data[i] = c.b[i];
}

/**
 * @brief Converts 2 bytes to a int16_t.
 *
 * @param[in] data  The array of bytes to be converted.
 *
 * @return The converted int16_t.
 */
static int16_t bytes2i16(const uint8_t data[2])
{
    /* Conversion union */
    i16Convert c;

    for (int i = 0; i < 2; i++)
        c.b[i] = data[i];

    return c.i16;
}

/**
 * @brief Converts a int16_t to 2 bytes.
 *
 * @param[in]  i16  The int16_t to be converted.
 * @param[out] data The array of converted bytes.
 */
static void i162bytes(const int16_t i16, uint8_t data[2])
{
    /* Conversion union */
    i16Convert c;

    c.i16 = i16;

    for (int i = 0; i < 2; i++)
        data[i] = c.b[i];
}

/* @brief The available flight modes via computer control. */
enum class FlightMode : uint8_t
{
    /* @brief Control each motor individually. */
    MOTOR_DIRECT_MODE = 1,

    /* @brief Control each motor through the mixing matrix. */
    MOTOR_INDIRECT_MODE = 2,

    /* @brief Control via rate references. */
    RATE_MODE = 3,

    /* @brief Control via attitude references. */
    ATTITUDE_MODE = 4
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

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    GetRunningMode(const std::vector<uint8_t> &payload)
    {
        if (payload.size() < 3)
            throw std::invalid_argument( "Payload too small" );
        else
        {
            sel = payload[3];
        }
    }
};

/* @brief Manage the subscriptions to messages. */
struct ManageSubscription : BasePayloadStruct
{
    /* @brief Port that the messages will be published on. */
    Ports port;

    /* @brief Command to subscribe to. */
    KFlyTelemetry::KFly_Command cmd;

    /* @brief Selector for subscribe or unsubscribe. */
    bool subscribe;

    /* @brief Milliseconds between publishes, curretly no event option. */
    uint32_t delta_ms;

    ManageSubscription(Ports port, KFlyTelemetry::KFly_Command cmd,
                       bool subscribe, uint32_t delta_ms) :
        port(port), cmd(cmd), subscribe(subscribe), delta_ms(delta_ms)
    {
        id = KFlyTelemetry::KFly_Command::ManageSubscriptions;
    }

    /**
     * @brief  Converts the structure to a byte string that KFly can parse.
     *
     * @return The vector containing the byte string.
     */
    const std::vector<uint8_t> toPayload(void)
    {
        /* Scratchpad for the byte converter. */
        uint8_t scratch[4];

        /* Create the vector and allocate the area needed. */
        std::vector<uint8_t> payload;
        payload.reserve(7);

        /* Add the payload. */
        payload.emplace_back( static_cast<uint8_t>( port ) );
        payload.emplace_back( static_cast<uint8_t>( cmd ) );

        if (subscribe)
            payload.emplace_back( 1 );
        else
            payload.emplace_back( 0 );

        u322bytes(delta_ms, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        return payload;
    }
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

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
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

    SetDeviceID(std::string user_string) : user_string(user_string)
    {
        id = KFlyTelemetry::KFly_Command::SetDeviceID;
    }

    /**
     * @brief  Converts the structure to a byte string that KFly can parse.
     *
     * @return The vector containing the byte string.
     */
    const std::vector<uint8_t> toPayload(void)
    {
        /* Create the vector and allocate the area needed. */
        std::vector<uint8_t> payload;
        payload.reserve(user_string.size());

        /* Add the payload. */
        for (char c : user_string)
            payload.emplace_back( static_cast<uint8_t>( c ) );

        return payload;
    }
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

    ControllerLimits()
    {
        id = KFlyTelemetry::KFly_Command::SetControllerLimits;
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
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

    /**
     * @brief  Converts the structure to a byte string that KFly can parse.
     *
     * @return The vector containing the byte string.
     */
    const std::vector<uint8_t> toPayload(void)
    {
        /* Scratchpad for the byte converter. */
        uint8_t scratch[4];

        /* Create the vector and allocate the area needed. */
        std::vector<uint8_t> payload;
        payload.reserve(40);

        /* Add the payload. */
        float2bytes(max_rate.roll, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(max_rate.pitch, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(max_rate.yaw, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(max_rate_attitude.roll, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(max_rate_attitude.pitch, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(max_rate_attitude.yaw, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(max_angle.roll, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(max_angle.pitch, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(max_velocity.horizontal, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(max_velocity.vertical, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        return payload;
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

    ControllerData(KFlyTelemetry::KFly_Command cmd)
    {
        if ((id == KFlyTelemetry::KFly_Command::SetRateControllerData) ||
            (id == KFlyTelemetry::KFly_Command::SetAttitudeControllerData))
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
    ControllerData(const std::vector<uint8_t> &payload)
    {
        if (payload.size() != 38)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            /* Skip the header. */
            int i = 2;

            roll_controller.P_gain = bytes2float( &payload[i] );
            i += 4;
            roll_controller.I_gain = bytes2float( &payload[i] );
            i += 4;
            roll_controller.I_limit = bytes2float( &payload[i] );
            i += 4;

            pitch_controller.P_gain = bytes2float( &payload[i] );
            i += 4;
            pitch_controller.I_gain = bytes2float( &payload[i] );
            i += 4;
            pitch_controller.I_limit = bytes2float( &payload[i] );
            i += 4;

            yaw_controller.P_gain = bytes2float( &payload[i] );
            i += 4;
            yaw_controller.I_gain = bytes2float( &payload[i] );
            i += 4;
            yaw_controller.I_limit = bytes2float( &payload[i] );
        }
    }

    /**
     * @brief  Converts the structure to a byte string that KFly can parse.
     *
     * @return The vector containing the byte string.
     */
    const std::vector<uint8_t> toPayload(void)
    {
        /* Scratchpad for the byte converter. */
        uint8_t scratch[4];

        /* Create the vector and allocate the area needed. */
        std::vector<uint8_t> payload;
        payload.reserve(36);

        /* Add the payload. */
        float2bytes(roll_controller.P_gain, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(roll_controller.I_gain, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(roll_controller.I_limit, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(pitch_controller.P_gain, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(pitch_controller.I_gain, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(pitch_controller.I_limit, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(yaw_controller.P_gain, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(yaw_controller.I_gain, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(yaw_controller.I_limit, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        return payload;
    }
};

/* @brief Affine channel mixing matrix. */
struct ChannelMix : BasePayloadStruct
{
    /* @brief Weights to the throttle, pitch, roll, yaw. */
    float weights[8][4];

    /* @brief Offset of the outputs, used for servos. */
    float offset[8];

    ChannelMix()
    {
        id = KFlyTelemetry::KFly_Command::SetChannelMix;
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    ChannelMix(const std::vector<uint8_t> &payload)
    {
        if (payload.size() != 162)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            /* Skip the header. */
            int i = 2;

            /* Weights. */
            for (int j = 0; j < 8; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    weights[j][k] = bytes2float( &payload[i] );
                    i += 4;
                }
            }

            /* Offset. */
            for (int j = 0; j < 8; j++)
            {
                offset[j] = bytes2float( &payload[i] );
                i += 4;
            }
        }
    }

    /**
     * @brief  Converts the structure to a byte string that KFly can parse.
     *
     * @return The vector containing the byte string.
     */
    const std::vector<uint8_t> toPayload(void)
    {
        /* Scratchpad for the byte converter. */
        uint8_t scratch[4];

        /* Create the vector and allocate the area needed. */
        std::vector<uint8_t> payload;
        payload.reserve(160);

        /* Add the payload. */

        /* Weights. */
        for (int j = 0; j < 8; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                float2bytes(weights[j][k], scratch);
                payload.insert(payload.end(), &scratch[0], &scratch[4]);
            }
        }

        /* Offset. */
        for (int j = 0; j < 8; j++)
        {
            float2bytes(offset[j], scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);
        }

        return payload;
    }
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

    RCCalibration()
    {
        id = KFlyTelemetry::KFly_Command::SetRCCalibration;
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    RCCalibration(const std::vector<uint8_t> &payload)
    {
        if (payload.size() != 102)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            /* Skip the header. */
            int i = 2;

            mode = static_cast<RCInput_Mode>( bytes2u32( &payload[i] ) );
            i += 4;

            for (int j = 0; j < 12; j++)
            {
                role[i] = static_cast<RCInput_Role>( payload[i] );
                i++;
            }

            for (int j = 0; j < 12; j++)
            {
                type[i] = static_cast<RCInput_Type>( payload[i] );
                i++;
            }

            for (int j = 0; j < 12; j++)
            {
                ch_top[i] = bytes2u16( &payload[i] );
                i += 2;
            }

            for (int j = 0; j < 12; j++)
            {
                ch_center[i] = bytes2u16( &payload[i] );
                i += 2;
            }

            for (int j = 0; j < 12; j++)
            {
                ch_top[i] = bytes2u16( &payload[i] );
                i += 2;
            }
        }
    }

    /**
     * @brief  Converts the structure to a byte string that KFly can parse.
     *
     * @return The vector containing the byte string.
     */
    const std::vector<uint8_t> toPayload(void)
    {
        /* Scratchpad for the byte converter. */
        uint8_t scratch[4];

        /* Create the vector and allocate the area needed. */
        std::vector<uint8_t> payload;
        payload.reserve(100);

        /* Add the payload. */

        u322bytes(static_cast<uint32_t>( mode ), scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        for (int j = 0; j < 12; j++)
        {
            u162bytes(static_cast<uint16_t>( role[j] ), scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[2]);
        }

        for (int j = 0; j < 12; j++)
        {
            u162bytes(static_cast<uint16_t>( type[j] ), scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[2]);
        }

        for (int j = 0; j < 12; j++)
        {
            u162bytes(ch_top[j], scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[2]);
        }

        for (int j = 0; j < 12; j++)
        {
            u162bytes(ch_center[j], scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[2]);
        }

        for (int j = 0; j < 12; j++)
        {
            u162bytes(ch_top[j], scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[2]);
        }

        return payload;
    }
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

    GetRCValues(const std::vector<uint8_t> &payload)
    {
        if (payload.size() != 36)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            /* Skip the header. */
            int i = 2;

            active_connection = (bytes2u32( &payload[i] ) != 0);
            i += 4;

            num_connections = bytes2u16( &payload[i] );
            i += 2;

            for (int j = 0; j < 12; j++)
            {
                channel_value[j] = bytes2u16( &payload[i] );
                i += 2;
            }

            rssi = bytes2u16( &payload[i] );
            i += 2;

            rssi_frequency = bytes2u16( &payload[i] );
        }
    }
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

    GetSensorData(const std::vector<uint8_t> &payload)
    {
        if (payload.size() != 42)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            /* Skip the header. */
            int i = 2;

            for (int j = 0; j < 3; j++)
            {
                accelerometer[j] = bytes2float( &payload[i] );
                i += 4;
            }

            for (int j = 0; j < 3; j++)
            {
                gyroscope[j] = bytes2float( &payload[i] );
                i += 4;
            }

            for (int j = 0; j < 3; j++)
            {
                magnetometer[j] = bytes2float( &payload[i] );
                i += 4;
            }

            temperature = bytes2float( &payload[i] );
        }
    }
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

    GetRawSensorData(const std::vector<uint8_t> &payload)
    {
        if (payload.size() != 26)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            /* Skip the header. */
            int i = 2;

            for (int j = 0; j < 3; j++)
            {
                accelerometer[j] = bytes2i16( &payload[i] );
                i += 2;
            }

            for (int j = 0; j < 3; j++)
            {
                gyroscope[j] = bytes2i16( &payload[i] );
                i += 2;
            }

            for (int j = 0; j < 3; j++)
            {
                magnetometer[j] = bytes2i16( &payload[i] );
                i += 2;
            }

            temperature = bytes2i16( &payload[i] );
            i += 2;

            pressure = bytes2u32( &payload[i] );
        }
    }
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

    SensorCalibration()
    {
        id = KFlyTelemetry::KFly_Command::SetSensorCalibration;
        timestamp = static_cast<uint32_t>( std::time(0) );
    }

    /**
     * @brief Converts a payload vector to a proper struct.
     *
     * @param[in] payload   The payload from the KFly including header.
     */
    SensorCalibration(const std::vector<uint8_t> &payload)
    {
        if (payload.size() != 54)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            /* Skip the header. */
            int i = 2;

            for (int j = 0; j < 3; j++)
            {
                accelerometer_bias[j] = bytes2float( &payload[i] );
                i += 4;
            }

            for (int j = 0; j < 3; j++)
            {
                accelerometer_gain[j] = bytes2float( &payload[i] );
                i += 4;
            }

            for (int j = 0; j < 3; j++)
            {
                magnetometer_bias[j] = bytes2float( &payload[i] );
                i += 4;
            }

            for (int j = 0; j < 3; j++)
            {
                magnetometer_gain[j] = bytes2float( &payload[i] );
                i += 4;
            }

            timestamp = bytes2u32( &payload[i] );
        }
    }

    /**
     * @brief  Converts the structure to a byte string that KFly can parse.
     *
     * @return The vector containing the byte string.
     */
    const std::vector<uint8_t> toPayload(void)
    {
        /* Scratchpad for the byte converter. */
        uint8_t scratch[4];

        /* Create the vector and allocate the area needed. */
        std::vector<uint8_t> payload;
        payload.reserve(52);

        /* Add the payload. */

        for (int j = 0; j < 3; j++)
        {
            float2bytes(accelerometer_bias[j], scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);
        }

        for (int j = 0; j < 3; j++)
        {
            float2bytes(accelerometer_gain[j], scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);
        }

        for (int j = 0; j < 3; j++)
        {
            float2bytes(magnetometer_bias[j], scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);
        }

        for (int j = 0; j < 3; j++)
        {
            float2bytes(magnetometer_gain[j], scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);
        }

        u322bytes(timestamp, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        return payload;
    }
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

    ComputerControlReference()
    {
        id = KFlyTelemetry::KFly_Command::ComputerControlReference;
    }

    /**
     * @brief  Converts the structure to a byte string that KFly can parse.
     *
     * @return The vector containing the byte string.
     */
    const std::vector<uint8_t> toPayload(void)
    {
        /* Scratchpad for the byte converter. */
        uint8_t scratch[4];

        /* Create the vector and allocate the area needed. */
        std::vector<uint8_t> payload;
        payload.reserve(24);

        /* Add the payload. */

        u322bytes(static_cast<uint32_t>( mode ), scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        switch (mode)
        {
        case FlightMode::MOTOR_DIRECT_MODE:

            for (int j = 0; j < 8; j++)
            {
                u162bytes(direct_control[j], scratch);
                payload.insert(payload.end(), &scratch[0], &scratch[2]);
            }

            payload.insert(payload.end(), 4, 0);

            break;

        case FlightMode::MOTOR_INDIRECT_MODE:

            float2bytes(indirect_control.roll, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            float2bytes(indirect_control.pitch, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            float2bytes(indirect_control.yaw, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            float2bytes(indirect_control.throttle, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            payload.insert(payload.end(), 4, 0);

            break;

        case FlightMode::RATE_MODE:

            float2bytes(rate.roll, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            float2bytes(rate.pitch, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            float2bytes(rate.yaw, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            float2bytes(rate.throttle, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            payload.insert(payload.end(), 4, 0);

            break;

        case FlightMode::ATTITUDE_MODE:

            float2bytes(attitude.w, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            float2bytes(attitude.x, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            float2bytes(attitude.y, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            float2bytes(attitude.z, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            float2bytes(attitude.throttle, scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[4]);

            break;

        }

        return payload;
    }
};

/* @brief Vicon measurement to KFly data, used for the internal estimation. */
struct ViconMeasurement : BasePayloadStruct
{
    /* @brief Position in meters. */
    float x, y, z;

    /* @brief Attitude in quaternions. */
    float qw, qx, qy, qz;

    ViconMeasurement()
    {
        id = KFlyTelemetry::KFly_Command::ViconMeasurement;
    }

    /**
     * @brief  Converts the structure to a byte string that KFly can parse.
     *
     * @return The vector containing the byte string.
     */
    const std::vector<uint8_t> toPayload(void)
    {
        /* Scratchpad for the byte converter. */
        uint8_t scratch[4];

        /* Create the vector and allocate the area needed. */
        std::vector<uint8_t> payload;
        payload.reserve(28);

        /* Add the payload. */

        float2bytes(x, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(y, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(z, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(qw, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(qx, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(qy, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(qz, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        return payload;
    }
};

}

#endif