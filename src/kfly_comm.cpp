//          Copyright Emil Fresk 2016 - 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "kfly_comm/kfly_comm.h"

namespace kfly_comm
{
/*********************************
 * Private members
 ********************************/

void codec::parse_packet(const std::vector< uint8_t > &payload)
{
  /* Extract command. */
  const uint8_t cmd = payload[0];

  /* Extract size and expected size. */
  const unsigned int expected_size = (unsigned int)payload[1];
  const unsigned int length        = payload.size();

  /* Extract the CRC and remove it. */
  union {
    uint8_t b[2];
    uint16_t u16;
  } crc;

  crc.b[0] = payload[length - 2];
  crc.b[1] = payload[length - 1];

  /* Remove CRC. */
  std::vector< uint8_t > data(payload.begin(), payload.end() - 2);

  /* Calculate CRC. */
  uint16_t crc_calc = CRC16_CCITT::generateCRC(data);

  /* Remove header. */
  data.erase(data.begin(), data.begin() + 2);

  /* Check sizes and CRC. */
  if (expected_size + 4 != length)
  {
    /* Size error. */
    return;
  }
  else if (crc_calc != crc.u16)
  {
    /* CRC error. */
    return;
  }
  else
  {
    /* Send payload to further processing. */
    try
    {
      transmit_datagram(cmd, data);
    }
    catch (const std::invalid_argument &e)
    {
      /* Whenever a wrong sized payload is parsed, this will run. */
    }
  }
}

void codec::transmit_datagram(const uint8_t cmd,
                              const std::vector< uint8_t > &payload)
{
  /* Extract the command. */
  const commands command = static_cast< commands >(cmd);

  /* Do appropriate operation for each command. */
  switch (command)
  {
    case commands::ACK:

      _callbacks.execute_callback(serializable_datagram< datagrams::Ack >());
      break;

    case commands::Ping:

      _callbacks.execute_callback(serializable_datagram< datagrams::Ping >());
      break;

    case commands::GetRunningMode:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::GetRunningMode >(payload));
      break;

    case commands::GetSystemInformation:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::GetSystemInformation >(payload));
      break;

    case commands::GetControllerLimits:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::ControllerLimits >(payload));
      break;

    case commands::GetArmSettings:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::ArmSettings >(payload));
      break;

    case commands::GetRateControllerData:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::ControllerData >(payload));
      break;

    case commands::GetAttitudeControllerData:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::ControllerData >(payload));
      break;

    case commands::GetChannelMix:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::ChannelMix >(payload));
      break;

    case commands::GetRCInputSettings:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::RCInputSettings >(payload));
      break;

    case commands::GetRCOutputSettings:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::RCOutputSettings >(payload));
      break;

    case commands::GetRCValues:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::GetRCValues >(payload));
      break;

    case commands::GetIMUData:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::GetIMUData >(payload));
      break;

    case commands::GetRawIMUData:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::GetRawIMUData >(payload));
      break;

    case commands::GetIMUCalibration:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::IMUCalibration >(payload));
      break;

    case commands::GetEstimationAttitude:

      _callbacks.execute_callback(
          serializable_datagram< datagrams::GetEstimationAttitude >(payload));
      break;

    default:
      break;
  }
}

/*********************************
 * Public members
 ********************************/

codec::codec() : _parser()
{
  /* Register the KFly packet parser to the parser output. */
  _parser.registerCallback([&](const std::vector< uint8_t > &payload) {
    this->parse_packet(payload);
  });
}

codec::~codec()
{
}


void codec::parse(const uint8_t data)
{
  std::lock_guard< std::mutex > locker(_parser_lock);

  _parser.parse(data);
}

void codec::parse(const std::vector< uint8_t > &payload)
{
  std::lock_guard< std::mutex > locker(_parser_lock);

  _parser.parse(payload);
}


template <typename Datagram>
std::vector< uint8_t > generate_packet(const Datagram& datagram)
{
}

template <>
std::vector< uint8_t > generate_packet<datagrams::Ack>(const datagrams::Ack&)
{
}

template <>
std::vector< uint8_t > generate_packet<datagrams::Ping>(const datagrams::Ping&)
{
}


// std::vector< uint8_t > codec::generatePacket(BasePayloadStruct &payload,
//                                             bool ack)
//{
//  std::vector< uint8_t > packet, parser_packet;
//  const std::vector< uint8_t > data_payload = payload.toPayload();
//
//  union {
//    uint8_t b[2];
//    uint16_t u16;
//  } crc;
//
//  /* Construct the final packet: | CMD | SIZE | PAYLOAD | CRC | */
//  if (ack)
//    packet.emplace_back(static_cast< uint8_t >(payload.id) | 0x80);
//  else
//    packet.emplace_back(static_cast< uint8_t >(payload.id));
//
//  packet.emplace_back(static_cast< uint8_t >(data_payload.size()));
//
//  if (data_payload.size() > 0)
//    packet.insert(packet.end(), data_payload.begin(), data_payload.end());
//
//  crc.u16 = CRC16_CCITT::generateCRC(packet);
//  packet.insert(packet.end(), &crc.b[0], &crc.b[2]);
//
//  kfly_parser::encode(packet, parser_packet);
//
//  return parser_packet;
//}
//
// std::vector< uint8_t > codec::generatePacket(
//    const std::shared_ptr< BasePayloadStruct > &payload, bool ack)
//{
//  std::vector< uint8_t > packet, parser_packet;
//  const std::vector< uint8_t > data_payload = payload->toPayload();
//
//  union {
//    uint8_t b[2];
//    uint16_t u16;
//  } crc;
//
//  /* Construct the final packet: | CMD | SIZE | PAYLOAD | CRC | */
//  if (ack)
//    packet.emplace_back(static_cast< uint8_t >(payload->id) | 0x80);
//  else
//    packet.emplace_back(static_cast< uint8_t >(payload->id));
//
//  packet.emplace_back(static_cast< uint8_t >(data_payload.size()));
//
//  if (data_payload.size() > 0)
//    packet.insert(packet.end(), data_payload.begin(), data_payload.end());
//
//  crc.u16 = CRC16_CCITT::generateCRC(packet);
//  packet.insert(packet.end(), &crc.b[0], &crc.b[2]);
//
//  kfly_parser::encode(packet, parser_packet);
//
//  return parser_packet;
//}
}
