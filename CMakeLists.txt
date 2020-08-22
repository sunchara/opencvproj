cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(./inih)

add_executable( DisplayImage testblur.cpp)
add_executable( gvc gvc.cpp )

target_link_libraries( DisplayImage ${OpenCV_LIBS} )

target_link_libraries( gvc ${OpenCV_LIBS} )
target_include_directories(gvc PUBLIC ./inih/cpp)
target_link_libraries(gvc inih)


enable_testing()

add_test(wowzaec gvc rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov)
add_test(hiof gvc rtsp://freja.hiof.no:1935/rtplive/definst/hessdalen03.stream )
