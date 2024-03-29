cmake_minimum_required(VERSION 2.11)
project( Opencv_imagpros )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Single image processing
add_executable( imgPros src/imgPros.cpp src/detection.cpp)
target_link_libraries( imgPros ${OpenCV_LIBS} )
# Multiple files at once
add_executable( imgPros_2 src/imgPros_2.cpp src/detection.cpp)
target_link_libraries( imgPros_2 ${OpenCV_LIBS} )
# Scaling down image
add_executable( DisplayImage src/DisplayImage.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )
# Scaling down multiple images
add_executable( scale_images src/scale_images.cpp )
target_link_libraries( scale_images ${OpenCV_LIBS} )
# Video Processing
add_executable( video_cv src/video_cv.cpp src/detection.cpp)
target_link_libraries( video_cv ${OpenCV_LIBS} )
# Circle detection
add_executable( circle src/circles.cpp)
target_link_libraries( circle ${OpenCV_LIBS} )
