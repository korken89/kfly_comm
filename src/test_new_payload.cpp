
#include <iostream>
#include <array>
#include <vector>
#include <type_traits>
#include <cstring>
#include <utility>
#include <functional>
#include <tuple>
#include <algorithm>

template < std::size_t N >
using serialized_array = std::array< uint8_t, N >;

template < typename T >
using make_serialized_array = serialized_array< sizeof(T) >;

template < typename T >
struct serializable_payload
{
  /* POD forces a default constructor. */
  static_assert(std::is_pod< T >::value == true, "Datagram need to be POD.");

  /* Trivially copyable guarantees correct operation for std::memcpy. */
  static_assert(std::is_trivially_copyable< T >::value == true,
                "Datagram need to be trivially copyable.");

  T datagram;

  /* Payload creation from data. */
  constexpr serializable_payload(T &&datagram) noexcept
      : datagram{std::move(datagram)}
  {
  }

  /* Payload creation from serialized data. */
  template < std::size_t N >
  constexpr serializable_payload(serialized_array< N > &data) noexcept
  {
    static_assert(N == sizeof(T), "Input serialized_array has wrong size.");
    std::memcpy(&datagram, data.data(), sizeof(T));
  }

  constexpr auto serialize() const noexcept
  {
    make_serialized_array< T > buffer;
    std::memcpy(buffer.data(), &datagram, sizeof(T));
    return buffer;
  }

  template < std::size_t N >
  constexpr void serialize(serialized_array< N > &buffer) const noexcept
  {
    static_assert(N == sizeof(T), "Output serialized_array has wrong size.");
    std::memcpy(buffer.data(), &datagram, sizeof(T));
  }

  constexpr void serialize(std::vector< uint8_t > &buffer) const noexcept
  {
    auto data = serialize();
    buffer    = std::vector< uint8_t >(data.data(), data.data() + data.size());
  }

  constexpr auto size() const noexcept
  {
    return sizeof(T);
  }

  constexpr auto getDatagram() const noexcept
  {
    return std::move(datagram);
  }
};

//
// -----------------------------------------------------------------------
//

#pragma pack(push, 1)

struct data1
{
  int a, b;
};

struct data2
{
  char a, b;
  unsigned int i;
  bool bl;
};

#pragma pack(pop)

//
// -----------------------------------------------------------------------
//

namespace datagram_callback
{
template < typename T >
using make_element = std::vector< std::function< void(T) > >;

template < typename... Ts >
using tuple = std::tuple< make_element< Ts >... >;

template < typename T, typename... Ts >
constexpr auto &get(tuple< Ts... > &p)
{
  return std::get< datagram_callback::make_element< T > >(p);
}
}

using dt = datagram_callback::tuple< data1, data2 >;

//
// -----------------------------------------------------------------------
//

using d1 = serializable_payload< data1 >;
using d2 = serializable_payload< data2 >;

//
// -----------------------------------------------------------------------
//

dt datagram_holder;

template < typename T >
void registerCallback(void(&&fun)(T))
{
  using namespace std;
  cout << "Register ptr\n";

  datagram_callback::get< T >(datagram_holder)
      .emplace_back(std::function< void(T) >(fun));
}

template < typename T >
void registerCallback(std::function< void(T) > &&fun)
{
  using namespace std;
  cout << "Register fun\n";

  datagram_callback::get< T >(datagram_holder).emplace_back(fun);
}

template < typename T >
void executeCallback(const T &data)
{
  auto callbacks = datagram_callback::get< T >(datagram_holder);

  for (auto callback : callbacks)
    callback(data);
}

//
// -----------------------------------------------------------------------
//

void cbd1(data1)
{
  using namespace std;
  cout << "Data1\n";
}

void cbd2(data2)
{
  using namespace std;
  cout << "Data2\n";
}

//
// -----------------------------------------------------------------------
//

int main()
{
  using namespace std;

  d1 test1(data1{1, 2});
  d2 test2(data2{'a', 'b', 1, true});

  auto ser1 = test1.serialize();
  auto ser2 = test2.serialize();

  d1 test3(ser1);
  d2 test4(ser2);

  auto datagram1 = test3.getDatagram();
  auto datagram2 = test4.getDatagram();

  registerCallback(cbd1);
  registerCallback(std::function< void(data2) >(cbd2));

  executeCallback(datagram1);
  executeCallback(datagram2);

  return 0;
}
