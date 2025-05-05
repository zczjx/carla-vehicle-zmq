#ifndef __RADAR_TRANSPORTER_HPP__
#define __RADAR_TRANSPORTER_HPP__

#include "Transporter.hpp"
#include <string>
#include <thread>
#include <memory>
#include <atomic>

namespace sensor_transports
{
namespace transporter
{

class RadarTransporter : public Transporter
{
public:
    RadarTransporter(const json& sensor_context);
    ~RadarTransporter();

private:
    void runLoop();
};

} // namespace transporter
} // namespace sensor_transports
#endif
