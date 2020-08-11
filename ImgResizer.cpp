//
// Created by redwan on 8/10/20.
//

#include "ImgResizer.h"

#include <utility>

ImgResizer::ImgResizer(FileManagerPtr fileManager):fileManager(std::move(fileManager)) {
    is_cropping = is_resizing = is_joining = false;
}


void ImgResizer::decode_param(const string &str_param, vector<int> &int_param) {
    std::regex rgx("[x]");
    std::sregex_token_iterator iter(str_param.begin(),
                                    str_param.end(),
                                    rgx,
                                    -1);
    std::sregex_token_iterator end;
    do
    {
        std::string digit = *iter;
        int_param.push_back(atoi(digit.c_str()));
    }while(++iter != end);
}

void ImgResizer::operator()(const vector<int> &param) {

    if (param.size()<1)
        return;
    else if (param.size() == CROP_PARAM)
    {

        crop_param_.clear();
        copy(param.begin(), param.end(), back_inserter(crop_param_));
        is_cropping = true;
    }
    else if(param.size() == RESIZE_PARAM)
    {

        resize_param_.clear();
        copy(param.begin(), param.end(), back_inserter(resize_param_));
        is_resizing = true;
    }

}

void ImgResizer::execute() {
    auto files = fileManager->filemap;
    is_joining = fileManager->joinmap.size()>0;
    auto start = std::chrono::steady_clock::now();
    if(is_joining)
    {
        for(auto it = files.begin(); it != files.end(); ++it)
            parallel.push_back(async(join, it->first, it->second, fileManager->joinType));
    }
    else if(is_resizing && is_cropping)
    {
        cout <<"[ImgResizer]: cropping & resizing images \n";
  
        for(auto it = files.begin(); it != files.end(); ++it)
        {
            parallel.push_back(async(combo_resize,resize_param_, crop_param_, it->first, it->second));
        }
    }
    else if (is_cropping)
    {
        cout <<"[ImgResizer]: cropping images \n";
        for(auto it = files.begin(); it != files.end(); ++it)
        {
            parallel.push_back(async(crop,resize_param_, crop_param_, it->first, it->second));
        }

    }
    else if (is_resizing)
    {
        cout<<"[ImgResizer]: resizing images \n";
        for(auto it = files.begin(); it != files.end(); ++it)
        {
            parallel.push_back(async(resize,resize_param_, crop_param_, it->first, it->second));
        }
    }

    
    for (int j = 0; j < parallel.size(); ++j) {
        parallel[j].get();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << int(parallel.size()/elapsed_seconds.count()) << " FPS\n";


}
