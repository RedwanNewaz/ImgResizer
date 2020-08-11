//
// Created by redwan on 8/10/20.
//

#ifndef GPURESIZER_ARGPARSER_H
#define GPURESIZER_ARGPARSER_H
#include "FileManager.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <memory>
#include <boost/program_options.hpp>
using namespace boost::program_options;
using namespace nlohmann;
class ArgParser
{
public:
    ArgParser(int argc, char *argv[]);
    bool operator()(const string& arg1, const FileManagerPtr& fileManager);
    std::string size, crop;
private:
    unique_ptr<variables_map> vm;
    unique_ptr<options_description> desc;
protected:
    void mf_read_arguments(const FileManagerPtr& fileManager);
    void mf_read_configFile(const string& configFile, const FileManagerPtr& fileManager);
    
};

ArgParser::ArgParser(int argc, char **argv) {
    desc = std::make_unique<options_description>("Options");
    desc->add_options()
            ("help,h", "Help screen")
            ("size", value<std::string>(), "size = widthxheight e.g., 300x300 ")
            ("crop",value<std::string>(), "rect = X0xY0xwidthxheight , e.g., 0x0x300x300 ")
            ("hjoin",value<std::string>(), "folder name for horizontal concatenation")
            ("vjoin", value<std::string>(), "folder name for vertical concatenation")
            ("dest", value<std::string>()->default_value("./"), "destination folder")
            ("conf", value<std::string>(), "JSON configuration file");
    vm = make_unique<variables_map>();
    store(parse_command_line(argc, argv, *desc), *vm);
    notify(*vm);
}

bool ArgParser::operator()(const string& arg1, const FileManagerPtr& fileManager) {

    if(vm->count("help"))
    {
        std::cout<<*desc<<std::endl;
        return false;
    }
    if(fileManager->is_json_file(arg1))
    {
        mf_read_configFile(arg1, fileManager);
    }
    else if( fs::is_directory(fs::path{arg1}))
    {
        mf_read_arguments(fileManager);
    }
    else
        return false;

    return true;
}

void ArgParser::mf_read_arguments(const FileManagerPtr& fileManager) {
    string src_dir, dest_dir, join_dir;
    auto vms = *vm.get();
    int join_type = 0;

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
        join_dir = vms["hjoin"].as<std::string>();
        join_type = 1;
    }
    else if (vm->count("vjoin"))
    {
        join_dir = vms["vjoin"].as<std::string>();
        join_type = 2;
    }

    fileManager->operator()(src_dir, dest_dir);
    if(join_dir.length()>0)
        fileManager->set_join(join_dir, join_type);
}

void ArgParser::mf_read_configFile(const string &configFile, const FileManagerPtr& fileManager) {
    json j;
    std::ifstream i(configFile);
    i >> j;
    string src_dir, dest_dir, join_dir;
    int join_type = 0;

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
        nlohmann::from_json(j["hjoin"], join_dir);
        join_type = 1;
    }
    else if (j.count("vjoin"))
    {
        nlohmann::from_json(j["vjoin"], join_dir);
        join_type = 2;
    }

    if(j.count("batch_dir"))
    {
        nlohmann::from_json(j["batch_dir"], src_dir);
        std::cout<<"[batch_dir]: "<<src_dir<<"\n";
    }
    else
    {
        std::cerr<<"[ERROR]: cannot find batch directory in json file \n";
    }

    fileManager->operator()(src_dir, dest_dir);
    if(join_dir.length()>0)
    {
        fileManager->set_join(join_dir, join_type);
    }

}


#endif //GPURESIZER_ARGPARSER_H
