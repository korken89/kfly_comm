#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>

using namespace std;

namespace crc
{
namespace details
{

template < typename CRC, CRC Polynomial >
constexpr CRC get_crc(const uint8_t message)
{
  const constexpr CRC top_bit = (1 << (8 * sizeof(CRC) - 1));
  CRC remainder               = message << (8 * (sizeof(CRC) - 1));

  for (uint8_t bit = 8; bit > 0; --bit)
  {
    if (remainder & top_bit)
      remainder = (remainder << 1) ^ Polynomial;
    else
      remainder = (remainder << 1);
  }

  return remainder;
}

template < typename CRC, CRC Polynomial, unsigned int... I >
constexpr auto make_crc_table_impl(std::integer_sequence< unsigned int, I... >)
{
  return std::array< CRC, 256 >{{get_crc< CRC, Polynomial >(I)...}};
}

} // details

template < typename CRC, CRC Polynomial>
constexpr auto make_crc_table(void)
{
  return details::make_crc_table_impl< CRC, Polynomial >(
      std::make_integer_sequence< unsigned int, 256 >{});
}

uint16_t generateCRC(const std::vector< uint8_t > &payload,
                     const uint16_t crc_start = 0xffff)
{
  static constexpr const auto crc_table = make_crc_table< uint16_t, 0x1021 >();

  uint16_t crc = crc_start;

  for (auto data : payload)
  {
    uint8_t tbl_idx = ((crc >> 8) ^ data) & 0xff;
    crc             = crc_table[tbl_idx] ^ (crc << 8);
  }

  return crc;
}

} // crc

int main()
{
  const int per_line = 8;

  static constexpr const auto crc_table =
    crc::make_crc_table< uint16_t, 0x1021 >();

  int i = 0;

  for (auto b : crc_table)
  {
    cout << "  0x" << setfill('0') << setw(4) << hex << uppercase << b << "    ";

    if (i++ == per_line - 1)
    {
      i = 0;
      cout << "\n";
    }
  }

  // for (uint8_t i = 0; i < 0xff/per_line; i++)
  // {
  //   for (uint8_t j = 0; j < per_line; j++)
  //   {
  //     cout << "  " << setfill('0') << setw(8) << hex << uppercase <<
  //       get_crc<uint32_t, 0x04C11DB7>(i*per_line+j) << "\t";
  //   }
  //
  //   cout << dec << "\n";
  // }

  return 0;
}
