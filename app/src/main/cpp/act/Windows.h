//
// Created by yanny on 14/6/2019.
//

#ifndef RINCONADA_WINDOWS_H
#define RINCONADA_WINDOWS_H

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    Windows(WindowInfo *);

    bool init();
    void start();
    void end();

    void test();
    void test2();
    void test3();
    void test4();
    void test1a();

    void loadTexture();
    int32_t touchX;
    int32_t touchY;
    GLuint programObject;
private:
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;


    WindowInfo * info;
    int32_t width;
    int32_t height;

    int initDisplay();
    int closeDisplay();
    int ActivityLoop();
    void draw_frame();
    void Draw(GLuint programObject);


    static void handle_cmd(android_app* app, int32_t cmd);
    static int handle_input(android_app* app, AInputEvent* event);
};


#endif //RINCONADA_WINDOWS_H
