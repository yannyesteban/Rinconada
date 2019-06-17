//
// Created by yanny on 14/6/2019.
//
#include <cstdlib>
#include "Log.h"
#include "Windows.h"
#include "Header.h"

Windows::Windows(WindowInfo * mInfo):
    info(mInfo){

}

void Windows::handle_cmd(android_app* app, int32_t cmd){
    Windows& windows = *(Windows*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != nullptr) {
                windows.initDisplay();
                windows.draw_frame();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            windows.closeDisplay();
            break;
        case APP_CMD_LOST_FOCUS:
            windows.draw_frame();
            break;
        default:
            break;
    }
}

int Windows::handle_input(android_app* app, AInputEvent* event){
    Windows & windows = *(Windows*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        windows.touchX = AMotionEvent_getX(event, 0);
        windows.touchY = AMotionEvent_getY(event, 0);
        LOGI("TOCA AQUI x %d\ty %d\n",windows.touchX, windows.touchY);




        return 1;
    }
    return 0;
}


int Windows::ActivityLoop() {
// Read all pending events.
    while (true) {
        int ident;
        int events;
        struct android_poll_source* source;

        while ((ident=ALooper_pollAll(0, NULL, &events,(void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(info->app, source);
            }

            // Check if we are exiting.
            if (info->app->destroyRequested != 0) {
                closeDisplay();
                return 0;
            }
        }

        // Draw the current frame
        draw_frame();
    }
    //return 0;
}
void Windows::init(){

    leer(info->app->activity->assetManager, "vs.glsl");
    /*
    const char* mPath;
    AAssetManager* mAssetManager;
    AAsset* mAsset;
    mPath = "vs.glsl";
    mAssetManager = info->app->activity->assetManager;
    mAsset = AAssetManager_open(mAssetManager, mPath, AASSET_MODE_UNKNOWN);
    if(mAsset!= NULL){
        LOGW("------------------OK----------");
    }else{
        LOGW("ERRRRRRRRRRRRRRROOORRRRRRRRR");
    }

    off_t fileLength = AAsset_getLength(mAsset);
    char *dataBuffer2 = (char *) malloc(fileLength);

    AAsset_read(mAsset, dataBuffer2, fileLength);
//the data has been copied to dataBuffer2, so , close it
    AAsset_close(mAsset);


    LOGW("%s",(const char*)dataBuffer2);
    free(dataBuffer2);
     */
}

void Windows::end(){
int a=1;
}


void Windows::start() {
    info->app->userData = this;
    info->app->onAppCmd = handle_cmd;
    info->app->onInputEvent = handle_input;

    init();

    ActivityLoop();
}

int Windows::initDisplay() {
// Setup OpenGL ES 2
    // http://stackoverflow.com/questions/11478957/how-do-i-create-an-opengl-es-2-context-in-a-native-activity

    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //important
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };

    EGLint attribList[] =
            {
                    EGL_CONTEXT_CLIENT_VERSION, 2,
                    EGL_NONE
            };

    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;


    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(info->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, info->app->window, NULL);

    context = eglCreateContext(display, config, NULL, attribList);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    // Grab the width and height of the surface
    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    // Initialize GL state.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //glViewport(100, 100, width/2, height/2);
    glViewport(100, 100, 200, 200);

    return 1;
}


int Windows::closeDisplay() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
    return 1;
}

void Windows::draw_frame() {
    // No display.
    if (display == NULL) {
        return;
    }

    glViewport(0, 0, 400,400);
    glClearColor(0.0f,0.1f,0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(display, surface);
    return;
}
