
#ifndef __TRANSPORT_MANAGER_HPP__
#define __TRANSPORT_MANAGER_HPP__
#include "x86_bsp/3rdparty/nlohmann/json.hpp"
#include <vector>
#include <memory>
#include "Transporter.hpp"

using json = nlohmann::json;
namespace sensor_transports
{
namespace transporter
{
class TransportManager
{
public:
    TransportManager(const json& sensors_array);
    ~TransportManager();

    void runLoop();

private:
    void launchSensorTransporter(const json& sensors_array);
private:
    std::vector<std::unique_ptr<Transporter>> m_transporter_list;
};

} // namespace transporter
} // namespace sensor_transports


#endif
