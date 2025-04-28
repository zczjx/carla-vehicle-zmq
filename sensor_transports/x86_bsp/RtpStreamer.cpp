#include "RtpStreamer.hpp"
#include <cstring>
#include <algorithm>
#include <functional>
namespace sensor_transports
{
namespace x86_bsp
{

RtpStreamer::RtpStreamer(const std::string& topic, int fps)
    : m_zmq_publisher(std::make_shared<ZmqPublisher>(topic)), m_fps(fps), m_rtp_header(RTP_HEADER_SIZE)
    , m_rtp_ssrc(generateSSRC(topic))
{
}

RtpStreamer::~RtpStreamer()
{
}

int RtpStreamer::publishRtpPacket(VideoEncoder::EncOutputPkt& pkt)
{
    fillRtpHeader(pkt, m_rtp_header);
    m_zmq_publisher->sendDataMore(m_rtp_header.data(), m_rtp_header.size());
    m_zmq_publisher->sendDataLast(pkt.av_pkt->data, pkt.av_pkt->size);
    return 0;
}

int RtpStreamer::fillRtpHeader(VideoEncoder::EncOutputPkt& pkt, std::vector<uint8_t>& rtp_header)
{
    if(rtp_header.size() != RTP_HEADER_SIZE)
    {
        rtp_header.resize(RTP_HEADER_SIZE);
    }

    // start of header
    rtp_header[0] = 0x80;
    // marker
    bool isKeyFrame = (pkt.av_pkt->flags & AV_PKT_FLAG_KEY) != 0;
    rtp_header[1] = isKeyFrame ? 0x80 : 0x00;
    rtp_header[1] |= 96;  // H264负载类型


    // sequence number
    rtp_header[2] = (m_rtp_seq >> 8) & 0xFF;
    rtp_header[3] = m_rtp_seq & 0xFF;
    m_rtp_seq = (m_rtp_seq + 1) & 0xFFFF;  // 回绕处理

    // timestamp
    uint32_t timestamp;
    if (pkt.av_pkt->pts != AV_NOPTS_VALUE)
    {
        timestamp = pkt.av_pkt->pts * 90000 / pkt.av_pkt->time_base.den;
    }
    else
    {
        // 使用当前时间或上一帧时间戳+增量
        timestamp = m_last_timestamp + 90000 / m_fps;
    }
    m_last_timestamp = timestamp;

    rtp_header[4] = (timestamp >> 24) & 0xFF;
    rtp_header[5] = (timestamp >> 16) & 0xFF;
    rtp_header[6] = (timestamp >> 8) & 0xFF;
    rtp_header[7] = timestamp & 0xFF;


    rtp_header[8] = (m_rtp_ssrc >> 24) & 0xFF;
    rtp_header[9] = (m_rtp_ssrc >> 16) & 0xFF;
    rtp_header[10] = (m_rtp_ssrc >> 8) & 0xFF;
    rtp_header[11] = m_rtp_ssrc & 0xFF;

    return 0;
}

uint32_t RtpStreamer::generateSSRC(const std::string& topic)
{
    std::hash<std::string> hash_fn;
    size_t hash_value = hash_fn(topic);
    return static_cast<uint32_t>(hash_value);
}



} // namespace x86_bsp
} // namespace sensor_transports
