cmake_minimum_required(VERSION 3.1)
project(Resizer)
set(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#SET(BUILD_SHARED_LIBS OFF)
#SET(CMAKE_EXE_LINKER_FLAGS "-static")
SET(Boost_USE_STATIC_LIBS ON)
#set(OpenCV_STATIC ON)

find_package( OpenCV REQUIRED )
find_package(Boost  COMPONENTS program_options filesystem REQUIRED)

include_directories(include ${Boost_INCLUDE_DIRS})
add_executable(Resizer main.cpp include/ArgParser.h ImgResizer.cpp
        include/ImgResizer.h)
target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${OpenCV_LIBS}
        -pthread
        )