#include <iostream>

#include "opencv2/opencv.hpp"
#include "thread_pool.h"
#include "xtkj_capture.h"
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

void run(xtkj::ICaptureSnap* capture, string rtsp)
{
    while (true) {
        capture->snap_shoot(rtsp);
    }
}

int get_rtsps(string rtsp_file, std::vector<std::string>& lines)
{
    std::ifstream infile(rtsp_file);  // 打开输入文件
    std::string   line;

    if (!infile) {  // 检查文件是否成功打开
        std::cerr << "无法打开文件" << std::endl;
        return 1;
    }

    while (std::getline(infile, line)) {  // 按行读取文件
        if (!line.empty()) {              // 检查行是否非空
            lines.push_back(line);        // 将非空行添加到向量中
        }
    }

    infile.close();  // 关闭文件
}
int main(int argc, char* argv[])
{
    string         rtsp_file = argv[1];
    vector<string> rtsps;
    get_rtsps(rtsp_file, rtsps);
    xtkj::ICaptureSnap* capture = xtkj::createObj();

    capture->init();
    string rtsp_url =
        "rtsp://admin:xtkj12345@192.168.1.86:554/h264/ch33/main/av_stream";

    thread th1 = thread(run, capture, rtsp_url);
    thread th2 = thread(run, capture, rtsp_url);
    thread th3 = thread(run, capture, rtsp_url);
    th1.join();
    th2.join();
    th3.join();
    return 0;
}