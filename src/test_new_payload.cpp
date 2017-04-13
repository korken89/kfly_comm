
#include <iostream>
#include <array>
#include <vector>
#include <type_traits>
#include <cstring>
#include <utility>
#include <functional>
#include <tuple>
#include <algorithm>
#include <exception>

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

  /* Payload creation from serialized (vector) data. */
  constexpr serializable_payload(const std::vector< uint8_t > &data)
  {
    if (data.size() == sizeof(T))
      std::memcpy(&datagram, data.data(), sizeof(T));
    else
      throw std::invalid_argument("Vector size is not the same as datagram.");
  }

  constexpr auto serialize() const noexcept
  {
    make_serialized_array< T > buffer;
    std::memcpy(buffer.data(), &datagram, sizeof(T));
    return buffer;
  }

  constexpr void serialize(std::vector< uint8_t > &buffer) const noexcept
  {
    auto serial_data = serialize();

    buffer = std::vector< uint8_t >(serial_data.data(),
                                    serial_data.data() + serial_data.size());
  }

  constexpr auto size() const noexcept
  {
    return sizeof(T);
  }

  constexpr auto get_datagram() const noexcept
  {
    return datagram;
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

template < typename... Datagrams >
class datagram_callback
{
private:
  template < typename T >
  using function_ref = void (*)(T);

  template < typename T >
  using make_element = std::vector< function_ref< T > >;

  template < typename... Ts >
  using callback_tuple = std::tuple< make_element< Datagrams >... >;

  callback_tuple< data1, data2 > callbacks;

  template < typename T1, typename T2 >
  constexpr auto &get(T2 &p)
  {
    return std::get< make_element< T1 > >(p);
  }

public:
  template < typename T >
  void bind_callback(function_ref< T > fun)
  {
    get< T >(callbacks).emplace_back(fun);
  }

  template < typename T >
  void release_callback(function_ref< T > fun)
  {
    auto &list_cb = get< T >(callbacks);

    list_cb.erase(
        std::remove_if(list_cb.begin(), list_cb.end(),
                       [&](function_ref< T > l_fun) { return fun == l_fun; }),
        list_cb.end());
  }

  template < typename T >
  void execute_callback(const T &data)
  {
    auto get_callbacks = get< T >(callbacks);

    for (auto callback : get_callbacks)
      callback(data);
  }
};

//
// -----------------------------------------------------------------------
//

using d1 = serializable_payload< data1 >;
using d2 = serializable_payload< data2 >;

//
// -----------------------------------------------------------------------
//

//
// -----------------------------------------------------------------------
//

void callback_d1(data1)
{
  using namespace std;
  cout << "Data1\n";
}

void callback_d11(data1)
{
  using namespace std;
  cout << "Data11\n";
}

void callback_d2(data2)
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

  datagram_callback< data1, data2 > cb;

  d1 test1(data1{1, 2});
  d2 test2(data2{'a', 'b', 1, true});

  auto ser1 = test1.serialize();
  auto ser2 = test2.serialize();
  vector< uint8_t > ser_vec;
  test1.serialize(ser_vec);

  d1 test3(ser1);
  d2 test4(ser2);

  d1 test5(ser_vec);

  auto datagram1 = test3.get_datagram();
  auto datagram2 = test4.get_datagram();

  cb.bind_callback(callback_d1);
  cb.bind_callback(callback_d11);
  cb.bind_callback(callback_d2);

  cb.execute_callback(datagram1);
  cb.execute_callback(datagram2);

  cb.release_callback(callback_d1);

  cb.execute_callback(datagram1);
  cb.execute_callback(datagram2);

  return 0;
}
