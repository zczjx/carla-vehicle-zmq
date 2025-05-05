#ifndef __LIDAR_TRANSPORTER_HPP__
#define __LIDAR_TRANSPORTER_HPP__

#include "Transporter.hpp"
#include <string>
#include <thread>
#include <memory>
#include <atomic>

namespace sensor_transports
{
namespace transporter
{

class LidarTransporter : public Transporter
{
public:
    LidarTransporter(const json& sensor_context);
    ~LidarTransporter();

private:
    void runLoop();
};

} // namespace transporter
} // namespace sensor_transports
#endif
