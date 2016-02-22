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

#ifndef _KFLY_COMMANDS_H
#define _KFLY_COMMANDS_H

#include <cstdint>

namespace KFlyTelemetry
{

/* Command are from the view of KFly. */
enum class KFly_Command : uint8_t
{
    /**
     * @brief   The zero command is not allowed.
     */
    None                        = 0,
    /**
     * @brief   ACK command.
     */
    ACK                         = 1,
    /**
     * @brief   Ping command.
     */
    Ping                        = 2,
    /**
     * @brief   Send debug message command.
     */
    DebugMessage                = 3,
    /**
     * @brief   Get running mode command.
     */
    GetRunningMode              = 4,
    /**
     * @brief   Manage Subscriptions command.
     */
    ManageSubscriptions         = 5,

    /*===============================================*/
    /* Bootloader specific commands.                 */
    /*===============================================*/

    /**
     * @brief   Prepare to write firmware command.
     * @note    Bootloader specific, shall always require ACK.
     */
    PrepareWriteFirmware        = 10,
    /**
     * @brief   Write firmware package command.
     * @note    Bootloader specific, shall always require ACK.
     */
    WriteFirmwarePackage        = 11,
    /**
     * @brief   Write the last firmware package.
     * @note    Bootloader specific, shall always require ACK.
     */
    WriteLastFirmwarePackage    = 12,
    /**
     * @brief   Read firmware package.
     * @note    Bootloader specific, shall always require ACK.
     */
    ReadFirmwarePackage         = 13,
    /**
     * @brief   Read the last firmware package.
     * @note    Bootloader specific, shall always require ACK.
     */
    ReadLastFirmwarePackage     = 14,
    /**
     * @brief   Next package command (sent to PC to resume transmission).
     * @note    Bootloader specific, shall always require ACK.
     */
    NextPackage                 = 15,
    /**
     * @brief   Exit bootloader command.
     * @note    Bootloader specific, shall always require ACK.
     */
    ExitBootloader              = 16,

    /*===============================================*/
    /* Info, ID and Save commands.                   */
    /*===============================================*/

    /**
     * @brief   Get device IDs.
     */
    GetDeviceInfo               = 17,
    /**
     * @brief   Set the user definable ID string.
     */
    SetDeviceID                 = 18,
    /**
     * @brief   Save all settings to flash.
     */
    SaveToFlash                 = 19,
    /**
     * @brief   Erase flash.
     */
    EraseFlash                  = 20,

    /*===============================================*/
    /* Controller specific commands.                 */
    /*===============================================*/

    /**
     * @brief   Get contreller settings.
     */
    GetControllerLimits         = 26,
    /**
     * @brief   Set controller settings.
     */
    SetControllerLimits         = 27,
    /**
     * @brief   Get Arm Settings.
     */
    GetArmSettings              = 28,
    /**
     * @brief   Set Arm Settings.
     */
    SetArmSettings              = 29,
    /**
     * @brief   Get rate controller data.
     */
    GetRateControllerData       = 30,
    /**
     * @brief   Set rate controller data.
     */
    SetRateControllerData       = 31,
    /**
     * @brief   Get attitude controller data.
     */
    GetAttitudeControllerData   = 32,
    /**
     * @brief   Set attitude controller data.
     */
    SetAttitudeControllerData   = 33,
    /**
     * @brief   Get velocity controller data.
     */
    GetVelocityControllerData   = 34,
    /**
     * @brief   Set velocity controller data.
     */
    SetVelocityControllerData   = 35,
    /**
     * @brief   Get position controller data.
     */
    GetPositionControllerData   = 36,
    /**
     * @brief   Set position controller data.
     */
    SetPositionControllerData   = 37,
    /**
     * @brief   Get channel mixer.
     */
    GetChannelMix               = 39,
    /**
     * @brief   Set channel mixer.
     */
    SetChannelMix               = 40,

    /*===============================================*/
    /* RC specific commands.                         */
    /*===============================================*/

    /**
     * @brief   Get RC calibration.
     */
    GetRCCalibration            = 41,
    /**
     * @brief   Set RC calibration.
     */
    SetRCCalibration            = 42,
    /**
     * @brief   Get RC values.
     */
    GetRCValues                 = 43,

    /*===============================================*/
    /* Sensor specific commands.                     */
    /*===============================================*/

    /**
     * @brief   Get calibrated IMU data.
     */
    GetIMUData                  = 44,
    /**
     * @brief   Get raw IMU data.
     */
    GetRawIMUData               = 45,
    /**
     * @brief   Get IMU calibration.
     */
    GetIMUCalibration           = 46,
    /**
     * @brief   Set IMU calibration.
     */
    SetIMUCalibration           = 47,

    /*===============================================*/
    /* Estimation specific commands.                 */
    /*===============================================*/

    /**
     * @brief   Get rate estimation.
     */
    GetEstimationRate           = 48,
    /**
     * @brief   Get attitude estimation.
     */
    GetEstimationAttitude       = 49,
    /**
     * @brief   Get velocity estimation.
     */
    GetEstimationVelocity       = 50,
    /**
     * @brief   Get position estimation.
     */
    GetEstimationPosition       = 51,
    /**
     * @brief   Get all states.
     */
    GetEstimationAllStates      = 52,
    /**
     * @brief   Reset estimation.
     */
    ResetEstimation             = 53,

    /*===============================================*/
    /* Computer control specific commands.           */
    /*===============================================*/

    /**
     * @brief   New control reference from the PC side.
     */
    ComputerControlReference    = 126,

    /*===============================================*/
    /* Motion capture frame specific commands.       */
    /*===============================================*/

    /**
     * @brief   New motion capture measurement.
     */
    MotionCaptureMeasurement    = 127
};

}

#endif
