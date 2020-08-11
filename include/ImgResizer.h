//
// Created by redwan on 8/10/20.
//

#ifndef GPURESIZER_IMGRESIZER_H
#define GPURESIZER_IMGRESIZER_H
#include "FileManager.h"
#include "CoreOperations.h"



class ImgResizer: public CoreOperations {
public:
    explicit ImgResizer(FileManagerPtr  fileManager);
    void operator()(const vector<int>& param);
    static void decode_param(const string& str_param, vector<int>& int_param);
    void execute();

private:
    const FileManagerPtr fileManager;
    vector<future<void>>parallel;
    vector<int> crop_param_, resize_param_;
    bool is_cropping, is_resizing, is_joining;

};


#endif //GPURESIZER_IMGRESIZER_H
