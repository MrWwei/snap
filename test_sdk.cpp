#include "xtkj_capture.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>


extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
}
using namespace cv;
using namespace std;
int main1(){
	AVCodecContext*    codecContext_;
    AVFormatContext*   format_ctx_;
    AVCodecParameters* codecParameters_;
     avformat_network_init();
     format_ctx_ = avformat_alloc_context();
    if (!format_ctx_) {
        std::cerr << "彗| 沾U佈~F轅~M AVFormatContext" << std::endl;
        return -1;
    }
    const char* rtsp_uri = "rtsp://admin:admin123@192.168.1.65:554/h264/ch33/main/av_stream";
    if (avformat_open_input(&format_ctx_,rtsp_uri, nullptr,
                            nullptr) != 0) {
	    printf("failed\n");
        std::cerr << "彗| 沾U彉~S廾@ RTSP 派A" << std::endl;
        return -1;
    }
    printf("success\n");
    return 0;
}
double __get_us(struct timeval t) { return (t.tv_sec * 1000000 + t.tv_usec); }
void threadFunc(string rtsp, int do_write, string out_dir){
    
	int idx = 0;
    struct timeval start_time, stop_time;
	while(true){
        gettimeofday(&start_time, NULL);
		Mat image = xtkj::snap_shoot(rtsp.c_str(),0);
        gettimeofday(&stop_time, NULL);
        printf("once snap use %f ms\n", (__get_us(stop_time) - __get_us(start_time)) / 1000);

        // std::cout << rtsp << ":get frame ok" << std::endl; 
		if(do_write && !image.empty()){
			cv::imwrite(out_dir+"/test" + std::to_string(idx++) + ".jpg", image);
		} 
        // delete[] capture;
        // capture = nullptr;
        // auto end = std::chrono::system_clock::now();
        // int  delay_infer =
        //     std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
        //         .count();
        // std::cout << "-------------------once------------------------ cost:"
        //           << delay_infer << " ms" << std::endl;

        
        
	}
}

int32_t opencvDirAndMkdir(const char* pathname)
{
	int ret=0;
	DIR * mydir =NULL;
	mydir=opendir(pathname); //打开目录
	if(mydir==NULL)
	{
		// std::cout<<version_name_string<<"--pathname="<<pathname<<std::endl;
		ret = mkdir(pathname,0755);   //创建目录
		if(ret!=0)
		{
			// std::cout<<version_name_string<<"--opendir_ret="<<ret<<std::endl;
			// std::cout<<version_name_string<<"--mkdir fialed."<<std::endl;
			return -1;
		}
		// std::cout<<version_name_string<<"--mkdir sucess."<<std::endl;
	}
	else
	{
		std::cout<<pathname << "--dir exist."<<std::endl;
	}
	closedir(mydir);//关闭目录
	return ret;
}
int main(int argc, char* argv[])
{
    int       do_write     = std::atoi(argv[1]);
    std::string filename = argv[2]; // 替换为你的文件路径
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return 1;
    }

    std::string line;
    int lineNumber = 0;

    // 逐行读取文件内容
    std::vector<string> rtsps;
    while (std::getline(inputFile, line)) {
        // 在这里可以对每一行的 RTSP 地址进行处理，例如存储到一个容器中或进行其他操作
        std::cout << "RTSP Address at line " << lineNumber + 1 << ": " << line << std::endl;
        lineNumber++;
        rtsps.push_back(line);
    }

    inputFile.close();
    int factory = 1;
    // string    rtsp_uri1    = argv[2];
    // string    rtsp_uri2    = argv[3];
    //string rtsp_uri = "rtsp://admin:jjzd0514@32.146.122.85:554/cam/realmonitor?channel=1&subtype=0";
    
    std::vector<std::thread> threads;
    threads.resize(lineNumber);
    for(int i = 0;i<lineNumber;i++){
        string outs = "outs" + std::to_string(i);
        opencvDirAndMkdir(outs.c_str());

        threads[i] = std::thread(threadFunc,rtsps[i], do_write, outs);

    }
    for(int i = 0;i<lineNumber;i++){
        threads[i].join();
    }
    // ICapture* capture1     = createCapture();
    // ICapture* capture2     = createCapture();
    // int       ret1         = capture1->initSdk(factory, 0);
    // int       ret2         = capture2->initSdk(factory, 0);
    // int       channel_num = 33;
    // int       stream_type = 0;
    // ret = capture->loginCamera("192.168.1.65", 8000, "admin",
    // "admin123",channel_num,stream_type); ret =
    // capture->loginCamera("192.168.1.220", 8000, "admin",
    // "admin123",channel_num, stream_type); ret =
    // capture->loginCamera("rtsp://admin:admin123@192.168.1.220:554/Streaming/Channels/201");
    // ret =
    // capture->loginCamera("rtsp://admin:admin123@192.168.1.65:554/h264/ch33/main/av_stream");
    // ret1 = capture1->loginCamera(rtsp_uri1);
    // ret2 = capture2->loginCamera(rtsp_uri2);
    // ret = capture->loginCamera("192.168.1.85", 37777, "admin", "xtkj12345",
    // channel_num, stream_type); ret =
    // capture->loginCamera("rtsp://admin:xtkj12345@192.168.1.85:554/cam/realmonitor?channel=1&subtype=1");
    // if (ret1 < 0) {
    //     std::cout << "1 login failed!" << std::endl;
    //     return 0;
    // }
    // if (ret2 < 0) {
    //     std::cout << "2 login failed!" << std::endl;
    //     return 0;
    // }
    int idx = 0;
    // string out_dir1 = "outs1";
    // string out_dir2 = "outs2";
    // std::thread t1(threadFunc, std::ref(capture1),rtsp_uri1, do_write, out_dir1);
    // std::thread t2(threadFunc, std::ref(capture2),rtsp_uri2, do_write, out_dir2);

    // // 等待线程执行完毕
    // t1.join();
    // t2.join();

    // while (true) {
    //     auto start = std::chrono::system_clock::now();

    //     cv::Mat image = capture->snap_shoot();
    //     if (image.empty()) {
    //         printf("image is empty\n");
    //         continue;
    //     }

    //     auto end = std::chrono::system_clock::now();
    //     int  delay_infer =
    //         std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
    //             .count();
    //     std::cout << "------------------------------------------- cost:"
    //               << delay_infer << " ms" << std::endl;
    //     // std::cout << "img shape:" << image.rows << " " << image.cols <<
    //     // std::endl;
    //     cv::imwrite("outs/test" + std::to_string(idx++) + ".jpg", image);
    //     // break;
    // }
    // capture->logout();
}
