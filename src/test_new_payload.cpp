
#include <iostream>
#include <array>
#include <vector>
#include <type_traits>
#include <cstring>
#include <utility>
#include <functional>

template < std::size_t N >
using serialized_array = std::array< uint8_t, N >;

template <typename T>
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
  constexpr serializable_payload(T &&datagram) noexcept : datagram{std::move(datagram)}
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

  constexpr void serialize(std::vector<uint8_t> &buffer) const noexcept
  {
    auto data = serialize();
    buffer = std::vector<uint8_t>(data.data(), data.data() + data.size());
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

using d1 = serializable_payload< data1 >;
using d2 = serializable_payload< data2 >;

template <typename T, std::size_t N>
void print_array(std::array<T, N> arr)
{
  using namespace std;

  for (auto b : arr)
    cout << "  " << static_cast<int>(b) << "\n";

  cout << "\n";
}

void print_array(std::vector<uint8_t> arr)
{
  using namespace std;

  for (auto b : arr)
    cout << "  " << static_cast<int>(b) << "\n";

  cout << "\n";
}

template <typename T>
void registerCallback(std::function< void(T) > fun)
{
  // ???
}

int main()
{
  using namespace std;

  d1 test1(data1{1, 2});
  d2 test2(data2{'a', 'b', 1, true});

  auto ser1 = test1.serialize();
  auto ser2 = test2.serialize();
  std::vector<uint8_t> test;
  test1.serialize(test);

  d1 test3(ser1);
  d2 test4(ser2);

  auto datagram1 = test3.getDatagram();
  auto datagram2 = test4.getDatagram();

  (void)datagram1;
  (void)datagram2;


  cout << "d1 size: " << sizeof(d1) << "\n";
  cout << "d2 size: " << sizeof(d2) << "\n\n";

  cout << "ser1:\n";
  print_array(ser1);
  cout << "ser1:\n";
  print_array(test);
  cout << "ser2:\n";
  print_array(ser2);

  return 0;
}

