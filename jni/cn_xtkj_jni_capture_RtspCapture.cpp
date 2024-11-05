#include "cn_xtkj_jni_capture_RtspCapture.h"
#include "xtkj_capture.h"
#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using namespace std;
// std::unordered_map<int, xtkj::Camera::ICapture*> caps;

// int                               netNum = 1;
xtkj::ICaptureSnap* capture = xtkj::createObj();

JNIEXPORT jint JNICALL
               Java_cn_xtkj_jni_capture_RtspCapture_initResource(JNIEnv* env,
                                                                 jobject thisObj,
                                                                 jint    thread_pool_size,
                                                                 jint    task_queue_size,
                                                                 jint    timeOutMilliseconds)
{
    capture->init(4, 8, 5000);
    return 1;
}

JNIEXPORT jbyteArray JNICALL
                     Java_cn_xtkj_jni_capture_RtspCapture_snap(JNIEnv* env,
                                                               jobject thisObj,
                                                               jstring rtsp_url,
                                                               jint    timeOutMilliseconds)
{
    std::vector<uchar> rlt_data;
    const char*        rtsp_uri_ = env->GetStringUTFChars(rtsp_url, NULL);

    cv::Mat frame = capture->snap_shoot(rtsp_uri_);
    if (frame.empty()) {
        env->ReleaseStringUTFChars(rtsp_url, rtsp_uri_);
        frame.release();
        return NULL;
    }

    cv::imencode(".jpg", frame, rlt_data);
    frame.release();

    jbyteArray byte_arr = env->NewByteArray(rlt_data.size());
    env->SetByteArrayRegion(byte_arr, 0, rlt_data.size(), (jbyte*)&rlt_data[0]);
    env->ReleaseStringUTFChars(rtsp_url, rtsp_uri_);

    return byte_arr;
}

JNIEXPORT jint JNICALL Java_cn_xtkj_jni_capture_RtspCapture_release(JNIEnv* env,
                                                                    jobject,
                                                                    jint handle)
{
    return 0;
}
