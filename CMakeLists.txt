cmake_minimum_required(VERSION 2.8)
project( reconhecimento )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS}    /home/leo/source/opencv/opencv-2.4.9/modules/objdetect/include )
add_executable( reconhecimento reconhecimento.cpp )
target_link_libraries( reconhecimento ${OpenCV_LIBS} )

