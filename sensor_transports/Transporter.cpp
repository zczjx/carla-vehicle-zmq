#include "Transporter.hpp"

namespace sensor_transports
{
namespace transporter
{

Transporter::Transporter(const json& sensor_context)
{
    m_type = sensor_context["type"];
    m_name = sensor_context["name"];
    m_ipc_topic = sensor_context["zmq_ipc"];
    m_transport_topic = sensor_context["zmq_transport"];

    m_input_sub = std::make_shared<ZmqSubscriber>(m_ipc_topic);
    m_output_pub = std::make_shared<ZmqPublisher>(m_transport_topic);
}

Transporter::~Transporter()
{
    m_input_sub.reset();
    m_output_pub.reset();
}

size_t Transporter::recvIpcData(uint8_t* buffer, size_t bytes)
{
    return m_input_sub->receiveData(buffer, bytes);
}

size_t Transporter::sendTransportData(const void* data, size_t size)
{
    return m_output_pub->sendData(data, size, false);
}

} // namespace transporter
} // namespace sensor_transports
