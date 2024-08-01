#include "media/video_builder.hpp"

#include "media/constants.hpp"

namespace media
{

    ffmpeg_error::ffmpeg_error(int error_code) : error_buffer{ 0 }, msg{ nullptr }
    {
        av_make_error_string(error_buffer, AV_ERROR_MAX_STRING_SIZE, error_code);
    }

    ffmpeg_error::ffmpeg_error(const char* error_msg) : error_buffer{ 0 }, msg{ error_msg } {}

    const char* ffmpeg_error::what() const noexcept
    {
        return msg ? msg : error_buffer;
    }

    struct video_builder::private_methods
    {
        static void init_context(video_builder& builder, const std::string& fn)
        {
            avformat_alloc_output_context2(&builder.fmt_ctx, nullptr, nullptr, fn.c_str());
            if (!builder.fmt_ctx) throw ffmpeg_error{ "Could not retrieve format information for this file name." };
            builder.output_fmt = builder.fmt_ctx->oformat; // set the output format
        }

        static void init_stream(video_builder& builder)
        {
            builder.codec = avcodec_find_encoder(builder.output_fmt->video_codec);
            if (!builder.codec) throw ffmpeg_error{ "Could not retrieve codec." };

            builder.pkt = av_packet_alloc();
            if (!builder.pkt) throw ffmpeg_error{ "Could not allocate packet." };

            builder.stream = avformat_new_stream(builder.fmt_ctx, nullptr);
            if (!builder.fmt_ctx) throw ffmpeg_error{ "Could not create a new stream." };
            builder.stream->id = (int) builder.fmt_ctx->nb_streams - 1;

            builder.codec_ctx = avcodec_alloc_context3(builder.codec);
            if (!builder.codec_ctx) throw ffmpeg_error{ "Failed to allocate codec context." };
        }

        static void set_settings(video_builder& builder, int width, int height, int bitrate, int fps, int gop, int b_frames)
        {
            builder.codec_ctx->codec_id = builder.output_fmt->video_codec;
            builder.codec_ctx->bit_rate = bitrate;
            builder.codec_ctx->width = width;
            builder.codec_ctx->height = height;
            builder.codec_ctx->time_base = builder.stream->time_base = { 1, fps };
            builder.codec_ctx->gop_size = gop;
            builder.codec_ctx->max_b_frames = b_frames;
            builder.codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

            if (builder.fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER) builder.codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }

        static void init_codec(video_builder& builder)
        {
            int ret;
            ret = avcodec_open2(builder.codec_ctx, builder.codec, nullptr);
            if (ret < 0) throw ffmpeg_error{ ret };

            ret = avcodec_parameters_from_context(builder.stream->codecpar, builder.codec_ctx);
            if (ret < 0) throw ffmpeg_error{ ret };
        }

        static void init_frames(video_builder& builder, int width, int height)
        {
            builder.yuv_frame = av_frame_alloc();
            if (!builder.yuv_frame) throw ffmpeg_error{ "Could not allocate a new frame for YUV420P format." };

            builder.rgb_frame = av_frame_alloc();
            if (!builder.rgb_frame) throw ffmpeg_error{ "Could not allocate a new frame for RGB format." };

            builder.yuv_frame->format = AV_PIX_FMT_YUV420P;
            builder.rgb_frame->format = AV_PIX_FMT_RGB0;

            builder.yuv_frame->width = builder.rgb_frame->width = width;
            builder.yuv_frame->height = builder.rgb_frame->height = height;

            int ret;
            ret = av_frame_get_buffer(builder.yuv_frame, 0);
            if (ret < 0) throw ffmpeg_error{ ret };

            ret = av_frame_get_buffer(builder.rgb_frame, 0);
            if (ret < 0) throw ffmpeg_error{ ret };
        }

        static void open_file(video_builder& builder, const std::string& fn)
        {
            int ret;
            av_dump_format(builder.fmt_ctx, 0, fn.c_str(), true);
            if (!(builder.output_fmt->flags & AVFMT_NOFILE))
            {
                ret = avio_open(&builder.fmt_ctx->pb, fn.c_str(), AVIO_FLAG_WRITE);
                if (ret < 0) throw ffmpeg_error{ ret };
            }
            ret = avformat_write_header(builder.fmt_ctx, nullptr);
            if (ret < 0) throw ffmpeg_error{ ret };
        }

        static void init_sws(video_builder& builder, int width, int height)
        {
            builder.sws_ctx = sws_getContext(width, height, AV_PIX_FMT_RGB0,
                                             width, height, AV_PIX_FMT_YUV420P,
                                             SWS_BICUBIC, nullptr, nullptr, nullptr);
            if (!builder.sws_ctx) throw ffmpeg_error{ "Failed to create sws_context." };
        }

        static void write_to_frame(video_builder& builder, uint8_t *data)
        {
            int ret;
            ret = av_frame_make_writable(builder.yuv_frame);
            if (ret < 0) throw ffmpeg_error{ ret };

            ret = av_frame_make_writable(builder.rgb_frame);
            if (ret < 0) throw ffmpeg_error{ ret };

            std::memcpy(builder.rgb_frame->data[0], data, builder.codec_ctx->width * builder.codec_ctx->height * 4);
            sws_scale(builder.sws_ctx, builder.rgb_frame->data, builder.rgb_frame->linesize,
                      0, builder.codec_ctx->height, builder.yuv_frame->data, builder.yuv_frame->linesize);
            builder.yuv_frame->pts = ++*builder.pts;
        }

        static void log_packet(video_builder& builder)
        {
            AVRational *time_base = &builder.fmt_ctx->streams[builder.pkt->stream_index]->time_base;
            char buf0[AV_TS_MAX_STRING_SIZE], buf1[AV_TS_MAX_STRING_SIZE];
            fmt::print("pts:{} pts_time:{} ", av_ts_make_string(buf0, builder.pkt->pts), av_ts_make_time_string(buf1, builder.pkt->pts, time_base));
            fmt::print("dts:{} dts_time:{} \n", av_ts_make_string(buf0, builder.pkt->dts), av_ts_make_time_string(buf1, builder.pkt->dts, time_base));
        }

        static void send_frame_to_codec(video_builder& builder)
        {
            int ret;
            ret = avcodec_send_frame(builder.codec_ctx, builder.yuv_frame);
            if (ret < 0) throw ffmpeg_error{ ret };

            while (true)
            {
                ret = avcodec_receive_packet(builder.codec_ctx, builder.pkt);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
                else if (ret < 0) throw ffmpeg_error{ ret };

                av_packet_rescale_ts(builder.pkt, builder.codec_ctx->time_base, builder.stream->time_base);
                builder.pkt->stream_index = builder.stream->index;

                if constexpr (LOG_PACKETS) { log_packet(builder); }

                ret = av_interleaved_write_frame(builder.fmt_ctx, builder.pkt);
                if (ret < 0) throw ffmpeg_error{ ret };
            }
        }

    };

    video_builder::video_builder(const std::string& fn, int video_width, int video_height, int video_bitrate, int video_fps, int gop, int b_frames) :
    pts{ new int{ 0 } }, ref_count{ new uint8_t{ 1 } }
    {
        private_methods::init_context(*this, fn);

        private_methods::init_stream(*this);
        private_methods::set_settings(*this, video_width, video_height, video_bitrate, video_fps, gop, b_frames);

        private_methods::init_codec(*this);

        private_methods::init_frames(*this, video_width, video_height);
        private_methods::open_file(*this, fn);

        private_methods::init_sws(*this, video_width, video_height);
    }

    video_builder::video_builder(const video_builder& cpy) :
    output_fmt{ cpy.output_fmt }, fmt_ctx{ cpy.fmt_ctx }, stream{ cpy.stream }, codec{ cpy.codec }, codec_ctx{ cpy.codec_ctx },
    yuv_frame{ cpy.yuv_frame }, rgb_frame{ cpy.rgb_frame }, sws_ctx{ cpy.sws_ctx }, pkt{ cpy.pkt }, pts{ cpy.pts }, ref_count{ cpy.ref_count }
    {
        ++*ref_count;
    }

    video_builder& video_builder::operator=(const video_builder& cpy)
    {
        if (&cpy == this) return *this;

        this->~video_builder(); // free resources if necessary

        output_fmt = cpy.output_fmt;
        fmt_ctx = cpy.fmt_ctx;
        stream = cpy.stream;
        codec = cpy.codec;
        codec_ctx = cpy.codec_ctx;
        yuv_frame = cpy.yuv_frame;
        rgb_frame = cpy.rgb_frame;
        sws_ctx = cpy.sws_ctx;
        pkt = cpy.pkt;

        pts = cpy.pts;
        ref_count = cpy.ref_count;

        ++*ref_count;

        return *this;
    }

    video_builder::~video_builder()
    {
        --*ref_count;
        if (*ref_count == 0)
        {
            av_write_trailer(fmt_ctx);
            avcodec_free_context(&codec_ctx);
            av_frame_free(&yuv_frame);
            av_frame_free(&rgb_frame);
            av_packet_free(&pkt);
            sws_freeContext(sws_ctx);
            if (!(output_fmt->flags & AVFMT_NOFILE)) // close file
            {
                avio_closep(&fmt_ctx->pb);
            }
            avformat_free_context(fmt_ctx);

            delete pts;
            delete ref_count;
        }
    }

    void video_builder::push_frame(uint8_t* rgbx_data)
    {
        private_methods::write_to_frame(*this, rgbx_data);
        private_methods::send_frame_to_codec(*this);
    }

    void video_builder::push_frame(const base::image& img)
    {
        push_frame(reinterpret_cast<uint8_t*>(img.get_buffer()));
    }

}