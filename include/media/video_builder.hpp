#ifndef GPU_RAYTRACE_VIDEO_BUILDER_HPP
#define GPU_RAYTRACE_VIDEO_BUILDER_HPP

#include <exception>

#include <fmt/core.h>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/timestamp.h>
}

namespace media
{

    class ffmpeg_error : public std::exception
    {
    private:
        char error_buffer[AV_ERROR_MAX_STRING_SIZE];
        const char *msg;
    public:
        ffmpeg_error(int error_code);

        ffmpeg_error(const char* error_msg);

        const char* what() const noexcept override;
    };

    /**
     * builder for a video.
     * converts from RGB data to YUV for encoding.
     * not thread-safe in that multiple threads cannot write frames concurrently
     * reference counted.
     */
    class video_builder
    {
    private:
        const AVOutputFormat* output_fmt = nullptr;
        AVFormatContext* fmt_ctx = nullptr;

        AVStream* stream = nullptr;
        const AVCodec* codec = nullptr;
        AVCodecContext* codec_ctx = nullptr;
        AVFrame* yuv_frame = nullptr, * rgb_frame = nullptr;
        SwsContext* sws_ctx = nullptr;
        AVPacket* pkt = nullptr;

        int* pts;
        uint8_t* ref_count;

        struct private_methods; // forward declare inner class for private member functions
    public:
        video_builder(const std::string& fn, int width, int height, int bitrate, int fps, int gop, int b_frames);

        video_builder(const video_builder& cpy);

        video_builder(video_builder&&) = delete;

        video_builder& operator=(const video_builder& cpy);

        video_builder& operator=(video_builder&&) = delete;

        ~video_builder();

        void push_frame(uint8_t* rgbx_data);
    };

}

#endif //GPU_RAYTRACE_VIDEO_BUILDER_HPP
