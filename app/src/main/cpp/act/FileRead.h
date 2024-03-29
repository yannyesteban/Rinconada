//
// Created by yanny on 24/6/2019.
//

#ifndef RINCONADA_FILEREAD_H
#define RINCONADA_FILEREAD_H
#include <fstream>
#include <iostream>
#include <string>
#include <android/asset_manager.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <vector>
#include "Log.h"

struct GMMesh{

    std::vector<glm::vec3> v;
    std::vector<glm::vec2> t;
    std::vector<glm::vec3> n;
    std::vector<glm::vec3> m;
    std::vector<GLushort> vi;
    std::vector<GLushort> ti;
    std::vector<GLushort> ni;

};

class FileRead {


public:


    const char *  Open(AAssetManager* mAssetManager, const char * pPath);

    void test(AAssetManager* mAssetManager, const char * paths, const char * paths2);

    static int s_value;


    static void AA();

    static const char * _externalDataPath;
    static const char * _internalDataPath;

    static void setAssetManager(AAssetManager* mmAssetManager);
    static void setDataPath(const char * internalDataPath,  const char * externalDataPath);



    int print(const char * path, GMMesh* mesh);
private:
    static AAssetManager * _assetManager;

};
//FileRead::s_value = 1; // initializer
//AAssetManager * FileRead::_assetManager=NULL;
//const char * FileRead::_externalDataPath="";
//onst char * FileRead::_internalDataPath="";
#endif //RINCONADA_FILEREAD_H
