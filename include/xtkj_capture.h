
/*
*┌────────────────────────────────────────────────┐
*│　author：wtwei
*│　version：1.0
*│　create time：2023/01/30 16:42
*└────────────────────────────────────────────────┘
*/
#ifndef XTKJ_CAPTURE_H
#define XTKJ_CAPTURE_H
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;

namespace xtkj {
		
	extern "C" cv::Mat snap_shoot(const char* rtsp_url, int time_out=1000);
			
}
#endif