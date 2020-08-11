//
// Created by redwan on 8/10/20.
//

#ifndef resizeR_COREOPERATIONS_H
#define resizeR_COREOPERATIONS_H
#include <future>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define CROP_PARAM 4
#define RESIZE_PARAM 2

class CoreOperations
{
protected:
    static void combo_resize(const vector<int>& resize_param_, const vector<int>& crop_param_,
                             const string& src, const string& dest)
    {
        auto img = cv::imread(src);
        cv::Rect roi(crop_param_[0], crop_param_[1], crop_param_[2], crop_param_[3]);
        img = img(roi);
        cv::resize(img,img,cv::Size(resize_param_[0], resize_param_[1]));
        cv::imwrite(dest, img);
    }
    static void resize(const vector<int>& resize_param_, const vector<int>& crop_param_,
                       const string& src, const string& dest)
    {
        auto img = cv::imread(src);

        cv::resize(img,img,cv::Size(resize_param_[0], resize_param_[1]));
        cv::imwrite(dest, img);
    }
    static void crop(const vector<int>& resize_param_, const vector<int>& crop_param_,
                     const string& src, const string& dest)
    {
        auto img = cv::imread(src);
        cv::Rect roi(crop_param_[0], crop_param_[1], crop_param_[2], crop_param_[3]);
        img = img(roi);
        cv::imwrite(dest, img);
    }

    static void join(const string& src, const string& dest, const JOIN_TYPE& type)
    {
        auto img = cv::imread(src);
        auto img2 = cv::imread(dest);
        switch (type) {
            case JOIN_TYPE::HORIZONTAL:hconcat(img, img2, img);break;
            case JOIN_TYPE::VERITCAL: vconcat(img, img2, img);break;
        }
        cv::imwrite(dest, img);
    }

    static void combo_join(const string& src, const string& dest, const JOIN_TYPE& type, const vector<int>& crop_param_)
    {
        auto img = cv::imread(src);
        auto img2 = cv::imread(dest);
        cv::Rect roi(crop_param_[0], crop_param_[1], crop_param_[2], crop_param_[3]);
        img = img(roi);
        if((img.size[0] != img2.size[0]) ||(img.size[1] != img2.size[1]) )
            cv::resize(img2,img2,cv::Size(img.size[0] , img.size[1] ));
        switch (type) {
            case JOIN_TYPE::HORIZONTAL:hconcat(img, img2, img);break;
            case JOIN_TYPE::VERITCAL: vconcat(img, img2, img);break;
        }
        cv::imwrite(dest, img);
    }
};
#endif //resizeR_COREOPERATIONS_H
