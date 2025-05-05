#ifndef __TRANSPORTER_HPP__
#define __TRANSPORTER_HPP__

#include <vector>
#include <memory>

#include "x86_bsp/3rdparty/nlohmann/json.hpp"
#include "x86_bsp/zmqPublisher.hpp"
#include "x86_bsp/zmqSubscriber.hpp"

using json = nlohmann::json;
using namespace sensor_transports::x86_bsp;

namespace sensor_transports
{
namespace transporter
{

class Transporter
{
public:
    Transporter(const json& sensor_context);
    virtual ~Transporter();

    virtual void runLoop() = 0;

protected:
    size_t recvIpcData(uint8_t* buffer, size_t bytes);
    size_t sendTransportData(const void* data, size_t size);

    std::shared_ptr<ZmqSubscriber> getInputSub() const
    {
        return m_input_sub;
    }
    std::shared_ptr<ZmqPublisher> getOutputPub() const
    {
        return m_output_pub;
    }

private:
    std::shared_ptr<ZmqSubscriber> m_input_sub;
    std::shared_ptr<ZmqPublisher> m_output_pub;
    std::string m_type;
    std::string m_name;
    std::string m_ipc_topic;
    std::string m_transport_topic;
};

} // namespace transporter

} // namespace sensor_transports

#endif
