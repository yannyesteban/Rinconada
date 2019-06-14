//
// Created by yanny on 14/6/2019.
//

#ifndef RINCONADA_GRAPHICMANAGER_H
#define RINCONADA_GRAPHICMANAGER_H
#include <jni.h>
#include <cstdlib>

#include <android_native_app_glue.h>

struct GraphicsInfo{
    GraphicsInfo(int32_t pWidth, int32_t pHeight):
    width(pWidth),
    height(pHeight){}
    int32_t width;
    int32_t height;



};

class GraphicManager {

public:


private:



};


#endif //RINCONADA_GRAPHICMANAGER_H
