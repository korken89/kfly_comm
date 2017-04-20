//          Copyright Emil Fresk 2016 - 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include <cstring>
#include <vector>
#include <stdexcept>
#include "kfly_comm/commands.hpp"
#include "kfly_comm/enums.hpp"

namespace kfly_comm
{
namespace datagrams
{
/*****************************************************************************/
/* NOTE!!! All structures bellow must be inside the #pragmas !!!             */
/*****************************************************************************/

#pragma pack(push, 1)

/* @brief Ack. */
struct Ack
{
};

/* @brief Ping. */
struct Ping
{
};

/* @brief Running mode (bootloader or firmware). */
struct RunningMode
{
  /* @brief 'B' for bootloader, 'P' for program. */
  char sel;
};

/* @brief Manage the subscriptions to messages. */
struct ManageSubscription
{
  /* @brief Port that the messages will be published on. */
  enums::Ports port;

  /* @brief Command to subscribe to. */
  commands cmd;

  /* @brief Selector for subscribe or unsubscribe. */
  bool subscribe;

  /* @brief Milliseconds between publishes, currently no event option. */
  uint32_t delta_ms;
};

/* @brief Version strings and unique identifiers. */
struct SystemStrings
{
  /*========================*/
  /* Identification         */
  /*========================*/

  /**
   * @brief   User selectable name of the vehicle.
   */
  char vehicle_name[48];

  /**
   * @brief   Type of vehicle (quadrotor, Neo, Firefly, Pelican, ...)
   */
  char vehicle_type[48];

  /**
   * @brief   Unique ID, read from the MCU.
   */
  uint8_t unique_id[12];

  /**
   * @brief   KFly and Git compile string.
   */
  char kfly_version[96];
};

/* @brief System status information. */
struct SystemStatus
{
  /*========================*/
  /* General info           */
  /*========================*/

  /**
   * @brief   Flight time in seconds.
   */
  float flight_time;

  /**
   * @brief   Up time in seconds.
   */
  float up_time;

  /**
   * @brief   CPU usage in [0, 1]
   */
  float cpu_usage;

  /*========================*/
  /* Hardware info          */
  /*========================*/

  /**
   * @brief   Battery voltage in V.
   */
  float battery_voltage;

  /**
   * @brief   Flag for if the motors are armed.
   */
  bool motors_armed;

  /**
   * @brief   Flag for if the vehicle is flying.
   */
  bool in_air;

  /**
   * @brief   Flag for if the serial computer control is enabled.
   */
  bool serial_interface_enabled;
};

/* @brief Set the user ID strings. */
struct SetDeviceStrings
{
  /* @brief Value of the user ID string. */
  char _vehicle_name[48];
  char _vehicle_type[48];

  void SetStrings(std::string vehicle_name, std::string vehicle_type)
  {
    std::strncpy(_vehicle_name, vehicle_name.c_str(), 48);
    std::strncpy(_vehicle_type, vehicle_type.c_str(), 48);
  }
};

/* @brief All limits in the control system. */
struct ControllerLimits
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
};

/* @brief Arming settings. */
struct ArmSettings
{
  /* @brief   Stick threshold for the arm/disarm logic to react. */
  float stick_threshold;

  /* @brief   Minimum throttle when armed (to spin propellers when armed). */
  float armed_min_throttle;

  /* @brief   Stick direction to arm the controllers. */
  enums::Arming_Stick_Direction stick_direction;

  /* @brief   Time (in seconds) needed to hold the sticks in correct
   *          position in order to arm the system. */
  uint8_t arm_stick_time;

  /* @brief   Time (in seconds) needed to disarm the controllers if no
   *          throttle has been given. */
  uint8_t arm_zero_throttle_timeout;
};

/* @brief Generic controller data structure. */
struct ControllerData
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
};

/* @brief Rate controller data. */
struct RateControllerData : ControllerData {};

/* @brief Attitude controller data. */
struct AttitudeControllerData : ControllerData {};

/* @brief Affine channel mixing matrix. */
struct ChannelMix
{
  /* @brief Weights to the throttle, pitch, roll, yaw. */
  float weights[8][4];

  /* @brief Offset of the outputs, used for servos. */
  float offset[8];
};

/* @brief Settings structure for the RC inputs. */
struct RCInputSettings
{
  /* @brief If the receiver is in CPPM or PWM mode. */
  enums::RCInput_Mode mode;

  uint16_t use_rssi;

  /* @brief Each channels' input role. */
  enums::RCInput_Role role[12];

  /* @brief Each channels' input type. */
  enums::RCInput_Type type[12];

  /* @brief Flag to reverse a channel. */
  bool ch_reverse[12];

  /* @brief The top value of the RC input (generally around 2000). */
  uint16_t ch_top[12];

  /* @brief The center value of the RC input (generally around 1500). */
  uint16_t ch_center[12];

  /* @brief The bottom value of the RC input (generally around 1000). */
  uint16_t ch_bottom[12];
};

/* @brief Settings structure for the RC inputs. */
struct RCOutputSettings
{
  /* @brief Mode of the outputs of bank 1. */
  enums::RCOutput_Mode mode_bank1;

  /* @brief Mode of the outputs of bank 2. */
  enums::RCOutput_Mode mode_bank2;

  /* @brief Enable flag for each output. */
  bool channel_enabled[8];
};

/* @brief The values and status of the RC input. */
struct RCValues
{
  /* @brief Active connection indicator. */
  uint32_t active_connection;

  /* @brief Number of active inputs (all 12 might not be used). */
  uint16_t num_connections;

  /* @brief The input value of each channel. */
  uint16_t channel_value[12];

  /* @brief The signal strength in percent. */
  uint16_t rssi;

  /* @brief The frequency of the RSSI PWM. */
  uint16_t rssi_frequency;

  /* @brief The calibrated input value of each channel. */
  float calibrated_value[12];

  /* @brief Input switch states. */
  enums::RCInput_Switch_Position switches[3];
};

/* @brief Calibrated sensor data structure. */
struct IMUData
{
  /* @brief Accelerometer data in x, y and z in G. */
  float accelerometer[3];

  /* @brief Gyroscope data in x, y and z in rad/s. */
  float gyroscope[3];

  /* @brief Magnetometer data in x, y and z in normalized units. */
  float magnetometer[3];

  /* @brief The temperature of the IMU in deg/C. */
  float temperature;

  /* @brief Pressure in Pascals. */
  float pressure;

  /* @brief Time stamp (internal clock) in nanoseconds. */
  int64_t time_stamp_ns;
};

/* @brief Raw sensor data, used for calibration or logging. */
struct RawIMUData
{
  /* @brief Accelerometer data in x, y and z in the internal format. */
  int16_t accelerometer[3];

  /* @brief Gyroscope data in x, y and z in the internal format. */
  int16_t gyroscope[3];

  /* @brief Magnetometer data in x, y and z in the internal format. */
  int16_t magnetometer[3];

  /* @brief Temperature of the IMU internal format. */
  int16_t temperature;

  /* @brief Pressure in the internal format. */
  uint32_t pressure;

  /* @brief Time stamp (internal clock) in nanoseconds. */
  int64_t time_stamp_ns;
};

/* @brief IMU calibration structure that takes the internal format and
 *        converts it into the true values. */
struct IMUCalibration
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

/* @brief Estimation Attitude structure, holds the attitude estimation states.
 */
struct EstimationAttitude
{
  /* @brief Quaternion containing attitude. */
  float qw, qx, qy, qz;

  /* @brief Angular rates in [rad/s]. */
  float angular_rate[3];

  /* @brief Angular rate biases in [rad/s]. */
  float rate_bias[3];
};

/* @brief Computer control data. */
struct ComputerControlReference
{
  /* @brief Flight mode to differentiate the following data. */
  enums::FlightMode mode;

  union {
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

/* @brief Motion capture frame to KFly data, used for the internal estimation.
 */
struct MotionCaptureFrame
{
  /* @brief Frame number. */
  uint32_t framenumber;

  /* @brief Position in meters. */
  float x, y, z;

  /* @brief Attitude in quaternions. */
  float qw, qx, qy, qz;
};

/*****************************************************************************/
/* NOTE!!! All structures bellow must be inside the #pragmas !!!             */
/*****************************************************************************/

#pragma pack(pop)

}  // namespace datagrams
}  // namespace kfly_comm
