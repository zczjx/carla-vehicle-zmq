#ifndef __VIDEO_RTP_STREAMER_HPP__
#define __VIDEO_RTP_STREAMER_HPP__

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "VideoEncoder.hpp"
#include "zmqPublisher.hpp"

namespace sensor_transports
{
namespace x86_bsp
{

class RtpStreamer
{
public:

    RtpStreamer(const std::string& topic, int fps);

    virtual ~RtpStreamer();

    int publishRtpPacket(VideoEncoder::EncOutputPkt& pkt);

private:

    int fillRtpHeader(VideoEncoder::EncOutputPkt& pkt, std::vector<uint8_t>& rtp_header);

    uint32_t generateSSRC(const std::string& topic);


private:
    static constexpr int RTP_HEADER_SIZE = 12;
    std::vector<uint8_t> m_rtp_header{};
    uint16_t m_rtp_seq{0};
    uint32_t m_rtp_ssrc{0};
    int m_fps{30};
    int64_t m_last_timestamp{0};
    std::shared_ptr<ZmqPublisher> m_zmq_publisher;
};

} // namespace x86_bsp
} // namespace sensor_transports
#endif // __VIDEO_RTP_STREAMER_HPP__