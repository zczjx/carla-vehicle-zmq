#ifndef __CAMERA_TRANSPORTER_HPP__
#define __CAMERA_TRANSPORTER_HPP__

#include <string>
#include <thread>
#include <memory>
#include <atomic>
#include "Transporter.hpp"
#include "x86_bsp/VideoEncoder.hpp"
#include "x86_bsp/RtpStreamer.hpp"

using namespace sensor_transports::x86_bsp;

namespace sensor_transports
{
namespace transporter
{

class CameraTransporter : public Transporter
{
public:
    CameraTransporter(const json& sensor_context);
    ~CameraTransporter();

private:
    void runLoop();

private:
    int m_xres;
    int m_yres;
    std::string m_raw_pixel_format;
    std::unique_ptr<std::thread> m_main_thread{nullptr};
    std::atomic<bool> m_stopSignal{false};

    std::unique_ptr<VideoEncoder> m_video_encoder;
    VideoEncoder::EncInputFrame m_input_frame_buffer;
    VideoEncoder::EncOutputPkt m_output_pkt_buffer;

    std::unique_ptr<RtpStreamer> m_rtp_streamer{};
};

} // namespace transporter
} // namespace sensor_transports

#endif

