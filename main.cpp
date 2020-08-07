#include "ArgParser.h"
#include "ImgResizer.h"

int main(int argc, char *argv[]) {

    std::cout << "Resizer Initilize" << std::endl;

    options_description desc{"Options"};
    desc.add_options()
            ("help,h", "Help screen")
            ("size", value<std::string>(), "size =(width x height)")
            ("crop",value<std::string>(), "rect =(originX x originY x width x height)")
            ("dest", value<std::string>()->default_value("./"), "destination folder")
            ("conf", value<std::string>(), "JSON configuration file");

    std::shared_ptr<variables_map> vm = std::make_shared<variables_map>();
    store(parse_command_line(argc, argv, desc), *vm);
    notify(*vm);
    if (vm->count("help")) {
        std::cout << desc << "\n";
        return 1;
    }


    ArgParser argParser;
    OPTION opt = argParser.parse_args(argc, argv);
    ImgResizer resizer(vm);

    switch (opt) {
        case IMG_DIR: resizer.img_batch_dir(argv[1]);break;
        case IMG_FILE: resizer.decode_single_img(argv[1]);break;
        case CONFIG_FILE: resizer.config_file_reader(argv[1]); break;

    }
    return 0;
}
