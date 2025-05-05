#include "CameraTransporter.hpp"

namespace sensor_transports
{
namespace transporter
{

CameraTransporter::CameraTransporter(const json& sensor_context):
    Transporter(sensor_context)
{
    m_xres = sensor_context["image_size_x"];
    m_yres = sensor_context["image_size_y"];
    m_raw_pixel_format = sensor_context["raw_pixel_format"];
    VideoEncoder::EncoderConfig enc_config = {
        .width = m_xres,
        .height = m_yres,
        .fps = 30,
        .bitrate = 400000,
        .gop_size = 10,
        .max_b_frames = 1,
        .pixel_format = m_raw_pixel_format
    };
    m_video_encoder = std::make_unique<VideoEncoder>("h264_nvenc", enc_config);
    m_video_encoder->allocateInputFrame(m_input_frame_buffer);
    m_video_encoder->allocateOutputPkt(m_output_pkt_buffer);
    m_rtp_streamer = std::make_unique<RtpStreamer>(getOutputPub(), 30);
    m_main_thread = std::make_unique<std::thread>([this]() {runLoop();});
}
CameraTransporter::~CameraTransporter()
{
    if (m_main_thread->joinable())
    {
        m_stopSignal = true;
        m_main_thread->join();
    }
}

void CameraTransporter::runLoop()
{
    uint64_t frame_count = 0;
    std::cout << "runLoop" << std::endl;
    while (!m_stopSignal)
    {
        if (recvIpcData(m_video_encoder->getInputFrameDataBuffer(m_input_frame_buffer), m_input_frame_buffer.bytes) < 128)
        {
            continue;
        }
        else
        {
            m_video_encoder->encode(m_input_frame_buffer, m_output_pkt_buffer);
            frame_count++;
            static auto last_time = std::chrono::high_resolution_clock::now();
            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_time).count();

            if (elapsed_time >= 1)
            {
                float fps = static_cast<float>(frame_count) / elapsed_time;
                std::cout << "CameraTransporter FPS: " << fps << std::endl;
                frame_count = 0;
                last_time = current_time;
            }
            m_rtp_streamer->publishRtpPacket(m_output_pkt_buffer);
        }
    }
}

} // namespace transporter
} // namespace sensor_transports
