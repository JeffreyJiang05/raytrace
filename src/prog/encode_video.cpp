#include <iostream>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/timestamp.h>
}

constexpr inline int DEFAULT_WIDTH = 1080;
constexpr inline int DEFAULT_HEIGHT = 720;
constexpr inline int DEFAULT_BITRATE = 400000;
constexpr inline int DEFAULT_FRAMES_PER_SECOND = 30;
constexpr inline int DEFAULT_GOP_SIZE = 12;
constexpr inline int DEFAULT_MAX_B_FRAMES = 2;

struct OutputVideoStream
{
    AVStream *stream;
    AVCodecContext *codec_ctx;

    int presentation_ts;

    AVFrame *frame;
    AVFrame *cvt_frame;

    SwsContext *sws_ctx;

    AVPacket *pkt;

    AVPixelFormat pixel_format;
};

struct VideoSettings
{
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int bitrate = DEFAULT_BITRATE;
    int fps = DEFAULT_FRAMES_PER_SECOND;
    int gop_size = DEFAULT_GOP_SIZE;
    int max_b_frames = DEFAULT_MAX_B_FRAMES;
    AVPixelFormat pixel_format = AV_PIX_FMT_YUV420P;
};

void initialize_stream(OutputVideoStream& vstream, AVFormatContext *output_ctx, const AVCodec **codec, enum AVCodecID codec_id, VideoSettings settings = VideoSettings{ })
{
    *codec = avcodec_find_encoder(codec_id);
    if (!*codec) { std::puts("Failed to get codec. Exiting..."); std::exit(-1); }

    vstream.pkt = av_packet_alloc();
    if (!vstream.pkt) { std::puts("Failed to allocate new packet. Exiting..."); std::exit(-1); }

    vstream.stream = avformat_new_stream(output_ctx, nullptr);
    if (!vstream.stream) { std::puts("Failed to allocate new stream. Exiting..."); std::exit(-1); }
    vstream.stream->id = (int) output_ctx->nb_streams - 1;

    vstream.codec_ctx = avcodec_alloc_context3(*codec);
    if (!vstream.codec_ctx) { std::puts("Failed to allocate codec context. Exiting..."); std::exit(-1); }

    // set video codec settings
    AVCodecContext *ctx = vstream.codec_ctx;
    ctx->codec_id = codec_id;

    ctx->bit_rate = settings.bitrate;
    ctx->width = settings.width;
    ctx->height = settings.height;

    ctx->time_base = vstream.stream->time_base = AVRational{ 1, settings.fps };
    ctx->gop_size = settings.gop_size;
    ctx->max_b_frames = settings.max_b_frames;
    ctx->pix_fmt = AV_PIX_FMT_YUV420P;

    vstream.pixel_format = settings.pixel_format;

    if (output_ctx->oformat->flags & AVFMT_GLOBALHEADER) ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

void free_stream(OutputVideoStream& vstream)
{
    avcodec_free_context(&vstream.codec_ctx);
    av_frame_free(&vstream.frame);
    av_frame_free(&vstream.cvt_frame);
    av_packet_free(&vstream.pkt);
    sws_freeContext(vstream.sws_ctx);
}

AVFrame* initialize_frame(AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *frame = av_frame_alloc();
    if (!frame) return nullptr;

    frame->format = pix_fmt;
    frame->width = width;
    frame->height = height;

    int ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) { std::puts("Could not allocate buffer for frame. Exiting..."); std::exit(-1); }

    return frame;
}

void initialize_video(OutputVideoStream& vstream, const AVCodec *codec)
{
    int ret;
    ret = avcodec_open2(vstream.codec_ctx, codec, nullptr);
    if (ret < 0) { std::puts("Could not open video codec. Exiting..."); std::exit(-1); }

    vstream.frame = initialize_frame(AV_PIX_FMT_YUV420P, vstream.codec_ctx->width, vstream.codec_ctx->height);
    if (!vstream.frame) { std::puts("Could not allocate video frame. Exiting..."); std::exit(-1); }

    // initialize conversion frame if needed.
    vstream.cvt_frame = nullptr;
    if (vstream.pixel_format != AV_PIX_FMT_YUV420P)
    {
        vstream.cvt_frame = initialize_frame(vstream.pixel_format, vstream.codec_ctx->width, vstream.codec_ctx->height);
        if (!vstream.cvt_frame) { std::puts("Could not allocate video frame for conversion. Exiting..."); std::exit(-1); }
    }

    ret = avcodec_parameters_from_context(vstream.stream->codecpar, vstream.codec_ctx);
    if (ret < 0) { std::puts("Could not copy parameters from the codec context to the stream. Exiting"); std::exit(-1); }
}

void write_pixel_data(uint8_t** data, int frame, int width, int height)
{
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            data[0][3 * (y * width + x) + 0] = data[0][3 * (y * width + x) + 1] = data[0][3 * (y * width + x) + 2] = (uint8_t) (frame * (255.0 / 120.0)); // 120 frames at 30 fps = 4 s video
        }
    }
}

void write_frame(OutputVideoStream& vstream)
{
    AVCodecContext *codec_ctx = vstream.codec_ctx;

    if (av_frame_make_writable(vstream.frame) < 0) { std::puts("Could not make the frame writable. Exiting..."); std::exit(-1); }

    if (vstream.pixel_format != AV_PIX_FMT_YUV420P)
    {
        // convert ang other pixel format to YUV420P for video encoding
        if (!vstream.sws_ctx) // initialize sws_ctx if needed
        {
            vstream.sws_ctx = sws_getContext(codec_ctx->width, codec_ctx->height, vstream.pixel_format,
                                             codec_ctx->width, codec_ctx->height, AV_PIX_FMT_YUV420P,
                                             SWS_BICUBIC, nullptr, nullptr, nullptr);
            if (!vstream.sws_ctx) { std::puts("Failed to retrieve SWS context. Exiting..."); std::exit(-1); }
        }

        if (av_frame_make_writable(vstream.cvt_frame) < 0) { std::puts("Could not make the conversion frame writable. Exiting..."); std::exit(-1); }

        write_pixel_data(vstream.cvt_frame->data, vstream.presentation_ts, codec_ctx->width, codec_ctx->height);
        sws_scale(vstream.sws_ctx, (const uint8_t *const *) vstream.cvt_frame->data, vstream.cvt_frame->linesize,
                  0, codec_ctx->height, vstream.frame->data, vstream.frame->linesize);
    }
    else
    {
        write_pixel_data(vstream.frame->data, vstream.presentation_ts - 1, codec_ctx->width, codec_ctx->height);
    }

    vstream.frame->pts = ++vstream.presentation_ts;
}

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    char buf0[AV_TS_MAX_STRING_SIZE], buf1[AV_TS_MAX_STRING_SIZE], buf2[AV_TS_MAX_STRING_SIZE];
    char buf3[AV_TS_MAX_STRING_SIZE], buf4[AV_TS_MAX_STRING_SIZE], buf5[AV_TS_MAX_STRING_SIZE];

    printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           av_ts_make_string(buf0, pkt->pts), av_ts_make_time_string(buf1, pkt->pts, time_base),
           av_ts_make_string(buf2, pkt->dts), av_ts_make_time_string(buf3, pkt->dts, time_base),
           av_ts_make_string(buf4, pkt->duration), av_ts_make_time_string(buf5, pkt->duration, time_base),
           pkt->stream_index);
}

void send_frame(OutputVideoStream& vstream, AVFormatContext *output_ctx)
{
    int ret;

    ret = avcodec_send_frame(vstream.codec_ctx, vstream.frame);
    if (ret < 0) { std::puts("Error sending frame to encoder. Exiting..."); std::exit(-1); }

    while (true)
    {
        ret = avcodec_receive_packet(vstream.codec_ctx, vstream.pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
        else if (ret < 0) { std::puts("Error encoding frame. Exiting..."); std::exit(-1); }

        // rescale timestamp based on timebase
        av_packet_rescale_ts(vstream.pkt, vstream.codec_ctx->time_base, vstream.stream->time_base);
        vstream.pkt->stream_index = vstream.stream->index;

        log_packet(output_ctx, vstream.pkt); // log packet data for user

        ret = av_interleaved_write_frame(output_ctx, vstream.pkt); // write the frame to the media file
        if (ret < 0) { std::puts("Error while writing output packet. Exiting..."); std::exit(-1); }
    }
}

int main()
{
    const char *video_file_name = "video.mp4";

    OutputVideoStream vstream { nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, AVPixelFormat::AV_PIX_FMT_RGB24 };
    const AVOutputFormat *output_fmt;
    AVFormatContext *output_ctx;
    const AVCodec *video_codec;

    int ret;

    avformat_alloc_output_context2(&output_ctx, nullptr, nullptr, video_file_name);
    if (!output_ctx) { std::puts("Could not deduce video type. Exiting..."); return -1; }

    output_fmt = output_ctx->oformat;

    if (output_fmt->video_codec != AV_CODEC_ID_NONE) // check if video codec exists. use it.
    {
        initialize_stream(vstream, output_ctx, &video_codec, output_fmt->video_codec, { .pixel_format = AV_PIX_FMT_RGB24 }); // initialize streams and codec contexts
    }
    initialize_video(vstream, video_codec); // initialize codecs and AVframes.

    av_dump_format(output_ctx, 0, video_file_name, 1);

    if (!(output_fmt->flags & AVFMT_NOFILE)) // open file if necessary
    {
        ret = avio_open(&output_ctx->pb, video_file_name, AVIO_FLAG_WRITE);
        if (ret < 0) { std::puts("Could not open the file. Exiting..."); std::exit(-1); }
    }

    ret = avformat_write_header(output_ctx, nullptr); // write stream header if necessary
    if (ret < 0) { std::puts("Could not write the header to the stream. Exiting..."); std::exit(-1); }

    for (int i = 0; i < 120; ++i)
    {
        write_frame(vstream);
        send_frame(vstream, output_ctx);
    }

    std::printf("DONE PROCESSING\n");

    av_write_trailer(output_ctx); // write trailer to the stream

    free_stream(vstream); // free up resources

    if (!(output_fmt->flags & AVFMT_NOFILE)) // close file
    {
        avio_closep(&output_ctx->pb);
    }

    avformat_free_context(output_ctx);

    return 0;
}
