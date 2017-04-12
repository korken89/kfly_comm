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

/* KFly includes */
#include "KFlyTelemetry/kfly_commands.h"
#include "KFlyTelemetry/kfly_payloads.h"
#include "KFlyTelemetry/crc.h"

namespace KFlyTelemetry
{
using namespace KFlyTelemetryPayload;

/** @brief Definition of the message in the callback. */
using kfly_message = const std::shared_ptr< BasePayloadStruct >;

/** @brief Definition of the message callback. */
using kfly_callback = std::function< void(kfly_message) >;

/** @brief Definition of the parser type. */
using kfly_parser = SLIP::SLIP;

class codec
{
private:
  /** @brief Mutex for the ID counter and the callback list. */
  std::mutex _id_cblock;

  /** @brief Vector holding the registered callbacks. */
  std::map< unsigned int, kfly_callback > callbacks;

  /** @brief ID counter for the removal of subscriptions. */
  unsigned int _id;

  /** @brief Parser for the system. */
  kfly_parser _parser;

  /** @brief Lock for the parser. */
  std::mutex _parser_lock;

  /**
   * @brief   Calls all the registered callbacks with the data payload.
   *
   * @param[in] payload   The payload to be sent.
   */
  void executeCallbacks(std::shared_ptr< BasePayloadStruct > payload);

  /**
   * @brief   Parses a payload and, if correct, runs executeCallbacks.
   *
   * @param[in] payload   The payload to be parsed.
   */
  void parseKFlyPacket(const std::vector< uint8_t > &payload);

  /**
   * @brief   Checks the command to apply the proper structure.
   *
   * @param[in] cmd       Command byte from the packet.
   * @param[in] payload   The payload to be parsed, without header or CRC.
   *
   * @return A BasePayloadStruct that holds the parsed message.
   */
  std::shared_ptr< BasePayloadStruct > payloadToStruct(
      const uint8_t cmd, const std::vector< uint8_t > &payload);

public:
  codec();

  ~codec();

  /**
   * @brief   Register a callback.
   *
   * @param[in] callback  The function to register.
   * @note    Shall be of the form void(const std::vector<uint8_t> &).
   *
   * @return  Return the ID of the callback, is used for unregistration.
   */
  unsigned int registerCallback(kfly_callback callback);

  /**
   * @brief   Unregister a callback from the queue.
   *
   * @param[in] id    The ID supplied from @p registerCallback.
   *
   * @return  Return true if the ID was deleted.
   */
  bool unregisterCallback(const unsigned int id);

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
  static std::vector< uint8_t > generatePacket(BasePayloadStruct &payload,
                                               bool ack);

  /**
   * @brief   Converts a BasePayloadStruct to a byte message for transmission.
   *
   * @param[in] payload   The BasePayloadStruct payload to be converted
   *                      passed by shared pointer..
   *
   * @return A vector that holds the generated message.
   */
  static std::vector< uint8_t > generatePacket(
      const std::shared_ptr< BasePayloadStruct > &payload, bool ack);
};

}  // namespace KFlyTelemetry

