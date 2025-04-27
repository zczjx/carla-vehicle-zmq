#include "RtpStreamer.hpp"
#include <cstring>
#include <algorithm>

namespace sensor_transports
{
namespace x86_bsp
{

RtpStreamer::RtpStreamer(const std::string& topic, int fps)
    : m_zmq_publisher(std::make_shared<ZmqPublisher>(topic)), fps_(fps), m_rtp_header(RTP_HEADER_SIZE)
{
}

RtpStreamer::~RtpStreamer()
{
}

int RtpStreamer::publishRtpPacket(VideoEncoder::EncOutputPkt& pkt)
{
    fillRtpHeader(pkt, m_rtp_header);
    m_zmq_publisher->sendData(m_rtp_header.data(), m_rtp_header.size());
    m_zmq_publisher->sendData(pkt.av_pkt->data, pkt.av_pkt->size);
    return 0;
}

int RtpStreamer::fillRtpHeader(VideoEncoder::EncOutputPkt& pkt, std::vector<uint8_t>& rtp_header)
{
    rtp_header[0] = 0x80;
    rtp_header[1] = 96;
    if (pkt.av_pkt->flags & AV_PKT_FLAG_KEY) header[1] |= 0x80;
    header[2] = (seq >> 8) & 0xFF;
    header[3] = seq & 0xFF;
    header[4] = (timestamp >> 24) & 0xFF;
    header[5] = (timestamp >> 16) & 0xFF;
    header[6] = (timestamp >> 8) & 0xFF;
    header[7] = timestamp & 0xFF;
    header[8] = (ssrc >> 24) & 0xFF;
    header[9] = (ssrc >> 16) & 0xFF;
    header[10] = (ssrc >> 8) & 0xFF;
    header[11] = ssrc & 0xFF;
    return 0;
}



} // namespace x86_bsp
} // namespace sensor_transports
