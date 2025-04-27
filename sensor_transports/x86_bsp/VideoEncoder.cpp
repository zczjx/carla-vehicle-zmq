#include "VideoEncoder.hpp"
#include <iostream>

namespace sensor_transports
{
namespace x86_bsp
{

VideoEncoder::VideoEncoder(const std::string &codec_name, EncoderConfig& config)
    : m_codec_name(codec_name), m_config(config)
{
    /* find the mpeg1video encoder */
    m_codec.reset(avcodec_find_encoder_by_name(m_codec_name.c_str()));

    if (nullptr == m_codec)
    {
        std::cerr << "Codec: " << m_codec_name << "not found" << std::endl;
        exit(1);
    }

    m_ctx = std::shared_ptr<AVCodecContext>(avcodec_alloc_context3(m_codec.get()), [](AVCodecContext* p) { avcodec_free_context(&p); });

    if (nullptr == m_ctx)
    {
        std::cerr << "Could not allocate video codec context" << std::endl;
        exit(1);
    }

    m_ctx->bit_rate = m_config.bitrate;
    m_ctx->width = m_config.width;
    m_ctx->height = m_config.height;
    m_ctx->time_base = (AVRational){1, m_config.fps};
    m_ctx->framerate = (AVRational){m_config.fps, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    m_ctx->gop_size = m_config.gop_size;
    m_ctx->max_b_frames = m_config.max_b_frames;
    m_ctx->pix_fmt = m_pixel_format_map.at(m_config.pixel_format);

    if (m_codec->id == AV_CODEC_ID_H264)
    {
        av_opt_set(m_ctx->priv_data, "preset", "llhp", 0); // 低延迟高性能
        av_opt_set(m_ctx->priv_data, "rc", "constqp", 0);  // 固定QP
    }

    /* open it */
    if (avcodec_open2(m_ctx.get(), m_codec.get(), NULL) < 0)
    {
        std::cerr << "Could not open codec: " << m_codec_name << std::endl;
        exit(1);
    }
}

int VideoEncoder::allocateInputFrame(EncInputFrame& frame)
{
    frame.av_frame = std::shared_ptr<AVFrame>(av_frame_alloc(), [](AVFrame* p) { av_frame_free(&p); });
    if (nullptr == frame.av_frame)
    {
        std::cerr << "Could not allocate video frame" << std::endl;
        exit(1);
    }

    frame.av_frame->format = m_ctx->pix_fmt;
    frame.av_frame->width = m_ctx->width;
    frame.av_frame->height = m_ctx->height;

    int ret = av_frame_get_buffer(frame.av_frame.get(), 0);
    if(ret < 0)
    {
        std::cerr << "Could not allocate video frame buffer" << std::endl;
        exit(1);
    }
    frame.bytes = av_image_get_buffer_size(m_ctx->pix_fmt, m_ctx->width, m_ctx->height, 1);
    return 0;
}

uint8_t* VideoEncoder::getInputFrameDataBuffer(EncInputFrame& frame)
{
    return frame.av_frame->data[0];
}

int VideoEncoder::setInputFramePts(EncInputFrame& frame, int64_t pts)
{
    frame.av_frame->pts = pts;
    return 0;
}

int VideoEncoder::allocateOutputPkt(EncOutputPkt& pkt)
{
    pkt.av_pkt = std::shared_ptr<AVPacket>(av_packet_alloc(), [](AVPacket* p) { av_packet_free(&p); });
    if (nullptr == pkt.av_pkt)
    {
        std::cerr << "Could not allocate video packet" << std::endl;
        exit(1);
    }
    pkt.bytes = 0;
    return 0;
}


void VideoEncoder::flushEncode(std::shared_ptr<FILE> outfile)
{
    EncInputFrame frame{nullptr, 0};
    encode(frame, outfile);
    fclose(outfile.get());
}


int VideoEncoder::encode(EncInputFrame &frame, EncOutputPkt &pkt)
{
    int ret;

    ret = avcodec_send_frame(m_ctx.get(), frame.av_frame.get());

    if (ret < 0)
    {
        std::cerr << "Error sending a frame for encoding" << std::endl;
        exit(1);
    }

    while (ret >= 0)
    {
        ret = avcodec_receive_packet(m_ctx.get(), pkt.av_pkt.get());

        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            return 0;
        }
        else if (ret < 0)
        {
            std::cerr << "Error during encoding" << std::endl;
            exit(1);
        }

        pkt.bytes = pkt.av_pkt->size;
        return pkt.bytes;
    }

    return 0;
}

void VideoEncoder::encode(EncInputFrame &frame, std::shared_ptr<FILE> outfile)
{

    if(nullptr == m_internal_cache_pkt.av_pkt)
    {
        allocateOutputPkt(m_internal_cache_pkt);
    }

    int ret = encode(frame, m_internal_cache_pkt);

    if (ret <= 0)
    {
        return;
    }

    std::cout << "Write packet " << m_internal_cache_pkt.av_pkt->pts << " (size=" << m_internal_cache_pkt.av_pkt->size << ")" << std::endl;
    fwrite(m_internal_cache_pkt.av_pkt->data, 1, m_internal_cache_pkt.av_pkt->size, outfile.get());
    av_packet_unref(m_internal_cache_pkt.av_pkt.get());
}

VideoEncoder::~VideoEncoder()
{
    m_ctx.reset();
    m_codec.reset();
}

} // namespace x86_bsp
} // namespace sensor_transports