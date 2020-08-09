#include "ArgParser.h"
#include "ImgResizer.h"

int main(int argc, char *argv[]) {

    std::cout << "Resizer Initilize" << std::endl;

    options_description desc{"Options"};
    desc.add_options()
            ("help,h", "Help screen")
            ("size", value<std::string>(), "size = widthxheight e.g., 300x300 ")
            ("crop",value<std::string>(), "rect = X0xY0xwidthxheight , e.g., 0x0x300x300 ")
            ("hjoin",value<std::string>(), "folder name for horizontal concatenation")
            ("vjoin", value<std::string>(), "folder name for vertical concatenation")
            ("dest", value<std::string>()->default_value("./"), "destination folder")
            ("conf", value<std::string>(), "JSON configuration file");

    std::shared_ptr<variables_map> vm = std::make_shared<variables_map>();
    store(parse_command_line(argc, argv, desc), *vm);
    notify(*vm);
    if(vm->count("help"))
    {
        std::cout<<desc<<std::endl;
        return 0;
    }

    ImgResizer resizer(vm);
    ArgParser argParser;
    OPTION opt = argParser.parse_args(argc, argv);
    switch (opt) {
        case IMG_DIR: resizer.img_batch_dir(argv[1]);break;
        case IMG_FILE: resizer.decode_single_img(argv[1]);break;
        case CONFIG_FILE: resizer.config_file_reader(argv[1]); break;

    }
    return 0;
}
