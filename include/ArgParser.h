//
// Created by redwan on 8/7/20.
//

#ifndef RESIZER_ARGPARSER_H
#define RESIZER_ARGPARSER_H
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#define debug(x) std::cout<<x<<std::endl
enum OPTION
{
    CONFIG_FILE,
    IMG_FILE,
    IMG_DIR,
    NONE
};

class ArgParser
{
public:
    ArgParser()
    {
        img_regex = std::unique_ptr<std::regex>(new std::regex(".*\\.jpg$|.*\\.png$"));
        json_regex = std::unique_ptr<std::regex>(new std::regex(".*\\.json$"));

    }
    OPTION parse_args(int argc, char *argv[])
    {
        if (argc<2)
        {
            debug("[ArgParser] argument NOT FOUND");
            return NONE;
        }
        else if (std::regex_match(argv[1], *json_regex))
        {
//            debug("[ArgParser] FOUND config file");
            return CONFIG_FILE;
        }
        else if (std::regex_match(argv[1], *img_regex))
        {
//            debug("[ArgParser] FOUND image file");
            return IMG_FILE;
        }
        else
        {
            debug("[ArgParser] FOUND image directory");
            return IMG_DIR;
        }

    }

private:
    std::unique_ptr<std::regex>json_regex, img_regex;
};
#endif //RESIZER_ARGPARSER_H
