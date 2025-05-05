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
    size_t cycle_count = 0;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        cycle_count++;
        std::cout << "TransportManager cycle_count: " << cycle_count << std::endl;
    }
}

void TransportManager::launchSensorTransporter(const json& sensors_array)
{
    for (const auto& sensor : sensors_array)
    {
        std::string sensor_type = sensor["type"];

        if (sensor_type.compare("camera") == 0)
        {
            std::cout << "launchCameraTransporter camera" << std::endl;
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
