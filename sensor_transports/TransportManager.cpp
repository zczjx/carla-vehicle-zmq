#include "TransportManager.hpp"
#include "CameraTransporter.hpp"
#include "GnssTransporter.hpp"
#include "ImuTransporter.hpp"
#include "LidarTransporter.hpp"
#include "RadarTransporter.hpp"

namespace sensor_transports
{
namespace transporter
{

TransportManager::TransportManager(const json& sensors_array)
{
    launchSensorTransporter(sensors_array);
}

TransportManager::~TransportManager()
{
    m_transporter_list.clear();
}

void TransportManager::runLoop()
{
    for (const auto& transporter : m_transporter_list)
    {
        transporter->runLoop();
    }
}

void TransportManager::launchSensorTransporter(const json& sensors_array)
{
    for (const auto& sensor : sensors_array)
    {
        std::string sensor_type = sensor["type"];

        if (sensor_type.compare("camera") == 0)
        {
            m_transporter_list.push_back(std::make_unique<CameraTransporter>(sensor));
        }
        else if (sensor_type.compare("gnss") == 0)
        {
            m_transporter_list.push_back(std::make_unique<GnssTransporter>(sensor));
        }
        else if (sensor_type.compare("imu") == 0)
        {
            m_transporter_list.push_back(std::make_unique<ImuTransporter>(sensor));
        }
        else if (sensor_type.compare("lidar") == 0)
        {
            m_transporter_list.push_back(std::make_unique<LidarTransporter>(sensor));
        }
        else if (sensor_type.compare("radar") == 0)
        {
            m_transporter_list.push_back(std::make_unique<RadarTransporter>(sensor));
        }
    }
}
} // namespace transporter
} // namespace sensor_transports
