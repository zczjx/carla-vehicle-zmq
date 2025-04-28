#include "zmqSubscriber.hpp"
#include <algorithm>
#include <iostream>
namespace sensor_transports
{
namespace x86_bsp
{

ZmqSubscriber::ZmqSubscriber(const std::string& topic)
{
    m_context = std::make_shared<zmq::context_t>(1);
    m_socket = std::make_shared<zmq::socket_t>(*m_context, ZMQ_SUB);
    m_socket->connect(topic);
    m_socket->set(zmq::sockopt::subscribe, "");
}

ZmqSubscriber::~ZmqSubscriber()
{
    m_socket->close();
    m_context->close();
}

size_t ZmqSubscriber::receiveDataMore(uint8_t* buffer, size_t bytes)
{
    zmq::message_t message;
    m_socket->recv(&message);
    if (message.size() > bytes)
    {
        std::cerr << "receiveDataMore: message size is greater than the user buffer size" << std::endl;
        return 0;
    }
    std::memcpy(buffer, message.data(), message.size());
    size_t bytes_index = message.size();

    while(message.more())
    {
        m_socket->recv(&message);
        if (message.size() > (bytes - bytes_index))
        {
            std::cerr << "receiveDataMore: next message size is greater than the user buffer size" << std::endl;
            return 0;
        }
        std::memcpy(buffer + bytes_index, message.data(), message.size());
        bytes_index += message.size();
    }
    return bytes_index;
}

size_t ZmqSubscriber::receiveData(uint8_t* buffer, size_t bytes)
{
    zmq::message_t message;
    m_socket->recv(&message);
    size_t actual_bytes = std::min(message.size(), bytes);
    std::memcpy(buffer, message.data(), actual_bytes);
    return actual_bytes;
}

} // namespace x86_bsp
} // namespace sensor_transports