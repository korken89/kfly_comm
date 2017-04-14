//          Copyright Emil Fresk 2016 - 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include "../include/kfly_comm/datagram_director.hpp"
#include "../include/kfly_comm/serializable_datagram.hpp"


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

struct data3
{
};

#pragma pack(pop)

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

void callback_d3(data3)
{
  using namespace std;
  cout << "Data3\n";
}

//
// -----------------------------------------------------------------------
//

int main()
{
  using namespace std;

  using d1 = serializable_datagram< data1 >;
  using d2 = serializable_datagram< data2 >;
  using d3 = serializable_datagram< data3 >;

  datagram_director< data1, data2 > cb;

  d1 test1(data1{1, 2});
  d2 test2(data2{'a', 'b', 1, true});
  d3 testd3(data3{});

  auto ser1 = test1.serialize();
  auto ser2 = test2.serialize();
  vector< uint8_t > ser_vec;
  test1.serialize(ser_vec);

  d1 test3(ser1);
  d2 test4(ser2);

  d1 test5(ser_vec);

  auto datagram1 = test3.get_datagram();
  auto datagram2 = test4.get_datagram();

  cb.register_callback(callback_d1);
  cb.register_callback(callback_d11);
  cb.register_callback(callback_d2);
  cb.register_callback(callback_d3); // This should fail

  cb.execute_callback(datagram1);
  cb.execute_callback(datagram2);

  cb.release_callback(callback_d1);

  cb.execute_callback(datagram1);
  cb.execute_callback(datagram2);

  cout << "Test size d1: " << test3.size() << "\n";
  cout << "Test size d2: " << d2::size() << "\n";
  cout << "Test size d3: " << d3::size() << "\n";

  return 0;
}
