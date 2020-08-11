//
// Created by redwan on 8/10/20.
//

#ifndef GPURESIZER_FILEMANAGER_H
#define GPURESIZER_FILEMANAGER_H
#include <iostream>
#include <map>
#include <memory>
#include <cassert>
#include <unordered_set>
#include <boost/filesystem.hpp>
#include <regex>
using namespace std;
namespace fs = boost::filesystem;

enum class JOIN_TYPE
{
    NONE = 0,
    HORIZONTAL,
    VERITCAL
};


class FileManager;
typedef shared_ptr<FileManager> FileManagerPtr;
typedef map<string, string> IOMAP;
class FileManager: public enable_shared_from_this<FileManager> {
public:
FileManager();
void operator()(const string& src_dir, const string& dest_dir);
FileManagerPtr getPtr();
/*
 * When other images are required to concatenate with the source image.
 * This function makes a join map for those files
 */
void set_join(const string& join_dir, int join_type );
bool is_image_file(const string& file)const;
bool is_json_file(const string& file)const;
JOIN_TYPE joinType;
private:
 string src_dir_;
 bool read_src_;
    std::unique_ptr<std::regex> img_regex, json_regex;
public:
    map<string, string> filemap;
    map<string, string> joinmap;
    unordered_set<string>filenames;
};


#endif //GPURESIZER_FILEMANAGER_H
