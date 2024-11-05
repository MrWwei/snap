#include "xtkj_capture.h"
#include "data_type.h"
#include "ffmpeg_decoder.h"
#include "mk_mediakit.h"
#include "opencv2/opencv.hpp"
#include "pullFramer.h"
#include "thread_pool.h"
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <future>
#include <queue>
#include <stack>
#include <thread>
#include <unordered_set>

#ifdef __cplusplus
extern "C" {
#endif

#include <queue>
#include <thread>
typedef struct
{
    void*   puller;
    void*   decoder;
    bool    is_finished = false;
    cv::Mat image_job;
} Context;
class CaptureSnap : public xtkj::ICaptureSnap {
  public:
    CaptureSnap(){};
    int     init(int thread_num, int queue_size, int time_out) override;
    cv::Mat snap_shoot(string rtsp_url) override;
    int     cur_task_num() override;
    ~CaptureSnap(){};

  private:
    ThreadPool pool_;
    int        time_out_{0};
};

int get_cpu_num()
{
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string   line;
    int           coreCount = 0;

    while (std::getline(cpuinfo, line)) {
        if (line.find("processor") != std::string::npos) {
            coreCount++;
        }
    }
    return coreCount;
}
cv::Mat do_shoot(const char* rtsp, int timeout);

int CaptureSnap::cur_task_num()
{
    return pool_.get_cur_task_num();
}
int CaptureSnap::init(int thread_num, int queue_size, int time_out)
{
    thread_num = get_cpu_num() * 2;
    queue_size = thread_num * 2;
    pool_.init(thread_num, queue_size, time_out);
    time_out_ = time_out;
}
cv::Mat CaptureSnap::snap_shoot(string rtsp_url)
{
    auto res = pool_.enqueue(do_shoot, rtsp_url.c_str(), time_out_);
    return res.get();
}

void API_CALL on_mk_play_event_func(void*       user_data,
                                    int         err_code,
                                    const char* err_msg,
                                    mk_track    tracks[],
                                    int         track_count);
void API_CALL on_mk_shutdown_func(void*       user_data,
                                  int         err_code,
                                  const char* err_msg,
                                  mk_track    tracks[],
                                  int         track_count);

int clamp(int value, int min, int max)
{
    if (value < min) {
        return min;
    }
    else if (value > max) {
        return max;
    }
    else {
        return value;
    }
}
bool YV12ToBGR24_OpenCV(unsigned char* pYUV,
                        int            width,
                        int            height,
                        cv::Mat&       image_job)
{
    unsigned char* pBGR24 = (unsigned char*)malloc(width * height * 3);

    if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL) {
        if (pBGR24) {
            free(pBGR24);
            pBGR24 = nullptr;
        }
        return false;
    }

    cv::Mat dst(height, width, CV_8UC3, pBGR24);
    cv::Mat src(height + height / 2, width, CV_8UC1, pYUV);
    cv::cvtColor(src, dst, cv::COLOR_YUV2BGR_I420);

    dst.copyTo(image_job);
    if (pBGR24) {
        free(pBGR24);
        pBGR24 = nullptr;
    }
    return true;
}
void onGetFrame(const FrameData::Ptr& frame,
                void*                 userData1,
                cv::Mat&              image_job)
{
    auto decoderPtr = static_cast<std::shared_ptr<VideoDecoder>*>(userData1);
    std::shared_ptr<VideoDecoder> decoder = *decoderPtr;

    if (decoder) {
        int32_t pixel_width  = 0;
        int32_t pixel_height = 0;
        int32_t pixel_format = 0;
        // auto start = std::chrono::steady_clock::now();
        auto dstYUV = decoder->decode(frame->data(), frame->size(), pixel_width,
                                      pixel_height, pixel_format);
        if (dstYUV == nullptr) {
            std::cerr << "decode error" << std::endl;
            return;
        }

        // Allocate memory for YUV and RGB data
        int  width  = pixel_width;
        int  height = pixel_height;
        bool ret    = YV12ToBGR24_OpenCV(dstYUV, width, height, image_job);

        free(dstYUV);
        dstYUV = nullptr;
    }
}
cv::Mat do_shoot(const char* rtsp, int timeout)
{
    mk_config config;
    config.ini         = NULL;
    config.ini_is_path = 0;
    config.log_level   = 0;
    config.log_mask    = LOG_CONSOLE;
    config.ssl         = NULL;
    config.ssl_is_path = 1;
    config.ssl_pwd     = NULL;
    config.thread_num  = 0;
    std::string url    = rtsp;

    mk_env_init(&config);
    auto                          player  = mk_player_create();
    auto                          puller  = PullFramer::CreateShared();
    std::shared_ptr<VideoDecoder> decoder = nullptr;
    Context                       ctx;
    memset(&ctx, 0, sizeof(Context));
    ctx.puller  = static_cast<void*>(&puller);
    ctx.decoder = static_cast<void*>(&decoder);
    puller->setOnGetFrame(onGetFrame, static_cast<void*>(&decoder),
                          ctx.image_job);
    mk_player_set_on_result(player, on_mk_play_event_func, &ctx);
    mk_player_set_on_shutdown(player, on_mk_shutdown_func, NULL);
    mk_player_set_option(player, "rtp_type", "0");
    mk_player_set_option(player, "protocol_timeout_ms", "5000");
    mk_player_set_option(player, "wait_track_ready", "false");
    mk_player_play(player, rtsp);

    // getchar();
    int times = timeout / 10;
    while (ctx.image_job.empty() && times > 0) {
        this_thread::sleep_for(chrono::milliseconds(10));
        times--;
    }
    if (player) {
        printf("release player\n");
        mk_player_release(player);
    }
    if (ctx.image_job.empty()) {
        return cv::Mat();
    }
    return ctx.image_job;
}

void API_CALL on_track_frame_out(void* user_data, mk_frame frame)
{
    Context* ctx = (Context*)user_data;
    // pullerPtr:智能指针的指针
    auto pullerPtr = static_cast<std::shared_ptr<PullFramer>*>(ctx->puller);
    std::shared_ptr<PullFramer> puller = *pullerPtr;
    if (puller) {
        puller->onFrame(frame, ctx->image_job);
    }
}

void API_CALL on_mk_play_event_func(void*       user_data,
                                    int         err_code,
                                    const char* err_msg,
                                    mk_track    tracks[],
                                    int         track_count)
{
    // Context* ctx = (Context*)user_data;
    if (err_code == 0) {
        // success
        log_debug("play success!");
        int i;
        for (i = 0; i < track_count; ++i) {
            if (mk_track_is_video(tracks[i])) {
                Context* ctx = (Context*)user_data;
                auto     decoderPtr =
                    static_cast<std::shared_ptr<VideoDecoder>*>(ctx->decoder);
                std::shared_ptr<VideoDecoder> decoder = *decoderPtr;
                if (decoder == nullptr) {
                    decoder = std::make_shared<VideoDecoder>(
                        mk_track_codec_id(tracks[i]));
                    *decoderPtr = decoder;
                }

                log_info("got video track: %s", mk_track_codec_name(tracks[i]));
                // ctx->video_decoder = mk_decoder_create(tracks[i], 0);
                // mk_decoder_set_cb(ctx->video_decoder, on_frame_decode,
                // user_data); 监听track数据回调
                mk_track_add_delegate(tracks[i], on_track_frame_out, user_data);
            }
        }
    }
    else {
        log_warn("play failed: %d %s", err_code, err_msg);
    }
}

void API_CALL on_mk_shutdown_func(void*       user_data,
                                  int         err_code,
                                  const char* err_msg,
                                  mk_track    tracks[],
                                  int         track_count)
{
    printf("play interrupted: %d %s", err_code, err_msg);
}
xtkj::ICaptureSnap* xtkj::createObj()
{
    return new CaptureSnap();
}

void xtkj::releaseObj(ICaptureSnap* pObj)
{
    delete pObj;
    pObj = nullptr;
}
#ifdef __cplusplus
}
#endif
