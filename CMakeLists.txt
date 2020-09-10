cmake_minimum_required(VERSION 3.10)
project(BatchResizer)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(Boost  COMPONENTS program_options filesystem REQUIRED)

include_directories(include ${Boost_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        )

set(HEADER_FILES
        include/ArgParser.h
        include/FileManager.h
        include/ImgResizer.h
        include/CoreOperations.h
        )
set(SRC_FILES
        ImgResizer.cpp
        FileManager.cpp
        )
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SRC_FILES}
        main.cpp

        )
target_link_directories(${PROJECT_NAME} PUBLIC
        /home/redwan/CLionProjects/opencv/build/3rdparty/lib
        /home/redwan/CLionProjects/opencv/build/lib)
target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${OpenCV_LIBS}
        -pthread
        )

## Install executable
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        )

## Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h*")