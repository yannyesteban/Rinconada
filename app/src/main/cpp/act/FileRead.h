//
// Created by yanny on 24/6/2019.
//

#ifndef RINCONADA_FILEREAD_H
#define RINCONADA_FILEREAD_H
#include <fstream>
#include <iostream>
#include <string>
#include <android/asset_manager.h>
#include "Log.h"

class FileRead {


public:
    const char *  Open(AAssetManager* mAssetManager, const char * pPath);

    void test(AAssetManager* mAssetManager, const char * paths, const char * paths2);
private:
};


#endif //RINCONADA_FILEREAD_H
