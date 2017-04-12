
#include <iostream>
#include <array>
#include <vector>
#include <type_traits>
#include <cstring>
#include <utility>

template < std::size_t N >
using serialized_payload = std::array< uint8_t, N >;


template < typename T >
struct serializable_payload
{
  static_assert(std::is_pod< T >::value == true, "Datagram need to be POD.");

  T datagram;

  /* Payload creation from data. */
  constexpr serializable_payload(T &&datagram) noexcept : datagram{std::move(datagram)}
  {
  }

  /* Payload creation from serialized data. */
  template < std::size_t N >
  constexpr serializable_payload(serialized_payload< N > &data) noexcept
  {
    static_assert(N == sizeof(T), "Serialized payload has wrong size");
    std::memcpy(&datagram, data.data(), sizeof(T));
  }

  constexpr auto serialize() const noexcept
  {
    serialized_payload< sizeof(T) > buffer;
    std::memcpy(buffer.data(), &datagram, sizeof(T));
    return buffer;
  }

  template < std::size_t N >
  constexpr void serialize(serialized_payload< N > &buffer) const noexcept
  {
    static_assert(N == sizeof(T), "Output buffer has wrong size");
    std::memcpy(buffer.data(), &datagram, sizeof(T));
  }

  constexpr auto size() const noexcept
  {
    return sizeof(T);
  }

  constexpr auto&& getDatagram() const noexcept
  {
    return std::move(datagram);
  }
};

#pragma pack(push, 1)

struct data1
{
  float a, b;
};

struct data2
{
  char a, b, c;
  int i;
};

#pragma pack(pop)

using d1 = serializable_payload< data1 >;
using d2 = serializable_payload< data2 >;

int main()
{
  using namespace std;

  d1 test1(data1{1, 2});
  d2 test2(data2{'a', 'b', 'c', 13});

  auto ser = test2.serialize();

  d2 test3(ser);

  auto datagram = test3.getDatagram();

  cout << "d1 size: " << sizeof(d1) << "\n";
  cout << "d2 size: " << sizeof(d2) << "\n";

  return 0;
}

