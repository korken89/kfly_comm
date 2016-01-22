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


#ifndef _KFLY_CRC_H
#define _KFLY_CRC_H

#include <cstdint>
#include <vector>

class CRC16_CCITT
{
public:
    /**
     * @brief   Calculates the CRC-CCITT of a payload.
     *
     * @param[in] payload   The payload to be checked.
     * @param[in] payload   The starting value of the CRC, can be use to chain
     *                      multiple calculations.
     *
     * @return          The calculated CRC-CCITT.
     */
    static uint16_t generateCRC(const std::vector<uint8_t> &payload,
                                const uint16_t crc_start = 0xffff);
};

#endif
