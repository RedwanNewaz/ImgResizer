FROM nvidia/cuda:10.1-devel-ubuntu18.04

WORKDIR /build

RUN apt-get update 
RUN apt-get install -y unzip python3 cmake g++ libboost-all-dev

ADD vendor/opencv-4.1.1.tar.gz /build/
ADD vendor/opencv_contrib-4.1.1.tar.gz /build/

RUN mkdir -p opencv-4.1.1/build && \
  cd opencv-4.1.1/build/ && \
  cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D INSTALL_PYTHON_EXAMPLES=OFF \
    -D INSTALL_C_EXAMPLES=OFF \
    -D OPENCV_ENABLE_NONFREE=ON \
    -D OPENCV_EXTRA_MODULES_PATH=/build/opencv_contrib-4.1.1/modules \
    -D BUILD_EXAMPLES=OFF \
    -D WITH_CUDA=ON \
    -D CMAKE_LIBRARY_PATH=/usr/local/cuda/lib64/stubs \
    -D BUILD_opencv_cudacodec=OFF \
    -D PYTHON_EXECUTABLE=/usr/bin/python3 ..

RUN cd opencv-4.1.1/build/ && make -j9
RUN cd opencv-4.1.1/build/ && make install