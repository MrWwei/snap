#ifndef DATA_TYPE_H  // 防止头文件被重复包含
#define DATA_TYPE_H

typedef struct
{
    int   width;
    int   height;
    int   width_stride;
    int   height_stride;
    int   format;
    void* virt_addr   = nullptr;
    void* packet_data = nullptr;
    int   packet_size = 0;
    int   fd;
} image_frame_t;
#endif