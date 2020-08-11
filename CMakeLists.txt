cmake_minimum_required(VERSION 3.10)
project(GpuResizer)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
find_package(Boost COMPONENTS filesystem program_options REQUIRED)

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(GpuResizer main.cpp FileManager.cpp FileManager.h ArgParser.h ImgResizer.cpp ImgResizer.h)
target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${OpenCV_LIBS} 
        -pthread
        )