#include <iostream>
#include "ArgParser.h"
#include "ImgResizer.h"

int main(int argc, char *argv[]) {
    assert(argc>1 && "Folder direcotry is required");
    std::cout << "CPU Resizer is Initialize!" << std::endl;
    ArgParser parser(argc, argv);
    shared_ptr<FileManager> filemanager = make_shared<FileManager>();
    ImgResizer imgResizer(filemanager->getPtr());
    if(parser(argv[1],filemanager->getPtr()))
    {
        auto files = filemanager->filemap;
        vector<int> resize_param, crop_param;
        ImgResizer::decode_param(parser.size, resize_param);
        ImgResizer::decode_param(parser.crop, crop_param);
        imgResizer(resize_param);
        imgResizer(crop_param);
        imgResizer.execute();
    }
    return 0;
}
