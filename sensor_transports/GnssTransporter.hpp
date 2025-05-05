#ifndef __GNSS_TRANSPORTER_HPP__
#define __GNSS_TRANSPORTER_HPP__

#include "Transporter.hpp"
#include <string>
#include <thread>
#include <memory>
#include <atomic>

namespace sensor_transports
{
namespace transporter
{

class GnssTransporter : public Transporter
{
public:
    GnssTransporter(const json& sensor_context);
    ~GnssTransporter();

private:
    void runLoop();
};

} // namespace transporter
} // namespace sensor_transports

#endif
