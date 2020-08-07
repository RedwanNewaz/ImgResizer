//
// Created by redwan on 8/7/20.
//

#ifndef RESIZER_IMGRESIZER_H
#define RESIZER_IMGRESIZER_H
#include <iostream>
#include <regex>
#include <filesystem>
#include <vector>
#include <future>
#include <chrono>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/program_options.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "nlohmann/json.hpp"
#define CROP_PARAM 4
#define RESIZE_PARAM 2
using namespace boost::program_options;
namespace fs = std::filesystem;
using namespace cv;

class ImgResizer {

public:
    ImgResizer(std::shared_ptr<variables_map> vm);
    void img_batch_dir(const std::string& path);
    void decode_single_img(const std::string& filename);
    void config_file_reader(const std::string& filename);

private:
    std::unique_ptr<std::regex>img_regex, img_size_regex, crop_size_regex;
    std::vector<int>crop_size, img_size;
    std::string config, dest_dir;
protected:
    static int decode_img(const std::string& filename, const std::string& dest_dir,
                          const std::vector<int>&crop_size_, const std::vector<int>& img_size);
    void m_decode_vm(std::shared_ptr<variables_map>& vm);
    std::vector<int> m_get_digits(std::string& string_to_split);

};


#endif //RESIZER_IMGRESIZER_H
