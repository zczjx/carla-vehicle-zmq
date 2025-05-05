#include "zmqPublisher.hpp"

namespace sensor_transports
{
namespace x86_bsp
{
ZmqPublisher::ZmqPublisher(const std::string& topic)
    : m_topic(topic)
{
    m_context = std::make_shared<zmq::context_t>(1);
    m_socket = std::make_shared<zmq::socket_t>(*m_context, ZMQ_PUB);
    m_socket->bind(topic);
}

ZmqPublisher::~ZmqPublisher()
{
    m_socket->close();
    m_context->close();
}

size_t ZmqPublisher::sendDataMore(const void* data, size_t size)
{
    return sendData(data, size, true);
}

size_t ZmqPublisher::sendDataLast(const void* data, size_t size)
{
    return sendData(data, size, false);
}

size_t ZmqPublisher::sendData(const void* data, size_t size, bool more)
{
    zmq::message_t message(data, size);
    auto result = m_socket->send(message, more ? zmq::send_flags::sndmore : zmq::send_flags::none);
    return result.value_or(0);
}





} // namespace x86_bsp
} // namespace sensor_transports