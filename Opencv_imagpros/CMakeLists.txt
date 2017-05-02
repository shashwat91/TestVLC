cmake_minimum_required(VERSION 2.11)
project( Opencv_imagpros )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( imgPros src/imgPros.cpp src/detection.cpp)
target_link_libraries( imgPros ${OpenCV_LIBS} )

#add_executable( detection src/detection.cpp )
#target_link_libraries( detection ${OpenCV_LIBS} )

#add_executable( DisplayImage src/DisplayImage.cpp )
#target_link_libraries( DisplayImage ${OpenCV_LIBS} )
