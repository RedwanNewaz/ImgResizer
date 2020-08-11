//
// Created by redwan on 8/10/20.
//

#ifndef GPURESIZER_IMGRESIZER_H
#define GPURESIZER_IMGRESIZER_H
#include "FileManager.h"
#include <future>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/cudaimgproc.hpp"

#define CROP_PARAM 4
#define RESIZE_PARAM 2
using namespace cv;
class ImgResizer {
public:
    explicit ImgResizer(FileManagerPtr  fileManager);
    void operator()(const vector<int>& param);
    static void decode_param(const string& str_param, vector<int>& int_param);
    void execute();

private:
    const FileManagerPtr fileManager;
    vector<future<void>>parallel;
    vector<int> crop_param_, resize_param_;
    bool is_cropping, is_resizing, is_joining;

protected:
    static void combo_resize(const vector<int>& resize_param_, const vector<int>& crop_param_,
                             const string& src, const string& dest)
    {
        auto img = imread(src);
        cv::Rect roi(crop_param_[0], crop_param_[1], crop_param_[2], crop_param_[3]);
        img = img(roi);
        cuda::GpuMat gpuInImage;
        cuda::GpuMat gpuOutImage;
        gpuInImage.upload(img);
        cuda::resize(gpuInImage, gpuOutImage, Size(resize_param_[0], resize_param_[1]));
        gpuOutImage.download(img);
        imwrite(dest, img);
    }


    static void resize(const vector<int>& resize_param_, const vector<int>& crop_param_,
                       const string& src, const string& dest)
    {
        auto img = imread(src);
        cuda::GpuMat gpuInImage;
        cuda::GpuMat gpuOutImage;
        gpuInImage.upload(img);
        cuda::resize(gpuInImage, gpuOutImage, Size(resize_param_[0], resize_param_[1]));
        gpuOutImage.download(img);
        imwrite(dest, img);
    }
    static void crop(const vector<int>& resize_param_, const vector<int>& crop_param_,
                     const string& src, const string& dest)
    {
        auto img = imread(src);
        cv::Rect roi(crop_param_[0], crop_param_[1], crop_param_[2], crop_param_[3]);
        img = img(roi);
        imwrite(dest, img);
    }

    static void join(const string& src, const string& dest, const JOIN_TYPE& type)
    {
        auto img = imread(src);
        auto img2 = imread(dest);
        switch (type) {
            case JOIN_TYPE::HORIZONTAL:hconcat(img, img2, img);break;
            case JOIN_TYPE::VERITCAL: vconcat(img, img2, img);break;
        }
        imwrite(dest, img);
    }
};


#endif //GPURESIZER_IMGRESIZER_H
