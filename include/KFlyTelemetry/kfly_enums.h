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

#ifndef _KFLY_ENUMS_H
#define _KFLY_ENUMS_H

#include <cstdint>

namespace KFlyTelemetryPayload
{

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

    /* @brief Throttle role selector. */
    ROLE_THROTTLE = 0,

    /* @brief Pitch role selector. */
    ROLE_PITCH = 1,

    /* @brief Roll role selector. */
    ROLE_ROLL = 2,

    /* @brief Yaw role selector. */
    ROLE_YAW = 3,

    /* @brief Aux 1 role selector. */
    ROLE_AUX1 = 4,

    /* @brief Aux 2 role selector.  */
    ROLE_AUX2 = 5,

    /*  @brief Aux 3 role selector. */
    ROLE_AUX3 = 6,

    /* @brief Role of a non-latching switch to arm the flight controller. */
    ROLE_ARM_NONLATCH = 7,

    /* @brief Role for a latching switch to switch to remote serial control. */
    ROLE_ENABLE_SERIAL_CONTROL = 8,

    /* @brief Aux 1 role selector. */
    ROLE_FLIGHTMODE = 9,

    /* @brief The number of roles - 1. Always have at the end! */
    ROLE_MAX = 10,

    /* @brief Unused role selector. */
    ROLE_OFF = 0xff,
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
enum class RCInput_Mode : uint16_t
{
    /* @brief Analog type input. */
    MODE_CPPM_INPUT = 1,

    /* @brief 3-state type input. */
    MODE_PWM_INPUT = 2
};

/* @brief Input switch states. */
enum class RCInput_Switch_Position : uint8_t
{
    /* @brief   Switch error, not a switch. */
    RCINPUT_SWITCH_UNDEFINED = 0,

    /* @brief   Switch error, not a switch. */
    RCINPUT_SWITCH_NOT_SWITCH = 1,

    /* @brief   Switch at bottom position. */
    RCINPUT_SWITCH_POSITION_BOTTOM = 2,

    /* @brief   Switch at center position. */
    RCINPUT_SWITCH_POSITION_CENTER = 3,

    /* @brief   Switch at top position. */
    RCINPUT_SWITCH_POSITION_TOP = 4
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
    STICK_YAW_MAX,

    /* @brief Arm with the non-latching switch. */
    NON_LATCHING_SWITCH
};

} // namespace KFlyTelemetryPayload

#endif
