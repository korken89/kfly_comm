//          Copyright Emil Fresk 2016 - 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

/* Data includes */
#include <vector>
#include <cstdint>
#include <map>
#include <memory>

/* Threading includes */
#include <mutex>

/* Library includes */
#include "cppSLIP/slip.h"
//#include "cobs/cobs.h"
#include "kfly_comm/datagram_director.hpp"
#include "kfly_comm/serializable_datagram.hpp"

/* KFly includes */
#include "kfly_comm/commands.hpp"
#include "kfly_comm/datagrams.hpp"
#include "kfly_comm/crc.hpp"

namespace kfly_comm
{
/** @brief Definition of the parser type. */
using kfly_parser = SLIP::SLIP;

class codec
{
private:
  /** @brief Mutex for the ID counter and the callback list. */
  std::mutex _id_cblock;

  /** @brief Parser for the system. */
  kfly_parser _parser;

  /** @brief Lock for the parser. */
  std::mutex _parser_lock;

  /** @brief Datagram director for the callbacks and registered datagrams. */
  datagram_director<
                     datagrams::Ack,
                     datagrams::Ping,
                     datagrams::GetRunningMode,
                     datagrams::ManageSubscription,
                     datagrams::GetSystemInformation,
                     datagrams::SetDeviceStrings,
                     datagrams::ControllerLimits,
                     datagrams::ArmSettings,
                     datagrams::ControllerData,
                     datagrams::ChannelMix,
                     datagrams::RCInputSettings,
                     datagrams::RCOutputSettings,
                     datagrams::GetRCValues,
                     datagrams::GetIMUData,
                     datagrams::GetRawIMUData,
                     datagrams::IMUCalibration,
                     datagrams::GetEstimationAttitude,
                     datagrams::GetEstimationAllStates,
                     datagrams::ComputerControlReference,
                     datagrams::MotionCaptureFrame
                   > _callbacks;

  /**
   * @brief   Parses a payload and, if correct, runs executeCallbacks.
   *
   * @param[in] payload   The payload to be parsed.
   */
  void parse_packet(const std::vector< uint8_t > &payload);

  /**
   * @brief   Checks the command to apply the proper structure.
   *
   * @param[in] cmd       Command byte from the packet.
   * @param[in] payload   The payload to be parsed, without header and CRC.
   */
  void transmit_datagram(const uint8_t cmd,
                         const std::vector< uint8_t > &payload);

public:
  codec();

  ~codec();

  /**
   * @brief   Register a callback.
   *
   * @param[in] callback  The function to register.
   * @note    Shall be of the form void(kfly_comm::datagrams::xxx).
   */
  template <typename Datagram>
  void register_callback(void (*callback)(Datagram))
  {
    _callbacks.register_callback(callback);
  }

  /**
   * @brief   Unregister a callback from the queue.
   *
   * @param[in] callback  The function to release.
   */
  template <typename Datagram>
  void release_callback(void (*callback)(Datagram))
  {
    _callbacks.release_callback(callback);
  }

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
  void parse(const std::vector< uint8_t > &payload);

  /**
   * @brief   Converts a BasePayloadStruct to a byte message for transmission.
   *
   * @param[in] payload   The BasePayloadStruct payload to be converted.
   * @param[in] ack       If true, then an ack is requested.
   *
   * @return A vector that holds the generated message.
   */
  //static std::vector< uint8_t > generatePacket(BasePayloadStruct &payload,
  //                                             bool ack);

  /**
   * @brief   Converts a BasePayloadStruct to a byte message for transmission.
   *
   * @param[in] payload   The BasePayloadStruct payload to be converted
   *                      passed by shared pointer..
   *
   * @return A vector that holds the generated message.
   */
  //static std::vector< uint8_t > generatePacket(
  //    const std::shared_ptr< BasePayloadStruct > &payload, bool ack);
};

}  // namespace KFlyTelemetry


