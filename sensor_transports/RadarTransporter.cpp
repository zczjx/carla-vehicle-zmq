#include "RadarTransporter.hpp"

namespace sensor_transports
{
namespace transporter
{

RadarTransporter::RadarTransporter(const json& sensor_context):
    Transporter(sensor_context)
{
}

RadarTransporter::~RadarTransporter()
{
}

void RadarTransporter::runLoop()
{
}

} // namespace transporter
} // namespace sensor_transports