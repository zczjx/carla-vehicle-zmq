extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

namespace sensor_transports
{
namespace x86_bsp
{

class VideoEncoder
{
public:
    /**
     * @brief Encoder config
     */
    struct EncoderConfig
    {
        int width{1280};
        int height{720};
        int fps{30};
        int bitrate{400000};
        int gop_size{10};
        int max_b_frames{0};
        /**
         * @brief Pixel format
         *  - BGRA
         *  - RGBA
         *  - RGB
         *  - NV12
         *  - NV21
         *  - YUV420P
         */
        std::string pixel_format{"BGRA"};
    };

    struct EncInputFrame
    {
        std::shared_ptr<AVFrame> av_frame{nullptr};
        size_t bytes{0};
    };

    struct EncOutputPkt
    {
        std::shared_ptr<AVPacket> av_pkt{nullptr};
        size_t bytes{0};
    };

    /**
     * @brief Construct a new Video Encoder object
     *
     * @param codec_name codec name, supported codecs:
     *  - h264_nvenc
     *  - hevc_nvenc
     *  - av1_nvenc
     * *
     * @param config encoder config
     */

    VideoEncoder(const std::string &codec_name, EncoderConfig& config);

    int allocateInputFrame(EncInputFrame& frame);

    uint8_t* getInputFrameDataBuffer(EncInputFrame& frame);

    int setInputFramePts(EncInputFrame& frame, int64_t pts);

    int allocateOutputPkt(EncOutputPkt& pkt);

    uint8_t* getOutputPktBuffer(EncOutputPkt& pkt);

    void flushEncode(std::shared_ptr<FILE> outfile);

    int encode(EncInputFrame &frame, EncOutputPkt &pkt);

    void encode(EncInputFrame &frame, std::shared_ptr<FILE> outfile);

    ~VideoEncoder();

private:
    const std::unordered_map<std::string, AVPixelFormat> m_pixel_format_map{
        {"BGRA", AV_PIX_FMT_BGRA},
        {"RGBA", AV_PIX_FMT_RGBA},
        {"RGB", AV_PIX_FMT_RGB24},
        {"NV12", AV_PIX_FMT_NV12},
        {"NV21", AV_PIX_FMT_NV21},
        {"YUV420P", AV_PIX_FMT_YUV420P},
    };


private:
    std::shared_ptr<const AVCodec> m_codec{nullptr};
    std::string m_codec_name{};
    std::shared_ptr<AVCodecContext> m_ctx{nullptr};
    struct EncoderConfig m_config{};

    EncOutputPkt m_internal_cache_pkt{nullptr};
};

} // namespace x86_bsp
} // namespace sensor_transports
