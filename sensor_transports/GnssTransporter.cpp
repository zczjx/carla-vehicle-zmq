#include "GnssTransporter.hpp"

namespace sensor_transports
{
namespace transporter
{

GnssTransporter::GnssTransporter(const json& sensor_context):
    Transporter(sensor_context)
{

}

GnssTransporter::~GnssTransporter()
{
}

void GnssTransporter::runLoop()
{
}

} // namespace transporter
} // namespace sensor_transports