export THIRD_PARTY=/home/ubuntu/ThirdParty
# export LD_LIBRARY_PATH=/home/user/mpp-develop/build/mpp:$THIRD_PARTY/rknpu2/examples/3rdparty/rga/RK3588/lib/Linux/aarch64:$THIRD_PARTY/FFmpeg-n6.0/install/lib:$THIRD_PARTY/opencv-4.5.4/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$THIRD_PARTY/ZLMediaKit/install/lib:$THIRD_PARTY/ffmpeg-4.3.8/install/lib:$THIRD_PARTY/opencv-4.5.4/build/install/lib:$LD_LIBRARY_PATH
./build/VideoDecoder 0 file.txt out_dir 8 20 1
# ./build/VideoDecoder rtsp://admin:xtkj12345@192.168.1.86:554/h264/ch33/main/av_stream
