#include "ImuTransporter.hpp"

namespace sensor_transports
{
namespace transporter
{

ImuTransporter::ImuTransporter(const json& sensor_context):
    Transporter(sensor_context)
{

}

ImuTransporter::~ImuTransporter()
{
}

void ImuTransporter::runLoop()
{
}

} // namespace transporter
} // namespace sensor_transports