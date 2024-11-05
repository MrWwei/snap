// // #include "RgaUtils.h"
// #include "data_type.h"
// // #include "im2d.h"
// #include "mk_mediakit.h"
// #include "opencv2/opencv.hpp"
// // #include "rga.h"
// // #include "utils/mpp_decoder.h"
// // #include "utils/mpp_encoder.h"
// // #include "rk_mpi.h"
// #include "xtkj_decoder.h"
// #include "pullFramer.h"
// #include <algorithm>
// #include <chrono>
// #include <condition_variable>
// #include <future>
// #include <queue>
// #include <stack>
// #include <thread>
// #include <unordered_set>
// #include "ffmpeg_decoder.h"

// #ifdef __cplusplus
// extern "C" {
// #endif

// // extern "C" {
// #include "libavcodec/avcodec.h"
// #include "libavformat/avformat.h"
// #include "libavutil/avutil.h"
// #include "libswscale/swscale.h"
// // }
// #include <queue>
// #include <thread>
// // #ifdef __cplusplus
// // extern "C" {
// // #endif

// using namespace xtkj;
// static unsigned char* load_data(FILE* fp, size_t ofst, size_t sz);
// static unsigned char* read_file_data(const char* filename, int* model_size);
// vector<std::mutex>    stack_mutexs_(16);
// vector<std::condition_variable> stack_conds_(16);
// #define MAX_STACK_SIZE 2
// // std::condition_variable cv_;
// // bool resourceReady = false;

// typedef struct
// {
//     // MppDecoder*    decoder;
//     // image_frame_t* frame;
//     // uint64_t       frame_pts{0};
//     // int            instance_index{0};
//     void* puller;
//     void* decoder;
//     bool is_finished = false;
// } Context;
// typedef struct
// {
//     image_frame_t* frame;
//     uint64_t       frame_pts{0};
// } frame_info;

// vector<std::stack<frame_info*>> frame_stacks_(16);

// // vector<std::atomic<frame_info*>> frame_atomics(16);

// class Decoder : public IDecoder {
//   public:
//     Decoder()
//     {
//         // decoder_ = new MppDecoder();
//         // memset(&app_ctx_, 0, sizeof(Context));
//     };
//     ~Decoder()
//     {
//         stop();
//         if (worker_->joinable())
//             worker_->join();

//         // if (decoder_ != nullptr) {
//         //     decoder_->Reset();
//         //     delete (decoder_);
//         //     decoder_ = nullptr;
//         // }
//     };
//     int               init(int decode_thread_num,
//                            int frame_interval = 1,
//                            int timeout_ms     = 200) override;
//     vector<long long> get_frame(string rtsp) override;
//     bool              start(string video_path, string decode_type) override;
//     int               stop() override;
//     bool              stop_ = false;

//   private:
//     // image_frame_t*               frame_;

//     void*                        mat_addr_ = nullptr;
//     // app_context_t                app_ctx_;
//     // MppDecoder*                  decoder_;
//     std::shared_ptr<std::thread> worker_;
//     // std::queue<image_frame_t*>   frame_queue_;

//     // std::condition_variable      cond_;
//     // int                          MAX_QUEUE_SIZE = 1;
//     int timeout_ms_{10};

//     int process_video(
//                       const char*         path,
//                       std::promise<bool>& pro);
                      
// };
// void mpp_decoder_frame_callback(void*    userdata,
//                                 int      width_stride,
//                                 int      height_stride,
//                                 int      width,
//                                 int      height,
//                                 int      format,
//                                 int      fd,
//                                 void*    data,
//                                 uint64_t frame_pts);
// int  Decoder::init(int decode_thread_num, int frame_interval, int timeout_ms)
// {
//     // app_ctx_.frame = new image_frame_t();
//     // int video_type = decode_type;
//     // app_ctx_.instance_index = decode_thread_num;
//     timeout_ms_             = timeout_ms;
//     // frame_atomics[app_ctx_.instance_index].store(nullptr);

//     // app_ctx_.frame = frame_;
//     printf("init ok!\n");
//     return 0;
// }
// bool Decoder::start(string video_path, string decode_type)
// {
//     // this_thread::sleep_for(chrono::milliseconds(200));
//     int decode_type_int = 264;
//     if (decode_type == "H265") {
//         decode_type_int = 265;
//     }
//     // decoder_->Reset();
//     // decoder_->Init(decode_type_int, 30, &app_ctx_);
//     // decoder_->SetCallback(mpp_decoder_frame_callback);
//     // app_ctx_.decoder = decoder_;
//     std::promise<bool> pro;
//     // printf("sdk start open stream\n");

//     // worker_ =
//     //     std::make_shared<std::thread>(&Decoder::process_video, this, 
//     //                                   video_path.c_str(), std::ref(pro));

//     // return pro.get_future().get();
//     return true;
// }
// void API_CALL on_mk_play_event_func(void*       user_data,
//                                     int         err_code,
//                                     const char* err_msg,
//                                     mk_track    tracks[],
//                                     int         track_count);
// void API_CALL on_mk_shutdown_func(void*       user_data,
//                                   int         err_code,
//                                   const char* err_msg,
//                                   mk_track    tracks[],
//                                   int         track_count);

// int Decoder::process_video(
//                            const char*         path,
//                            std::promise<bool>& pro)
// {
//     mk_config config;
//     memset(&config, 0, sizeof(mk_config));
//     config.log_mask = LOG_CONSOLE;
//     mk_env_init(&config);
//     mk_player player = mk_player_create();
//     mk_player_set_on_result(player, on_mk_play_event_func, ctx);
//     pro.set_value(true);
//     mk_player_set_on_shutdown(player, on_mk_shutdown_func, ctx);
//     mk_player_play(player, path);

//     // printf("enter any key to exit\n");
//     // getchar();
//     while (!stop_) {
//         this_thread::sleep_for(chrono::milliseconds(10));
//     }

//     if (player) {
//         printf("release player\n");
//         mk_player_release(player);
//     }
//     return 0;
    
//     return 0;
// }
// int clamp(int value, int min, int max) {
//     if (value < min) {
//         return min;
//     } else if (value > max) {
//         return max;
//     } else {
//         return value;
//     }
// }
// cv::Mat YV12ToBGR24_OpenCV(unsigned char* pYUV,int width,int height)
// {
//     unsigned char* pBGR24 = (unsigned char*)malloc(width*height*3);

//     if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL)
//         return cv::Mat();
//     cv::Mat dst(height,width,CV_8UC3,pBGR24);
//     cv::Mat src(height + height/2,width,CV_8UC1,pYUV);
//     cv::cvtColor(src,dst,cv::COLOR_YUV2BGR_I420);
//     cv::Mat bgr_mat;
//     dst.copyTo(bgr_mat);
//     if(!pBGR24){
//         free(pBGR24);
//         pBGR24 = nullptr;
//     }


//     // cv::imwrite("out.jpg", dst);
//     // exit(0);
//     return bgr_mat;
// }
// void onGetFrame(const FrameData::Ptr& frame, void* userData1, bool& is_finished)
// {
//     // std::cout <<frame<<std::endl;
//     auto decoderPtr = static_cast<std::shared_ptr<VideoDecoder>*>(userData1);
//     std::shared_ptr<VideoDecoder> decoder = *decoderPtr;

//     // auto displayerPtr = static_cast<std::shared_ptr<YUVDisplayer>*>(userData2);
//     // std::shared_ptr<YUVDisplayer> displayer = *displayerPtr;
//     if (decoder) {
//         int32_t pixel_width = 0;
//         int32_t pixel_height = 0;
//         int32_t pixel_format = 0;
//         auto start = std::chrono::steady_clock::now();
//         auto dstYUV = decoder->decode(frame->data(), frame->size(), pixel_width, pixel_height, pixel_format);
//         if (dstYUV == nullptr) {
//             std::cerr << "decode error" << std::endl;
//             return;
//         }

//         // Allocate memory for YUV and RGB data
//         int width = pixel_width;
//         int height = pixel_height;
//         cv::Mat bgr_mat = YV12ToBGR24_OpenCV(dstYUV, width, height);
//         static int idx = 0;
//         // 转换为 RGB 图像
//         cv::imwrite(std::to_string(idx++)+"out.jpg", bgr_mat);
//         // std::cout << "format :" << pixel_format << std::endl;
//         // exit(0);
//         auto end = std::chrono::steady_clock::now();
//         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//         // std::cout<<"width:"<<pixel_width<<" height:"<<pixel_height<<" format:"<<pixel_format<<std::endl;
//         std::cout << "decode cost " << duration << " ms" << std::endl;
// // #if 1
//         // if (displayer) {
//         //     start = end;
//         //     displayer->display(dstYUV, pixel_width, pixel_height, pixel_format);
//         //     end = std::chrono::steady_clock::now();
//         //     duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//         //     std::cout << "display cost " << duration << " ms" << std::endl;
//         // }
// // #endif
//         free(dstYUV);
//         dstYUV = nullptr;
//     }
// }
// vector<long long> Decoder::get_frame(string rtsp)
// {
//     mk_config config;
//     config.ini = NULL;
//     config.ini_is_path = 0;
//     config.log_level = 0;
//     config.log_mask = LOG_CONSOLE;
//     config.ssl = NULL;
//     config.ssl_is_path = 1;
//     config.ssl_pwd = NULL;
//     config.thread_num = 0;
//     std::string url = rtsp;
    
//     mk_env_init(&config);
//     auto player = mk_player_create();
//     auto puller = PullFramer::CreateShared();
//     // auto displayer = std::make_shared<YUVDisplayer>(url);
//     std::shared_ptr<VideoDecoder> decoder = nullptr;
//     Context ctx;
//     memset(&ctx, 0, sizeof(Context));
//     ctx.puller = static_cast<void*>(&puller);
//     ctx.decoder = static_cast<void*>(&decoder);
//     bool is_finished = false;
//     puller->setOnGetFrame(onGetFrame, static_cast<void*>(&decoder), is_finished);
//     mk_player_set_on_result(player, on_mk_play_event_func, &ctx);
//     mk_player_set_on_shutdown(player, on_mk_shutdown_func, NULL);
//     mk_player_set_option(player, "rtp_type", "0");
//     mk_player_set_option(player, "protocol_timeout_ms", "5000");
//     mk_player_play(player, url.c_str());
    
//     // std::cout << "Hello World!\n";
//     // log_info("enter any key to exit");
//     getchar();
//     if (player) {
//     mk_player_release(player);
//     //
//     }
//     return {};
// }

// // void mpp_decoder_frame_callback(void*    userdata,
// //                                 int      width_stride,
// //                                 int      height_stride,
// //                                 int      width,
// //                                 int      height,
// //                                 int      format,
// //                                 int      fd,
// //                                 void*    data,
// //                                 uint64_t frame_pts)
// // {
// //     app_context_t* ctx = (app_context_t*)userdata;

// //     int        ret         = 0;
// //     static int frame_index = 0;
// //     frame_index++;

// //     void*        mpp_frame      = NULL;
// //     int          mpp_frame_fd   = 0;
// //     void*        mpp_frame_addr = NULL;
// //     int          enc_data_size;
// //     rga_buffer_t src;
// //     rga_buffer_t dst;
// //     im_rect      src_rect;
// //     im_rect      dst_rect;
// //     memset(&src_rect, 0, sizeof(src_rect));
// //     memset(&dst_rect, 0, sizeof(dst_rect));
// //     memset(&src, 0, sizeof(src));
// //     memset(&dst, 0, sizeof(dst));
// //     // std::lock_guard<std::mutex> lock(frame_lock_);
// //     // std::unique_lock<std::mutex> lock(frame_lock_);
// //     if (ctx->frame->virt_addr == nullptr)
// //         ctx->frame->virt_addr =
// //             malloc(width * height * get_bpp_from_format(RK_FORMAT_BGRA_8888));
// //     memset(ctx->frame->virt_addr, 0,
// //            width * height * get_bpp_from_format(RK_FORMAT_BGRA_8888));
// //     ctx->frame->height = height;
// //     ctx->frame->width  = width;
// //     ctx->frame_pts += frame_pts;
// //     src = wrapbuffer_virtualaddr((void*)data, width, height,
// //                                  RK_FORMAT_YCbCr_420_SP, width_stride,
// //                                  height_stride);
// //     dst = wrapbuffer_virtualaddr((void*)ctx->frame->virt_addr, width, height,
// //                                  RK_FORMAT_BGRA_8888);
// //     ret = imcheck(src, dst, src_rect, dst_rect);
// //     imcopy(src, dst);
// //     frame_info* frame_info_ptr = new frame_info();
// //     frame_info_ptr->frame      = new image_frame_t();
// //     frame_info_ptr->frame->virt_addr =
// //         malloc(width * height * get_bpp_from_format(RK_FORMAT_BGRA_8888));

// //     frame_info_ptr->frame_pts     = ctx->frame_pts;
// //     frame_info_ptr->frame->height = height;
// //     frame_info_ptr->frame->width  = width;
// //     memcpy(frame_info_ptr->frame->virt_addr, ctx->frame->virt_addr,
// //            ctx->frame->height * ctx->frame->width *
// //                get_bpp_from_format(RK_FORMAT_BGRA_8888));
// //     // std::cout << "sdk start push data" << std::endl;
// //     // std::this_thread::sleep_for(std::chrono::milliseconds(500));

// //     {
// //         std::unique_lock<std::mutex> lock(stack_mutexs_[ctx->instance_index]);
// //         int                          ins_index = ctx->instance_index;
// //         // stack_conds_[ctx->instance_index].wait(lock,[ins_index]{return
// //         // frame_stacks_[ins_index].size() < MAX_STACK_SIZE;});
// //         while (frame_stacks_[ctx->instance_index].size() >= MAX_STACK_SIZE) {
// //             frame_info* frame_info_ptr_del =
// //                 frame_stacks_[ctx->instance_index].top();
// //             frame_stacks_[ctx->instance_index].pop();
// //             if (frame_info_ptr_del->frame->virt_addr != nullptr) {
// //                 free(frame_info_ptr_del->frame->virt_addr);
// //                 frame_info_ptr_del->frame->virt_addr = nullptr;
// //             }
// //             if (frame_info_ptr_del->frame != nullptr) {
// //                 delete frame_info_ptr_del->frame;
// //                 frame_info_ptr_del->frame = nullptr;
// //             }
// //             if (frame_info_ptr_del != nullptr) {
// //                 delete frame_info_ptr_del;
// //                 frame_info_ptr_del = nullptr;
// //             }
// //         }
// //         frame_stacks_[ctx->instance_index].push(frame_info_ptr);
// //         stack_conds_[ctx->instance_index].notify_all();
// //         // std::cout << "sdk push data ok" << std::endl;
// //     }
// // }

// void API_CALL on_track_frame_out(void* user_data, mk_frame frame)
// {
//     Context* ctx = (Context*)user_data;
//     auto pullerPtr = static_cast<std::shared_ptr<PullFramer>*>(ctx->puller);
//     std::shared_ptr<PullFramer> puller = *pullerPtr;
//     if (puller) {
//         bool is_finished = false;
// 		puller->onFrame(frame, is_finished);
// 	}



//     //   printf("on_track_frame_out ctx=%p\n", ctx);
//     // const char* data = mk_frame_get_data(frame);
//     // size_t      size = mk_frame_get_data_size(frame);
    
//     // API_EXPORT uint64_t API_CALL mk_frame_get_pts(mk_frame frame);
//     // uint16_t pts_time = mk_frame_get_pts(frame) * 1000;  // 微秒
//     // printf("pts time:%llu\n", pts_time);
//     //   printf("decoder=%p\n", ctx->decoder);
//     // ctx->decoder->Decode((uint8_t*)data, size, pts_time, pts_time);
//     // ffmpeg

//     // VideoDecoder ffmpeg_decoder = VideoDecoder(0);//h265
//     // int pix_w,pix_h=0;
//     // int32_t format;
//     // printf("22222222222222\n");
//     // static int count = 0;
//     // count++;
//     // if (count < 200)return;
//     // ffmpeg_decoder.decode((uint8_t*)data,size, pix_w,pix_h,format);


    

// }

// void API_CALL on_mk_play_event_func(void*       user_data,
//                                     int         err_code,
//                                     const char* err_msg,
//                                     mk_track    tracks[],
//                                     int         track_count)
// {
//     // Context* ctx = (Context*)user_data;
//     if (err_code == 0) {
//         //success
//         log_debug("play success!");
//         int i;
//         for (i = 0; i < track_count; ++i) {
//             if (mk_track_is_video(tracks[i])) {
//                 Context* ctx = (Context*)user_data;
//                 auto decoderPtr = static_cast<std::shared_ptr<VideoDecoder>*>(ctx->decoder);
//                 std::shared_ptr<VideoDecoder> decoder = *decoderPtr;
//                 if (decoder == nullptr) {
// 					decoder = std::make_shared<VideoDecoder>(mk_track_codec_id(tracks[i]));
// 					*decoderPtr = decoder;
// 				}

//                 log_info("got video track: %s", mk_track_codec_name(tracks[i]));
//                 //ctx->video_decoder = mk_decoder_create(tracks[i], 0);
//                 //mk_decoder_set_cb(ctx->video_decoder, on_frame_decode, user_data);
//                 //监听track数据回调
//                 mk_track_add_delegate(tracks[i], on_track_frame_out, user_data);
//             }
//         }
//     }
//     else {
//         log_warn("play failed: %d %s", err_code, err_msg);
//     }
// }

// void API_CALL on_mk_shutdown_func(void*       user_data,
//                                   int         err_code,
//                                   const char* err_msg,
//                                   mk_track    tracks[],
//                                   int         track_count)
// {
//     printf("play interrupted: %d %s", err_code, err_msg);
// }
// int Decoder::stop()
// {
//     stop_ = true;
//     this_thread::sleep_for(chrono::milliseconds(200));
//     if (worker_->joinable())
//             worker_->join();

//     // if (decoder_ != nullptr) {
//     //     decoder_->Reset();
//     //     delete (decoder_);
//     //     decoder_ = nullptr;
//     // }
//     return 0;
// }

// static unsigned char* load_data(FILE* fp, size_t ofst, size_t sz)
// {
//     unsigned char* data;
//     int            ret;

//     data = NULL;

//     if (NULL == fp) {
//         return NULL;
//     }

//     ret = fseek(fp, ofst, SEEK_SET);
//     if (ret != 0) {
//         printf("blob seek failure.\n");
//         return NULL;
//     }

//     data = (unsigned char*)malloc(sz);
//     if (data == NULL) {
//         printf("buffer malloc failure.\n");
//         return NULL;
//     }
//     ret = fread(data, 1, sz, fp);
//     return data;
// }

// static unsigned char* read_file_data(const char* filename, int* model_size)
// {
//     FILE*          fp;
//     unsigned char* data;

//     fp = fopen(filename, "rb");
//     if (NULL == fp) {
//         printf("Open file %s failed.\n", filename);
//         return NULL;
//     }

//     fseek(fp, 0, SEEK_END);
//     int size = ftell(fp);

//     data = load_data(fp, 0, size);

//     fclose(fp);

//     *model_size = size;
//     return data;
// }
// IDecoder* xtkj::createDecoder()
// {
//     return new Decoder();
// }

// void xtkj::releaseDecoder(IDecoder* pIDecoder)
// {
//     delete pIDecoder;
//     pIDecoder = nullptr;
// }
// #ifdef __cplusplus
// }
// #endif
