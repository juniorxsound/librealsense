// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015 Intel Corporation. All Rights Reserved.

#pragma once

#include "context.h"
#include "device.h"
#include <limits>

namespace librealsense
{
    /**
    * device_hub class - encapsulate the handling of connect and disconnect events
    */
    class device_hub
    {
    public:
        explicit device_hub(std::shared_ptr<librealsense::context> ctx, int vid = 0, bool register_device_notifications = true);

        /**
         * The function implements both blocking and non-blocking device generation functionality based on the input parameters:
         * Calling the function with zero timeout results in searching and fetching the device specified by the `serial` parameter
         * from a list of connected devices. The call will return nullptr if none is found.
         * Calling the function with a valid timeout will block till the resulted device is found or the timeout expires.
         *
         * \param[in] timeout_ms  The waiting period for the requested device to be reconnected (milliseconds).
         * Set it to zero to avoid blocking the thread execution
         *\param[in] loop_through_devices  - promote internal index to the next available device that will be retrieved on successive call
         * \param[in] serial  The serial number of the requested device. Use empty pattern ("") to request for "any suitable" device.
         * \return       a shared pointer to the device_interface that satisfies the search criteria. In case of failure the function will
         * return nullptr for the non-blocking call The function will throw and throw exception when a blocking wait is timed out
         */
        std::shared_ptr<device_interface> wait_for_device( unsigned int timeout_ms = std::numeric_limits<unsigned int>::max(),
                                                            bool loop_through_devices = true,
                                                            const std::string& serial = "");

        /**
        * Checks if device is still connected
        */
        bool is_connected(const device_interface& dev);

        ~device_hub()
        {
            _ctx->stop();
        }

    private:
        std::shared_ptr<device_interface> create_device(const std::string& serial, bool cycle_devices = true);
        std::shared_ptr<librealsense::context> _ctx;
        std::mutex _mutex;
        std::condition_variable _cv;
        std::vector<std::shared_ptr<device_info>> _device_list;
        int _camera_index = 0;
        int _vid = 0;
        bool _register_device_notifications;
    };
}


