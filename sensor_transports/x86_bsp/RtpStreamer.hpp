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


private:
    // RTP打包相关参数
    static constexpr int RTP_HEADER_SIZE = 12;
    static constexpr int MAX_RTP_PAYLOAD = 1400;
    std::vector<uint8_t> m_rtp_header{};
    uint16_t rtp_seq_ = 0;
    uint32_t rtp_timestamp_ = 0;
    uint32_t rtp_ssrc_ = 0x12345678;
    int fps_ = 30;
    std::shared_ptr<ZmqPublisher> m_zmq_publisher;
};

} // namespace x86_bsp
} // namespace sensor_transports
#endif // __VIDEO_RTP_STREAMER_HPP__