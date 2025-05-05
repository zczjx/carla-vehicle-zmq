#include "LidarTransporter.hpp"

namespace sensor_transports
{
namespace transporter
{

LidarTransporter::LidarTransporter(const json& sensor_context):
    Transporter(sensor_context)
{
}

LidarTransporter::~LidarTransporter()
{
}

void LidarTransporter::runLoop()
{
}

} // namespace transporter
} // namespace sensor_transports
