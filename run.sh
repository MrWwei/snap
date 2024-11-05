export THIRD_PARTY=/home/mnt/wtwei/ThirdParty
# export LD_LIBRARY_PATH=/home/user/mpp-develop/build/mpp:$THIRD_PARTY/rknpu2/examples/3rdparty/rga/RK3588/lib/Linux/aarch64:$THIRD_PARTY/FFmpeg-n6.0/install/lib:$THIRD_PARTY/opencv-4.5.4/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$THIRD_PARTY/ZLMediaKit/install/lib:$THIRD_PARTY/FFmpeg-n6.0/install/lib:$THIRD_PARTY/opencv-4.5.4/build/install/lib:$LD_LIBRARY_PATH
./build/VideoDecoder rtsp://192.168.1.11/videos/300w4M25z.mp4
