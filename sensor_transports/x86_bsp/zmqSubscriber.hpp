#ifndef __ZMQ_SUBSCRIBER_HPP__
#define __ZMQ_SUBSCRIBER_HPP__

#include <memory>
#include <vector>
#include <zmq.hpp>

namespace sensor_transports
{
namespace x86_bsp
{

class ZmqSubscriber
{
public:
    ZmqSubscriber(const ZmqSubscriber&) = delete;
    ZmqSubscriber& operator=(const ZmqSubscriber&) = delete;
    ZmqSubscriber(ZmqSubscriber&&) = delete;
    ZmqSubscriber& operator=(ZmqSubscriber&&) = delete;
    /**
     * @brief Construct a new Zmq Subscriber object
     *
     * @param topic example: "tcp://127.0.0.1:5555"
     */
    explicit ZmqSubscriber(const std::string& topic);

    /**
     * @brief Destroy the Zmq Subscriber object
     */
    virtual ~ZmqSubscriber();
    /**
     * @brief Receive data from the topic with more flag
     *
     * @param buffer buffer to store the received data
     * @return total number of bytes received
     */
    size_t receiveDataMore(uint8_t* buffer, size_t bytes);

    /**
     * @brief Receive data from the topic with last flag
     * 
     * @param buffer buffer to store the received data
     * @param bytes bytes of the user buffer size
     * @return size_t the number of bytes received
     */
    size_t receiveData(uint8_t* buffer, size_t bytes);

    /**
     * @brief Receive data from the topic
     * 
     * @param buffer 
     * @param bytes 
     * @return size_t 
     */
    const std::string& getTopic() const
    {
        return m_topic;
    }
private:
    std::shared_ptr<zmq::context_t> m_context;
    std::shared_ptr<zmq::socket_t> m_socket;
    std::string m_topic;

};
} // namespace x86_bsp
} // namespace sensor_transports


#endif
