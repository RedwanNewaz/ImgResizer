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

    std::vector<std::future<void>> parallel;
    auto start = std::chrono::steady_clock::now();
    boost::filesystem::path pathDir{pathName}, joinDir{join_dir};
    std::vector<std::string>path_from, path_join;
    for(auto & p : boost::filesystem::directory_iterator( pathDir ))
    {
        if (std::regex_match(p.path().string(), *img_regex))
            switch (concate_img) {
                case SINGLE:
                    parallel.push_back(std::async(decode_img,p.path().string(), dest_dir,crop_size, img_size));break;
                default:
                    path_from.push_back(p.path().string()); break;;
            }

    }
    if(concate_img != SINGLE)
    {
        auto type = (concate_img == HORIZONTAL)?"horizontal":"vertical";
        std::cout<<"[JOIN_TYPE]: "<< type <<"\n";
        // read the join dir file
        for(auto & p : boost::filesystem::directory_iterator( joinDir ))
            if (std::regex_match(p.path().string(), *img_regex))
                path_join.push_back(p.path().string());
        std::sort(path_from.begin(), path_from.end());
        std::sort(path_join.begin(), path_join.end());
//        assert(path_from.size()==path_join.size());
        // if not same size then use minimum size
        std::size_t N = std::min(path_join.size(), path_from.size());

        for (int i = 0; i < N; ++i) {
            parallel.push_back(std::async(join_img,path_from[i],path_join[i],dest_dir,concate_img, crop_size, img_size));

        }
    }

    for (auto& action:tqdm::tqdm(parallel))
        action.get();

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << int(parallel.size()/elapsed_seconds.count()) << " FPS\n";
}

void ImgResizer::decode_img(const std::string &filename, const std::string& dest_dir,
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
        resize(image, image,Size(img_size[0], img_size[1]));
    }
    auto output_file = fs::path(dest_dir);
    output_file /= fs::path(filename).filename();


    imwrite(output_file.string(), image);

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

    if(vm->count("hjoin"))
    {
        concate_img = HORIZONTAL;
        join_dir = vms["hjoin"].as<std::string>();
    }
    else if (vm->count("vjoin"))
    {
        concate_img = VERTICAL;
        join_dir = vms["vjoin"].as<std::string>();
    }
    else
    {
        concate_img = SINGLE;
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
    if(j.count("hjoin"))
    {
        concate_img = HORIZONTAL;
        nlohmann::from_json(j["hjoin"], join_dir);
    }
    else if (j.count("vjoin"))
    {
        concate_img = VERTICAL;
        nlohmann::from_json(j["vjoin"], join_dir);
    }
    else
    {
        concate_img = SINGLE;
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

void ImgResizer::join_img(const std::string &file_from, const std::string &file_join,
                          const std::string &dest_dir, const JOIN_TYPE& type,
                          const std::vector<int>&crop_size, const std::vector<int>& img_size)
{

    auto src_img = imread(file_from);
    if(crop_size.size()== CROP_PARAM)
    {
        cv::Rect roi(crop_size[0], crop_size[1], crop_size[2], crop_size[3]);
        src_img = src_img(roi);
    }
    if(img_size.size()== RESIZE_PARAM)
    {
        resize(src_img, src_img,Size(img_size[0], img_size[1]));
    }
    auto join_img = imread(file_join);
    /*
     * We can crop the src image and fix the join image.
     * Join image need to be resized based on the src image size
     * src image can be given in cropped format as well
     * Join image must NOT be given in cropped format
     */
    bool need_resize = (src_img.size[0] != join_img.size[0]) || (src_img.size[1] != join_img.size[1]);
    bool cropping = (crop_size[0] == join_img.size[0]) and (crop_size[1] == join_img.size[1]);
    if(need_resize and not cropping)
        resize(join_img, join_img,Size(src_img.size[0], src_img.size[1]));

    switch (type) {
        case HORIZONTAL:hconcat(src_img, join_img,  src_img);break;
        case VERTICAL:vconcat(src_img, join_img, src_img);break;
    }


    auto output_file = fs::path(dest_dir);
    output_file /= fs::path(file_from).filename();
    imwrite(output_file.string(), src_img);
}
