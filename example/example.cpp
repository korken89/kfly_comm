//          Copyright Emil Fresk 2016 - 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
#include "kfly_comm/kfly_comm.h"

using namespace std;

void test_ping(kfly_comm::datagrams::Ping);

struct test
{
  void callback(kfly_comm::datagrams::SystemStatus)
  {
  }
};

int main()
{
  /* Create a KFly Telemetry object. */
  kfly_comm::codec kft;


  test t_obj;


  /* Register a callback. */
  kft.register_callback(test_ping);
  kft.register_callback(&t_obj, &test::callback);

  /* Generate a test message (PING). */
  auto testPayload = kft.generate_command(kfly_comm::commands::Ping);

  /* Test the parser. */
  kft.parse(testPayload);

  /* Create a message for generation. */
  auto payload = kfly_comm::datagrams::SetDeviceStrings();
  payload.SetStrings("aaa", "bbb");

  /* Generate the payload. */
  auto packet = kft.generate_packet(payload);

  for (uint8_t b : packet)
    cout << "0x" << setfill('0') << setw(2) << hex << uppercase
         << static_cast< int >(b) << " ";

  cout << "\n";

  return 0;
}

/* Print incoming data... */
void test_ping(kfly_comm::datagrams::Ping)
{
  cout << "Got Ping!!!\n";
}
