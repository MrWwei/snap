
/*
 *┌────────────────────────────────────────────────┐
 *│　author：wtwei
 *│　version：1.0
 *│　create time：2023/01/30 16:42
 *└────────────────────────────────────────────────┘
 */
#ifndef XTKJ_CAPTURE_H
#define XTKJ_CAPTURE_H
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
using namespace std;

namespace xtkj {

class ICaptureSnap {
  public:
    /**
     * @description: 初始化
     * @param {int} decode_thread_num 解码线程数
     * @param {int} timeout_ms 超时时间
     * @return {*}
     */
    virtual int
    init(int thread_num = 64, int queue_size = 8, int timeout_ms = 1000) = 0;

    virtual cv::Mat snap_shoot(string rtsp_url) = 0;

    virtual int cur_task_num() = 0;

    virtual ~ICaptureSnap() noexcept = default;
};
extern "C" ICaptureSnap* createObj();
extern "C" void          releaseObj(ICaptureSnap* pObj);
}  // namespace xtkj

#endif