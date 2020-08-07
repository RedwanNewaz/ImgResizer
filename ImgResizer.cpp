//
// Created by redwan on 8/7/20.
//

#include "ImgResizer.h"
#include "tqdm/tqdm.h"

ImgResizer::ImgResizer(std::shared_ptr<variables_map> vm) {
    img_regex = std::unique_ptr<std::regex>(new std::regex(".*\\.jpg$|.*\\.png$"));
    img_size_regex = std::unique_ptr<std::regex>(new std::regex("(^\\d+)+[x]+(\\d+$)"));
    crop_size_regex = std::unique_ptr<std::regex>(new std::regex("(^\\d+)+[x]+(\\d+)+[x]+(\\d+)+[x](\\d+$)"));
    m_decode_vm(vm);
}

void ImgResizer::img_batch_dir(const std::string &pathName) {

    std::vector<std::future<int>> parallel;
    auto start = std::chrono::steady_clock::now();
    boost::filesystem::path pathDir{pathName};
    for(auto & p : boost::filesystem::directory_iterator( pathDir ))
    {
        if (std::regex_match(p.path().string(), *img_regex))
            parallel.push_back(std::async(decode_img,p.path().string(), dest_dir,crop_size, img_size));
    }
    for (auto& action:tqdm::tqdm(parallel)) {
        action.get();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << int(parallel.size()/elapsed_seconds.count()) << " FPS\n";
}

int ImgResizer::decode_img(const std::string &filename, const std::string& dest_dir,
                           const std::vector<int>&crop_size, const std::vector<int>& img_size) {

    auto image = imread(filename);
    //Create the rectangle
    if(crop_size.size()== CROP_PARAM)
    {
        cv::Rect roi(crop_size[0], crop_size[1], crop_size[2], crop_size[3]);
        image = image(roi);
    }

    if(img_size.size()== RESIZE_PARAM)
    {
        cv::Mat dst;
        resize(image, dst,Size(img_size[0], img_size[1]));
        image = dst;
    }
//    imwrite(dest_dir/)
//    auto output_file = fs::path(dest_dir);
//    output_file /= fs::path(filename).filename();
    auto output_file = fs::path(dest_dir);
    output_file /= fs::path(filename).filename();


    imwrite(output_file.string(), image);
    return filename.length();
}

void ImgResizer::m_decode_vm(std::shared_ptr<variables_map> &vm) {

    auto vms = *vm.get();
    std::string size, crop;
    if (vm->count("size"))
    {
        size = vms["size"].as<std::string>();
        std::cout<<"[size]: "<<size<<"\n";
    }

    if (vm->count("crop"))
    {
        crop =  vms["crop"].as<std::string>();
        std::cout<<"[crop]: "<<crop<<"\n";
    }
    if(vm->count("dest"))
    {
        dest_dir = vms["dest"].as<std::string>();
        std::cout<<"[dest_dir]: "<<dest_dir<<"\n";
    }

    if(std::regex_match(size, *img_size_regex))
    {
        img_size = m_get_digits(size);

    }
    if(std::regex_match(crop, *crop_size_regex))
    {
        crop_size = m_get_digits(crop);
    }
}

std::vector<int> ImgResizer::m_get_digits(std::string& string_to_split) {
    std::regex rgx("[x]");
    std::sregex_token_iterator iter(string_to_split.begin(),
                                    string_to_split.end(),
                                    rgx,
                                    -1);
    std::sregex_token_iterator end;
    std::vector<int> result;
    do
    {
        std::string digit = *iter;
        result.push_back(atoi(digit.c_str()));
    }while(++iter != end);

    return result;
}

void ImgResizer::decode_single_img(const std::string &filename) {
    decode_img(filename, dest_dir, crop_size, img_size);
}

void ImgResizer::config_file_reader(const std::string &filename) {
    nlohmann::json j;
    std::ifstream i(filename);
    i >> j;
//
    std::string size, crop;
    if (j.count("size"))
    {
        nlohmann::from_json(j["size"], size);
        std::cout<<"[size]: "<<size<<"\n";
    }

    if (j.count("crop"))
    {
        nlohmann::from_json(j["crop"], crop);
        std::cout<<"[crop]: "<<crop<<"\n";
    }
    if(j.count("dest"))
    {
        nlohmann::from_json(j["dest"], dest_dir);
        std::cout<<"[dest_dir]: "<<dest_dir<<"\n";
    }

    if(std::regex_match(size, *img_size_regex))
    {
        img_size = m_get_digits(size);

    }
    if(std::regex_match(crop, *crop_size_regex))
    {
        crop_size = m_get_digits(crop);
    }

    if(j.count("batch_dir"))
    {
        std::string batch_dir;
        nlohmann::from_json(j["batch_dir"], batch_dir);
        std::cout<<"[batch_dir]: "<<batch_dir<<"\n";
        img_batch_dir(batch_dir);
    }
    else
    {
        std::cerr<<"[ERROR]: cannot find batch directory in json file \n";
    }

}
