// #include "RgaUtils.h"
#include "data_type.h"
// #include "im2d.h"
#include "im2d.h"
#include "mk_mediakit.h"
#include "opencv2/opencv.hpp"
// #include "rga.h"
// #include "utils/mpp_decoder.h"
// #include "utils/mpp_encoder.h"
// #include "rk_mpi.h"
#include "RgaUtils.h"
#include "mpp_decoder.h"
#include "rga.h"
#include "xtkj_capture.h"
#include <algorithm>
#include <chrono>
#include <condition_variable>
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
namespace xtkj {
    typedef struct
    {
        MppDecoder*    decoder;
        image_frame_t* frame;
        uint64_t       frame_pts{0};
        int            instance_index{0};
        cv::Mat        image_job;
        int            frame_count{0};
        int            decode_type{0};
    } app_context_t;
    typedef struct
    {
        image_frame_t* frame;
        uint64_t       frame_pts{0};
    } frame_info;

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
    void          mpp_decoder_frame_callback(void*    userdata,
                                             int      width_stride,
                                             int      height_stride,
                                             int      width,
                                             int      height,
                                             int      format,
                                             int      fd,
                                             void*    data,
                                             uint64_t frame_pts,
                                             size_t   data_size);

    cv::Mat snap_shoot(const char* rtsp, int timeout)
    {
        mk_config config;
        memset(&config, 0, sizeof(mk_config));

        config.ini         = NULL;
        config.ini_is_path = 0;
        config.log_level   = 0;
        config.log_mask    = LOG_CONSOLE;
        config.ssl         = NULL;
        config.ssl_is_path = 1;
        config.ssl_pwd     = NULL;
        config.thread_num  = 0;
        app_context_t ctx;
        memset(&ctx, 0, sizeof(app_context_t));
        // config.log_mask = LOG_CONSOLE;
        mk_env_init(&config);
        mk_player player = mk_player_create();
        mk_player_set_on_result(player, on_mk_play_event_func, &ctx);
        mk_player_set_on_shutdown(player, on_mk_shutdown_func, &ctx);
        mk_player_set_option(player, "rtp_type", "0");
        mk_player_set_option(player, "protocol_timeout_ms", "10000");
        mk_player_set_option(player, "media_timeout_ms", "10000");

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
    void* YV12ToBGR24_OpenCV(unsigned char* pYUV, int width, int height)
    {
        unsigned char* pBGR24 = (unsigned char*)malloc(width * height * 3);

        if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL) {
            if (pBGR24 != nullptr) {
                free(pBGR24);
                pBGR24 = nullptr;
            }
            return nullptr;
        }
        cv::Mat dst(height, width, CV_8UC3, pBGR24);
        cv::Mat src(height + height / 2, width, CV_8UC1, pYUV);
        cv::cvtColor(src, dst, cv::COLOR_YUV420sp2RGB);

        return pBGR24;
    }
    void mpp_decoder_frame_callback(void*    userdata,
                                    int      width_stride,
                                    int      height_stride,
                                    int      width,
                                    int      height,
                                    int      format,
                                    int      fd,
                                    void*    data,
                                    uint64_t frame_pts,
                                    size_t   data_size)
    {
        app_context_t* ctx = (app_context_t*)userdata;
        ctx->frame_pts += frame_pts;
        void* mdata = nullptr;

        mdata          = YV12ToBGR24_OpenCV((unsigned char*)data, width_stride,
                                   height_stride);
        ctx->image_job = cv::Mat(height_stride, width_stride, CV_8UC3, mdata);
        return;
    }
    void API_CALL on_track_frame_out(void* user_data, mk_frame frame)
    {
        app_context_t* ctx = (app_context_t*)user_data;
        //   printf("on_track_frame_out ctx=%p\n", ctx);
        const char* data = mk_frame_get_data(frame);
        size_t      size = mk_frame_get_data_size(frame);
        // API_EXPORT uint64_t API_CALL mk_frame_get_pts(mk_frame frame);
        uint16_t pts_time = mk_frame_get_pts(frame) * 1000;  // 微秒
        ctx->decoder->multi_dec_simple((uint8_t*)data, size, pts_time, 0,
                                       ctx->instance_index);
    }

    void API_CALL on_mk_play_event_func(void*       user_data,
                                        int         err_code,
                                        const char* err_msg,
                                        mk_track    tracks[],
                                        int         track_count)
    {
        app_context_t* ctx = (app_context_t*)user_data;
        if (err_code == 0) {
            // success
            printf("play success!");
            int i;
            for (i = 0; i < track_count; ++i) {
                if (mk_track_is_video(tracks[i])) {
                    log_info("got video track: %s",
                             mk_track_codec_name(tracks[i]));
                    int32_t codec_id = mk_track_codec_id(tracks[i]);
                    ctx->decode_type = codec_id;
                    ctx->decoder     = new MppDecoder();
                    ctx->decoder->Reset();
                    ctx->decoder->SetCallback(mpp_decoder_frame_callback);
                    ctx->decoder->Init(codec_id, 30, ctx);

                    // 监听track数据回调
                    mk_track_add_delegate(tracks[i], on_track_frame_out,
                                          user_data);
                }
            }
        }
        else {
            printf("play failed: %d %s", err_code, err_msg);
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
}  // namespace xtkj
#ifdef __cplusplus
}
#endif
