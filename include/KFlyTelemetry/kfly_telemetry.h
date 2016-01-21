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

#ifndef _KFLY_TELEMETRY_H
#define _KFLY_TELEMETRY_H

/* Data includes */
#include <vector>
#include <cstdint>
#include <map>

/* Threading includes */
#include <mutex>

namespace KFlyTelemetry
{

/** @brief Definition of the message callback. */
typedef std::function<void(const std::vector<uint8_t> &)> kfly_callback;

class KFlyTelemetry
{
private:

    /** @brief Mutex for the ID counter and the callback list. */
    std::mutex _id_cblock;

    /** @brief Vector holding the registered callbacks. */
    std::map<unsigned int, kfly_callback> callbacks;

    /** @brief ID counter for the removal of subscriptions. */
    unsigned int _id;

    /**
     * @brief   Calls all the registered callbacks with the data payload.
     *
     * @param[in] payload   The payload to be sent.
     */
    void executeCallbacks(const std::vector<uint8_t> &payload);

public:
    /**
     * @brief
     */
    KFlyTelemetry();

    ~KFlyTelemetry();

    /**
     * @brief   Register a callback.
     *
     * @param[in] callback  The function to register.
     * @note    Shall be of the form void(const std::vector<uint8_t> &).
     *
     * @return  Return the ID of the callback, is used for unregistration.
     */
    unsigned int registerCallback(kfly_callback callback);

    /**
     * @brief   Unregister a callback from the queue.
     *
     * @param[in] id    The ID supplied from @p registerCallback.
     *
     * @return  Return true if the ID was deleted.
     */
    bool unregisterCallback(const unsigned int id);

};

} // namespace KFlyTelemetry

#endif
