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

#include <iostream>
#include "KFlyTelemetry/kfly_telemetry.h"

using namespace std;

void test(const std::shared_ptr<KFlyTelemetryPayload::BasePayloadStruct> data);

int main()
{
    /* Create a KFly Telemetry object. */
    KFlyTelemetry::KFlyTelemetry kft;

    /* Register a callback. */
    kft.registerCallback(test);

    /* Generate a test message (PING). */
    std::vector<uint8_t> testPayload({0xc0, 0x2, 0, 0x6d, 0x7b, 0xc0});

    /* Test the parser. */
    kft.parse( testPayload );

    /* Create a message for generation. */
    std::shared_ptr<KFlyTelemetryPayload::BasePayloadStruct> ptr =
        std::make_shared<KFlyTelemetryPayload::SetDeviceIDStruct>("test string");

    /* Generate the payload. */
    const std::vector<uint8_t> patload = kft.generatePacket(ptr);

    for (uint8_t b : patload)
        cout << static_cast<char>( b );

    return 0;
}

/* Print incomming data... */
void test(const std::shared_ptr<KFlyTelemetryPayload::BasePayloadStruct> data)
{
    (void) data;

    /* Do something with incomming messages. */
}

