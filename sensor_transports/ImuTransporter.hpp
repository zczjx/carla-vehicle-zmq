#ifndef __IMU_TRANSPORTER_HPP__
#define __IMU_TRANSPORTER_HPP__

#include "Transporter.hpp"
#include <string>
#include <thread>
#include <memory>
#include <atomic>

namespace sensor_transports
{
namespace transporter
{

class ImuTransporter : public Transporter
{
public:
    ImuTransporter(const json& sensor_context);
    ~ImuTransporter();

private:
    void runLoop();
};

} // namespace transporter
} // namespace sensor_transports

#endif
