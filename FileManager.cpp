//
// Created by redwan on 8/10/20.
//

#include "FileManager.h"

#include <memory>

FileManager::FileManager() {
    read_src_ = false;
    img_regex = std::make_unique<std::regex>(".*\\.jpg$|.*\\.png$");
    json_regex = std::make_unique<std::regex>(".*\\.json$");
    joinType= JOIN_TYPE::NONE;
}

void FileManager::operator()(const string &src_dir, const string &dest_dir) {
    boost::filesystem::path srcDir{src_dir};
    this->src_dir_ = src_dir;
    for(auto & p : boost::filesystem::directory_iterator( srcDir ))
    {
        string absolute_src_name = p.path().string();
        string filename = p.path().filename().string();
        if(!is_image_file(filename))continue;
        boost::filesystem::path destDir{dest_dir};
        destDir/=filename;
        string absolute_dest_name = destDir.string();
        filemap[absolute_src_name] = absolute_dest_name;
        filenames.insert(filename);
    }
    read_src_ = true;
}

FileManagerPtr FileManager::getPtr() {
    return shared_from_this();
}

void FileManager::set_join(const string &join_dir, int join_type) {
    assert(read_src_ && "src dir has not been parsed yet");
    joinType = static_cast<JOIN_TYPE>(join_type);
    boost::filesystem::path joinDir{join_dir};
    for(auto & p : boost::filesystem::directory_iterator( joinDir ))
    {
        string absolute_join_name = p.path().string();
        string filename = p.path().filename().string();
        if(!is_image_file(filename))continue;
        if(filenames.count(filename))
        {
            boost::filesystem::path srcDir{src_dir_};
            srcDir /= filename;
            string absolute_src_name = srcDir.string();
            joinmap[absolute_src_name] = absolute_join_name;
        }
    }
}

bool FileManager::is_image_file(const string &file) const {
    return (std::regex_match(file, *img_regex));
}

bool FileManager::is_json_file(const string &file) const {
    return (std::regex_match(file, *json_regex));
}
