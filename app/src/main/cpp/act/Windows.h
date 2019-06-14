//
// Created by yanny on 14/6/2019.
//

#ifndef RINCONADA_WINDOWS_H
#define RINCONADA_WINDOWS_H

#include <android_native_app_glue.h>
struct WindowInfo{

    WindowInfo(android_app* pApp, int32_t pWidth, int32_t pHeight):
    app(pApp),
    width(pWidth),
    height(pHeight){}
    int32_t width;
    int32_t height;
    struct android_app* app;


};


class Windows {
public:
    void start();
    void end();

private:
    int32_t width;
    int32_t height;
};


#endif //RINCONADA_WINDOWS_H
