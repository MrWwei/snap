/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class cn_xtkj_jni_capture_RtspCapture */

#ifndef _Included_cn_xtkj_jni_capture_RtspCapture
#    define _Included_cn_xtkj_jni_capture_RtspCapture
#    ifdef __cplusplus
extern "C" {
#    endif
/*
 * Class:     cn_xtkj_jni_capture_RtspCapture
 * Method:    initResource
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
               Java_cn_xtkj_jni_capture_RtspCapture_initResource(JNIEnv*,
                                                                 jobject,
                                                                 jint,
                                                                 jint,
                                                                 jint);

/*
 * Class:     cn_xtkj_jni_capture_RtspCapture
 * Method:    snap
 * Signature: (Ljava/lang/String;I)[B
 */
JNIEXPORT jbyteArray JNICALL Java_cn_xtkj_jni_capture_RtspCapture_snap(JNIEnv*,
                                                                       jobject,
                                                                       jstring,
                                                                       jint);

/*
 * Class:     cn_xtkj_jni_capture_RtspCapture
 * Method:    release
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_cn_xtkj_jni_capture_RtspCapture_release(JNIEnv*,
                                                                    jobject,
                                                                    jint);

#    ifdef __cplusplus
}
#    endif
#endif
