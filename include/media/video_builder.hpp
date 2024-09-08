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

#include "base/image.hpp"

namespace media
{

    /**
     * class for errors that occur while using ffmpeg library functions.
     */
    class ffmpeg_error : public std::exception
    {
    private:
        char error_buffer[AV_ERROR_MAX_STRING_SIZE];
        const char *msg;
    public:

        /**
         * initializes the error based on error codes returned by ffmpeg library functions.
         * @param error_code error code returned by ffmpeg library functions.
         */
        ffmpeg_error(int error_code);

        /**
         * initializes error with a custom error message
         * @param error_msg custom error message for when ffmpeg library functions return null
         */
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

        /**
         * initializes the builder for the video. after construction, frames can be readily appended to the video.
         * upon its destruction, the video is outputted.
         * @param fn file name of the video. requires an extension for ffmpeg to deduce the formatting type.
         * @param width width in pixels of the video. must be a multiple of two.
         * @param height height in pixels of the video. must be a multiple of two.
         * @param bitrate the bitrate used in the video encoding.
         * @param fps frames per second for the video
         * @param gop group of pictures numbers.
         * @param b_frames max number of b-frames in the gop.
         */
        video_builder(const std::string& fn, int width, int height, int bitrate, int fps, int gop, int b_frames);

        /**
         * performs a shallow copy and increments one to the reference counter.
         * @param cpy video builder to be referenced now
         */
        video_builder(const video_builder& cpy);

        video_builder(video_builder&&) = delete;

        /**
         * performs a shallow copy assignment and sets this objects reference to the copy.
         * @param cpy video builder to be referenced now
         * @return self
         */
        video_builder& operator=(const video_builder& cpy);

        video_builder& operator=(video_builder&&) = delete;

        /**
         * decrements the reference counter. frees resources and saves video.
         */
        ~video_builder();

        /**
         * pushes a single frame of data from rgbx data.
         * @param rgbx_data bytes in the format RGBXRGBX...
         * @note every fourth byte is unused (marked X). this is for alignment.
         */
        void push_frame(uint8_t* rgbx_data);

        /**
         * pushes a single frame of data from an image.
         * @param img image object to be recorded.
         */
        void push_frame(const base::image& img);
    };

}

#endif //GPU_RAYTRACE_VIDEO_BUILDER_HPP
