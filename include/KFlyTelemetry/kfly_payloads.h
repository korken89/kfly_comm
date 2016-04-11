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

    /* @brief Control via Euler references (not implemented). */
    ATTITUDE_EULER_MODE = 4,

    /* @brief Control via attitude references. */
    ATTITUDE_MODE = 5
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
    PORT_AUX3 = 3,

    /* @brief Same as the message came on. */
    PORT_SAME = 0xff
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

/**
 * @brief   Possible stick direction for arming the controllers.
 */
enum class Arming_Stick_Direction : uint8_t
{
    /* @brief   Arm direction not yet set. */
    STICK_NONE = 0,

    /* @brief Arm at pitch at min. */
    STICK_PITCH_MIN,

    /* @brief Arm at pitch at max. */
    STICK_PITCH_MAX,

    /* @brief Arm at roll at min. */
    STICK_ROLL_MIN,

    /* @brief Arm at roll at max. */
    STICK_ROLL_MAX,

    /* @brief Arm at yaw at min. */
    STICK_YAW_MIN,

    /* @brief Arm at yaw at max. */
    STICK_YAW_MAX
};

/* @brief Base structure that allows generic usage of functions. */
struct BasePayloadStruct
{
    /* @brief ID for the casting later. */
    KFlyTelemetry::KFly_Command id;

    /**
     * @brief  Default payload (no data).
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
        if (payload.size() != 1)
            throw std::invalid_argument( "Payload too small" );
        else
        {
            sel = payload[0];
        }
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
        if (payload.size() < 17)
            throw std::invalid_argument( "Payload too small" );
        else
        {
            unsigned int i;
            const unsigned int size = payload.size();

            for (i = 0; i < 12; i++)
                unique_id[i] = payload[i];

            while ((payload[i] != 0) && (i < size))
                bootloader_version.append( 1, payload[i++] );

            i++;

            while ((payload[i] != 0) && (i < size))
                firmware_version.append( 1, payload[i++] );

            i++;

            while ((payload[i] != 0) && (i < size))
                user_string.append( 1, payload[i++] );
        }
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
     * @param[in] payload   The payload from the KFly including header.
     */
    ControllerLimitsStruct(const std::vector<uint8_t> &payload)
    {
        if (payload.size() != 40)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

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
        if (payload.size() != 11)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

            stick_threshold = bytes2float( &payload[i] );
            i += 4;
            armed_min_throttle = bytes2float( &payload[i] );
            i += 4;
            stick_direction = static_cast<Arming_Stick_Direction>( payload[i] );
            i += 1;
            arm_stick_time = payload[i];
            i += 1;
            arm_zero_throttle_timeout = payload[i];
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
        payload.reserve(11);

        /* Add the payload. */
        float2bytes(stick_threshold, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(armed_min_throttle, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        payload.emplace_back( static_cast<uint8_t>( stick_direction ) );
        payload.emplace_back( arm_stick_time );
        payload.emplace_back( arm_zero_throttle_timeout );

        return payload;
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
        if (payload.size() != 24)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

            roll_controller.P_gain = bytes2float( &payload[i] );
            i += 4;
            roll_controller.I_gain = bytes2float( &payload[i] );
            i += 4;

            pitch_controller.P_gain = bytes2float( &payload[i] );
            i += 4;
            pitch_controller.I_gain = bytes2float( &payload[i] );
            i += 4;

            yaw_controller.P_gain = bytes2float( &payload[i] );
            i += 4;
            yaw_controller.I_gain = bytes2float( &payload[i] );
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

        float2bytes(pitch_controller.P_gain, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(pitch_controller.I_gain, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(yaw_controller.P_gain, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        float2bytes(yaw_controller.I_gain, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

        return payload;
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
        if (payload.size() != 160)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

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
struct RCCalibrationStruct : BasePayloadStruct
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
        if (payload.size() != 100)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

            mode = static_cast<RCInput_Mode>( bytes2u32( &payload[i] ) );
            i += 4;

            for (int j = 0; j < 12; j++)
            {
                role[j] = static_cast<RCInput_Role>( payload[i] );
                i++;
            }

            for (int j = 0; j < 12; j++)
            {
                type[j] = static_cast<RCInput_Type>( payload[i] );
                i++;
            }

            for (int j = 0; j < 12; j++)
            {
                ch_top[j] = bytes2u16( &payload[i] );
                i += 2;
            }

            for (int j = 0; j < 12; j++)
            {
                ch_center[j] = bytes2u16( &payload[i] );
                i += 2;
            }

            for (int j = 0; j < 12; j++)
            {
                ch_bottom[j] = bytes2u16( &payload[i] );
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
            payload.emplace_back(static_cast<uint8_t>( role[j] ));
        }

        for (int j = 0; j < 12; j++)
        {
            payload.emplace_back(static_cast<uint8_t>( type[j] ));
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
            u162bytes(ch_bottom[j], scratch);
            payload.insert(payload.end(), &scratch[0], &scratch[2]);
        }

        return payload;
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
        if (payload.size() != 34)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

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
        if (payload.size() != 40)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

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
        if (payload.size() != 24)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

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
        if (payload.size() != 52)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

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
        if (payload.size() != 40)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

            qw = bytes2float( &payload[i] );
            i += 4;

            qx = bytes2float( &payload[i] );
            i += 4;

            qy = bytes2float( &payload[i] );
            i += 4;

            qz = bytes2float( &payload[i] );
            i += 4;

            for (int j = 0; j < 3; j++)
            {
                angular_rate[j] = bytes2float( &payload[i] );
                i += 4;
            }

            for (int j = 0; j < 3; j++)
            {
                rate_bias[j] = bytes2float( &payload[i] );
                i += 4;
            }
        }
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
        if (payload.size() != 18)
            throw std::invalid_argument( "Wrong size payload" );
        else
        {
            int i = 0;

            acc_z = ((double)bytes2i16( &payload[i] )) / 10000.0;
            i += 2;

            for (int j = 0; j < 3; j++)
            {
                angular_rate[j] = ((double)bytes2i16( &payload[i] )) / 10000.0;
                i += 2;
            }

            for (int j = 0; j < 3; j++)
            {
                torque_ref[j] = ((double)bytes2i16( &payload[i] )) / 10000.0;
                i += 2;
            }

            throttle_ref = ((double)bytes2i16( &payload[i] )) / 10000.0;
            i += 2;

            cnt = bytes2u16( &payload[i] );
        }
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
        payload.reserve(21);

        /* Add the payload. */
        payload.insert(payload.end(), static_cast<uint8_t>( mode ));

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
        payload.reserve(32);

        /* Add the payload. */

        u322bytes(framenumber, scratch);
        payload.insert(payload.end(), &scratch[0], &scratch[4]);

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

} // namespace KFlyTelemetryPayload

#endif
