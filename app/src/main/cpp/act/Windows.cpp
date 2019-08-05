//
// Created by yanny on 14/6/2019.
//


#include <cstdlib>
#include <JNIHelper.h>
#include<gestureDetector.h>
#include "Log.h"
#include "Windows.h"
#include "Header.h"
#include "Asset.h"
#include "ShadersManager.h"
#include <png.h>
#include <list>
#include "loadpng.h"
#include "GLHelper.h"
#include "FileRead.h"


#define VERTEX_POS_INDX 0
#define VERTEX_NORMAL_INDX 1
#define VERTEX_COLOR_INDEX 1
#define VERTEX_TEXCOORD0_INDX 2


#define VERTEX_POS_INDEX 0
#define VERTEX_POS_SIZE 3 // x, y and z

#define VERTEX_COLOR_INDEX 1
#define VERTEX_COLOR_SIZE 3 // x, y and z

#define VERTEX_NORMAL_INDEX 8
#define VERTEX_NORMAL_SIZE 3 // x, y and z


#define VERTEX_TEXCOORD0_INDEX 3
#define VERTEX_TEXCOORD0_SIZE 2 // s and t


#define VERTEX_TEXCOORD1_INDEX 4
#define VERTEX_TEXCOORD2_INDEX 5



GLuint MatrixID;
GLfloat px=0;
GLfloat py=0;
GLfloat rx=0.0f;
GLfloat ry=0.0f;
GLfloat rz=0.0f;

GLfloat ex=0.0f;
GLfloat ey=1.0f;
GLfloat ez=0.0f;
GLfloat aspect=1.0f;
bool isProgram = false;
bool isMVP = true;
static int _test = 5;

GLint text[5];
GLfloat up=0.0f;
GLfloat left=0.0f;

std::list<GLAttrib> lAttrib;

GMMesh mesh;

void fff(){
    _LOGE("xxxx esperanza");
}

void Windows::test1x(){
    _LOGE("yyyy esperanza2222");
}

Windows::Windows(WindowInfo * mInfo):
    info(mInfo){

}

void Windows::handle_cmd(android_app* app, int32_t cmd){
    _LOGE("handle_cmd***");
    Windows& windows = *(Windows*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != nullptr) {
                _LOGE("APP_CMD_INIT_WINDOW***");
                windows.initDisplay();

                windows.init();
                windows.draw_frame();

            }
            break;
        case APP_CMD_TERM_WINDOW:
            _LOGE("APP_CMD_TERM_WINDOW***");
            // The window is being hidden or closed, clean it up.
            windows.closeDisplay();
            break;
        case APP_CMD_LOST_FOCUS:
            _LOGE("APP_CMD_LOST_FOCUS***");
            windows.draw_frame();
            break;
        default:
            break;
    }
}

int Windows::handle_input(android_app* app, AInputEvent* event){



    ndk_helper::DragDetector drag;
    ndk_helper::GESTURE_STATE xxx = drag.Detect(event);
    //ndk_helper::Vec2 v2;
    //drag.GetPointer(v2);
    //v2.Dump();
    //LOGI("iii %d", v2.);
    _LOGI("gesture xxx %d", xxx);

    Windows & windows = *(Windows*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        windows.touchX = AMotionEvent_getX(event, 0);
        windows.touchY = AMotionEvent_getY(event, 0);
        _LOGI("TOCA AQUI x %d\ty %d\n",windows.touchX, windows.touchY);
        static GLushort mode = 0;
        if(windows.touchX>=0 && windows.touchX<200){
            if(windows.touchY>=1500 && windows.touchY<1800){

                mode = (mode+1) % 3;
                //LOGI("trans Mode %d", mode);


            }
        }
        switch(mode){
            case 0:
                ex=1.0f;
                ey=0.0f;
                ez=0.0f;
                break;
            case 1:
                ex=0.0f;
                ey=1.0f;
                ez=0.0f;
                break;
            case 2:
                ex=0.0f;
                ey=0.0f;
                ez=1.0f;
                break;

        }

        if(windows.touchX>=0 && windows.touchX<200){
            if(windows.touchY>=0 && windows.touchY<200){
                _LOGI("trans PX px-");
                px -= 0.05;
                up = 0;
                left = 0;

            }
        }
        if(windows.touchX>=800 && windows.touchX<1000){
            if(windows.touchY>=0 && windows.touchY<200){
               _LOGI("trans PX px+");
                px += 0.05;
                up = 0;
                left = 0;

            }
        }


        if(windows.touchX>=0 && windows.touchX<200){
            if(windows.touchY>=200 && windows.touchY<400){
                _LOGI("trans PY py-");
                py -= 0.05;
                up = 0;
                left = 0;


            }
        }
        if(windows.touchX>=800 && windows.touchX<1000){
            if(windows.touchY>=200 && windows.touchY<400){
                _LOGI("trans PY py+");
                py += 0.05;
                up = 0;
                left = 0;


            }
        }


        if(windows.touchX>=0 && windows.touchX<200){
            if(windows.touchY>=400 && windows.touchY<600){
               _LOGI("trans Rota x-");

                rx -= 2.0;
                up = 0;
                left = 0;

            }
        }
        if(windows.touchX>=800 && windows.touchX<1000){
            if(windows.touchY>=400 && windows.touchY<600){
               _LOGI("trans Rota x+");
                rx += 2.0;
                up = 0;
                left = 0;

            }
        }


        if(windows.touchX>=0 && windows.touchX<300){
            if(windows.touchY>=1000 && windows.touchY<1200){
                up +=0.02;
            }
        }

        if(windows.touchX>=300 && windows.touchX<600){
            if(windows.touchY>=1000 && windows.touchY<1200){
                up -=0.02;
            }
        }

        if(windows.touchX>=0 && windows.touchX<300){
            if(windows.touchY>=1200 && windows.touchY<1400){
                left -=0.02;
            }
        }

        if(windows.touchX>=300 && windows.touchX<600){
            if(windows.touchY>=1200 && windows.touchY<1400){
                left +=0.02;
            }
        }


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
        _LOGE("draw_frame***");
        // Draw the current frame
        draw_frame();
    }
    //return 0;
}
bool Windows::init(){

//https://gitlab.com/wikibooks-opengl/modern-tutorials
    FileRead F;
    F.print("obj/cubo4b.obj", &mesh);

    _LOGE("XXXX n mesh %hu", mesh.v.size());
    std::vector<glm::vec3>::iterator it;

    for (it = mesh.v.begin(); it != mesh.v.end(); it++){
        _LOGE("XXXX %f", it->x);

    }

    _LOGE("XXXX n mesh %d", mesh.n.size());
    std::vector<glm::vec3>::iterator it2;

    for (it2 = mesh.n.begin(); it2 != mesh.n.end(); ++it2){
        _LOGE("XXXX %f", it2->x);

    }

    _LOGE("XXXX vi mesh %d", mesh.vi.size());
    std::vector<GLushort>::iterator it3;

    for (it3 = mesh.vi.begin(); it3 != mesh.vi.end(); ++it3){
        _LOGE("XXXX index %d", *it3);

    }

   // GLfloat *c =  &mesh.v;

    //return 0;

    Asset::setAssetManager(info->app->activity->assetManager);
    m =  new ShadersManager();
    m->mAssetManager = info->app->activity->assetManager;
    std::unordered_map<GLushort , std::string> mAttrib;


    text[0] = Texture(info->app->activity->assetManager, "png/mario.png");
    text[1] = Texture(info->app->activity->assetManager, "png/elefante.png");
    text[2] = Texture(info->app->activity->assetManager, "png/mickey.png");
    text_png PNG;
    switch(_test){
        case 1:
            m->setVS("shaders/alpha_vs.glsl");
            m->setFS("shaders/alpha_fs.glsl");


            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_POS_INDEX, "aPosition"));
            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_COLOR_INDEX, "aColor"));
            break;
        case 2:


            m->setVS("shaders/betha_vs.glsl");
            m->setFS("shaders/betha_fs.glsl");


           // std::map<GLushort , std::string> mAttrib;

            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_POS_INDEX, "aPosition"));
            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_TEXCOORD0_INDEX, "aTexture"));

            //glActiveTexture(GL_TEXTURE0);
            //loadTexture1(info->app->activity->assetManager, "png/elefante.png", PNG);
            //loadTexture1(info->app->activity->assetManager, "png/mickey.png", PNG);
            //Texture(info->app->activity->assetManager, "png/mario.png");
            glBindTexture(GL_TEXTURE_2D, text[0]);

            break;

        case 3:


            m->setVS("shaders/light_vs.glsl");
            m->setFS("shaders/light_fs.glsl");


            // std::map<GLushort , std::string> mAttrib;

            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_POS_INDEX, "aPosition"));
            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_TEXCOORD0_INDEX, "aTexture"));

            //glActiveTexture(GL_TEXTURE0);
            //loadTexture1(info->app->activity->assetManager, "png/elefante.png", PNG);
            //loadTexture1(info->app->activity->assetManager, "png/mickey.png", PNG);
            //Texture(info->app->activity->assetManager, "png/mario.png");
            glBindTexture(GL_TEXTURE_2D, text[0]);

            break;
        case 4:
            _LOGE("compiling LIGHT2");
            m->setVS("shaders/light2_vs.glsl");
            m->setFS("shaders/light2_fs.glsl");
            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_POS_INDEX, "aPosition"));
            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_TEXCOORD0_INDEX, "aTexture"));
            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_NORMAL_INDEX, "aNormal"));

            lAttrib.push_back({VERTEX_POS_INDEX, VERTEX_POS_SIZE, "aPosition"});
            lAttrib.push_back({VERTEX_COLOR_INDEX, VERTEX_COLOR_SIZE, "aColor"});
            lAttrib.push_back({VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE, "aTexture"});
            lAttrib.push_back({VERTEX_NORMAL_INDEX, VERTEX_NORMAL_SIZE, "aNormal"});


            glBindTexture(GL_TEXTURE_2D, text[0]);
            break;
        case 5:
            _LOGE("compiling LIGHT5");
            m->setVS("shaders/torus_vs.glsl");
            m->setFS("shaders/torus_fs.glsl");
            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_POS_INDEX, "aPosition"));
            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_TEXCOORD0_INDEX, "aTexture"));
            mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_NORMAL_INDEX, "aNormal"));

            lAttrib.push_back({VERTEX_POS_INDEX, VERTEX_POS_SIZE, "aPosition"});
            //lAttrib.push_back({VERTEX_COLOR_INDEX, VERTEX_COLOR_SIZE, "aColor"});
            //lAttrib.push_back({VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE, "aTexture"});
            //lAttrib.push_back({VERTEX_NORMAL_INDEX, VERTEX_NORMAL_SIZE, "aNormal"});


            glBindTexture(GL_TEXTURE_2D, text[0]);
    }





    m->Program3(lAttrib);
    lProgram[0] = m->programObject;

    //MatrixID = glGetUniformLocation(m->programObject, "MVP");
    //glUseProgram( lProgram[0]);

   // MatrixID = glGetUniformLocation(programObject, "MVP");




    return true;


}

void Windows::end(){

}


void Windows::start() {
    info->app->userData = this;
    info->app->onAppCmd = handle_cmd;
    info->app->onInputEvent = handle_input;



    ActivityLoop();
}

int Windows::initDisplay() {
// Setup OpenGL ES 2
    // http://stackoverflow.com/questions/11478957/how-do-i-create-an-opengl-es-2-context-in-a-native-activity
_LOGE("initDisplay..................");
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
        _LOGE("Unable to eglMakeCurrent");
        return -1;
    }

    // Grab the width and height of the surface
    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    aspect = 1;
    if(height>width){
        aspect = (float)width/(float)height;
    }else{
        aspect = (float)height/(float)width;
    }

    _LOGE("aspect2 %f w:%d, h:%d ", aspect, width, height);
    // Initialize GL state.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //glViewport(100, 100, width, height);
    glViewport(0, 0, width, height);
    //glViewport(100, 100, 200, 200);

    return 0;
}


int Windows::closeDisplay() {
    //LOGE("--------->BYE!!!!!!!!!!!!!!!!");
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
    //LOGE("BYE!!!!!!!!!!!!!!!!");
    return 1;
}

void Windows::draw_frame() {
    // No display.
    if (display == NULL) {
        _LOGE("************************ CLOSE");
        return;
    }

    //glViewport(0, 0, 400,400);
    //glClearColor(0.1f,0.4f,0.3f, 1);
    //glClear(GL_COLOR_BUFFER_BIT);
    //LOGE("ancho = %d, alto = %d",width, height);
    //glClearColor(100, 0, 0, 1);
    //glClear(GL_COLOR_BUFFER_BIT);
    //init();


    switch(_test) {
        case 1:
            break;
        case 3:
            test1b();
            break;

        case 4:
            test1c();
            break;
        case 5:
            test1d();
            break;
    }

    return;
    ShadersManager m =  ShadersManager();
    m.mAssetManager = info->app->activity->assetManager;
    m.setVS("vs.glsl");
    m.setFS("fs.glsl");
    m.Program1();
    glClearColor(0.0f, 0.3f, 0.28f, 1.0f);
    Draw(m.programObject);
    //LOGE("---------P:%d--V:%d--F:%d------", m.programObject, m.vertexShader, m.fragmentShader);
    //eglSwapBuffers(display, surface);
    return;
}

void Windows::Draw(GLuint programObject) {
    //UserData *userData = esContext->userData;
    GLfloat vVertices1[] = {0.0f, 0.5f, 0.0f,
                            -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f};

    GLfloat vVertices2[] = {1.0f, 1.0f, 0.0f,
                            -1.0f, -1.0f, 0.0f,
                            1.0f, -1.0f, 0.0f};

    GLfloat vVertices[] = {-0.5f, 0.5f, 0.0f,
                           -0.5f, -0.5f, 0.0f,
                           0.0f, -0.5f, 0.0f,

                           0.0f, -0.5f,0.0f,
                           0.5f, -0.5f, 0.0f,
                           0.5f,0.5f, 0.0f
    };

    GLfloat vColors[] = {1.0f, 0.0f, 0.0f,
                           0.5f, 1.0f, 0.0f,
                           0.0f, 0.5f, 0.0f,

                           0.0f, 0.5f,0.0f,
                           0.5f, 0.5f, 1.0f,
                           1.5f,1.0f, 1.0f
    };


    //LOGE("[ancho] = %d, alto = %d",width, height);
    // Set the viewport
    glViewport(0, 0, width, height);
    //glViewport(0,0,400,400);
    // Clear the color buffer
    //glClearColor(0.4f,0.2f,0.3f, 1);
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    // Use the program object
    //MatrixID = glGetUniformLocation(programObject, "MVP");
    //LOGE("yanny => %d", MatrixID );
    //glUseProgram(programObject);
    //eglSwapBuffers(display, surface);
    test4();
    return;
    // Load the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);



    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, vColors);


    glDrawArrays(GL_TRIANGLES, 0, 6);
    eglSwapBuffers(display, surface);
    //LOGE("******YES******");
}

void Windows::test() {
#define VERTEX_POS_SIZE 3 // x, y and z
#define VERTEX_NORMAL_SIZE 3 // x, y and z
#define VERTEX_TEXCOORD0_SIZE 2 // s and t
#define VERTEX_POS_INDX 0
#define VERTEX_NORMAL_INDX 1
#define VERTEX_TEXCOORD0_INDX 2

    GLfloat vVertices[] = {-0.5f,  0.5f,  0.0f,  1.0f, 0.5f,  0.3f,
                           -0.5f, -0.5f,  0.0f,  0.2f, 0.5f,  0.0f,
                            0.0f, -0.5f,  0.0f,  1.0f, 0.1f,  0.3f,


    };


    GLint numVertices = 3;
    GLint vtxStride = 6*sizeof(GLfloat);
    GLuint offset = 0;
    GLuint vboIds[2];
// vboIds[0] – used to store vertex attribute data
// vboIds[1] – used to store element indices
    glGenBuffers(2, vboIds);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, vtxStride * numVertices, vVertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    //glEnableVertexAttribArray{VERTEX_TEXCOORD0_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, vtxStride, (const void*)offset);
    offset += VERTEX_POS_SIZE * sizeof(GLfloat);
    glVertexAttribPointer(VERTEX_NORMAL_INDX, VERTEX_NORMAL_SIZE,    GL_FLOAT, GL_FALSE, vtxStride,     (const void*)offset);
    /*
    offset += VERTEX_NORMAL_SIZE * sizeof(GLfloat);
    glVertexAttribPointer(VERTEX_TEXCOORD0_INDX,     VERTEX_TEXCOORD0_SIZE,    GL_FLOAT, GL_FALSE, vtxStride,    (const void*)offset);
    */

    //glBindAttribLocation(programObject, VERTEX_POS_INDX, "vPosition");
    //glBindAttribLocation(programObject, VERTEX_NORMAL_INDX, "v_color");
    //glBindAttribLocation(program, VERTEX_TEXCOORD0_INDX, "v_texcoord");
    //glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    eglSwapBuffers(display, surface);
    //LOGE("hola");
    glDeleteBuffers(2, vboIds);

}

void Windows::test2() {
#define VERTEX_POS_SIZE 3 // x, y and z
#define VERTEX_NORMAL_SIZE 3 // x, y and z
#define VERTEX_TEXCOORD0_SIZE 2 // s and t
#define VERTEX_POS_INDX 0
#define VERTEX_NORMAL_INDX 1
#define VERTEX_TEXCOORD0_INDX 2

    GLfloat vVertices[] = {-0.5f,  0.5f,  0.0f,  1.0f, 0.0f,  1.0f,
                           -0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  1.0f,
                           0.0f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f,


    };

    GLushort indices[] = {0,1,2};
    GLint numIndices=3;
    GLint numVertices = 3;
    GLint vtxStride = 6*sizeof(GLfloat);
    GLuint offset = 0;
    GLuint vboIds[2];
// vboIds[0] – used to store vertex attribute data
// vboIds[1] – used to store element indices
    glGenBuffers(2, vboIds);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, vtxStride * numVertices, vVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    //glEnableVertexAttribArray{VERTEX_TEXCOORD0_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, vtxStride, (const void*)offset);
    offset += VERTEX_POS_SIZE * sizeof(GLfloat);
    glVertexAttribPointer(VERTEX_NORMAL_INDX, VERTEX_NORMAL_SIZE,    GL_FLOAT, GL_FALSE, vtxStride,     (const void*)offset);
    /*
    offset += VERTEX_NORMAL_SIZE * sizeof(GLfloat);
    glVertexAttribPointer(VERTEX_TEXCOORD0_INDX,     VERTEX_TEXCOORD0_SIZE,    GL_FLOAT, GL_FALSE, vtxStride,    (const void*)offset);
    */

    //glBindAttribLocation(programObject, VERTEX_POS_INDX, "vPosition");
    //glBindAttribLocation(programObject, VERTEX_NORMAL_INDX, "v_color");
    //glBindAttribLocation(program, VERTEX_TEXCOORD0_INDX, "v_texcoord");
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    eglSwapBuffers(display, surface);
    //LOGE("hola 2");
    glDeleteBuffers(2, vboIds);
}

void Windows::test3() {
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
            glm::vec3(0+px,0+py,-4), // Camera is at (4,3,-3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);
    //LOGI("aspect1 %d", aspect);
    Model = glm::scale(Model,glm::vec3(1.0f,1.0f*aspect,1.0f));
    Model = glm::rotate(Model,glm::radians(rx),glm::vec3(ex,ey,ez));
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = Projection * View * Model ; // Remember, matrix multiplication is the other way around

    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
    };

    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data1[] = {
            0.583f,  0.771f,  0.014f,
            0.609f,  0.115f,  0.436f,
            0.327f,  0.483f,  0.844f,
            0.822f,  0.569f,  0.201f,
            0.435f,  0.602f,  0.223f,
            0.310f,  0.747f,  0.185f,

            0.597f,  0.770f,  0.761f,
            0.559f,  0.436f,  0.730f,
            0.359f,  0.583f,  0.152f,
            0.483f,  0.596f,  0.789f,
            0.559f,  0.861f,  0.639f,
            0.195f,  0.548f,  0.859f,

            0.014f,  0.184f,  0.576f,
            0.771f,  0.328f,  0.970f,
            0.406f,  0.615f,  0.116f,
            0.676f,  0.977f,  0.133f,
            0.971f,  0.572f,  0.833f,
            0.140f,  0.616f,  0.489f,

            0.997f,  0.513f,  0.064f,
            0.945f,  0.719f,  0.592f,
            0.543f,  0.021f,  0.978f,
            0.279f,  0.317f,  0.505f,
            0.167f,  0.620f,  0.077f,
            0.347f,  0.857f,  0.137f,

            0.055f,  0.953f,  0.042f,
            0.714f,  0.505f,  0.345f,
            0.783f,  0.290f,  0.734f,
            0.722f,  0.645f,  0.174f,
            0.302f,  0.455f,  0.848f,
            0.225f,  0.587f,  0.040f,

            0.517f,  0.713f,  0.338f,
            0.053f,  0.959f,  0.120f,
            0.393f,  0.621f,  0.362f,
            0.673f,  0.211f,  0.457f,
            0.820f,  0.883f,  0.371f,
            0.982f,  0.099f,  0.879f
    };

    static const GLfloat g_color_buffer_data[] = {
            0.999f,  0.000f,  0.999f,//5
            0.999f,  0.000f,  0.999f,//5
            0.999f,  0.000f,  0.999f,//5
            0.222f,  0.222f,  0.222f,//2
            0.222f,  0.222f,  0.222f,//2
            0.222f,  0.222f,  0.222f,//2

            0.999f,  0.200f,  0.000f,//4
            0.999f,  0.200f,  0.000f,//4
            0.999f,  0.200f,  0.000f,//4
            0.222f,  0.222f,  0.222f,//2
            0.222f,  0.222f,  0.222f,//2
            0.222f,  0.222f,  0.222f,//2

            0.999f,  0.000f,  0.999f,//5
            0.999f,  0.000f,  0.999f,//5
            0.999f,  0.000f,  0.999f,//5
            0.999f,  0.200f,  0.000f,//4
            0.999f,  0.200f,  0.000f,//4
            0.999f,  0.200f,  0.000f,//4

            0.000f,  0.999f,  0.000f,//6
            0.000f,  0.999f,  0.000f,//6
            0.000f,  0.999f,  0.000f,//6
            0.999f,  0.999f,  0.000f,//3
            0.999f,  0.999f,  0.000f,//3
            0.999f,  0.999f,  0.000f,//3

            0.999f,  0.999f,  0.000f,//3
            0.999f,  0.999f,  0.000f,//3
            0.999f,  0.999f,  0.000f,//3
            0.000f,  0.999f,  0.999f,//1
            0.000f,  0.999f,  0.999f,//1
            0.000f,  0.999f,  0.999f,//1

            0.000f,  0.999f,  0.999f,//1
            0.000f,  0.999f,  0.999f,//1
            0.000f,  0.999f,  0.999f,//1
            0.000f,  0.999f,  0.000f,//6
            0.000f,  0.999f,  0.000f,//6
            0.000f,  0.999f,  0.000f,//6

    };

    //LOGE("yanny => %d", MatrixID );
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles



    eglSwapBuffers(display, surface);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
}

void Windows::test4() {


    GLfloat vVertices[] = {-0.5F,  0.5f,  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
                           -0.5F, -0.5f,  0.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,
                           0.0F, -0.5f,  0.0f,  0.0f, 0.0f,  0.6f, 1.0f, 1.0f


    };

    GLushort indices[] = {0,1,2};
    GLint numIndices=3;
    GLint numVertices = 3;
    GLint vtxStride = 8*sizeof(GLfloat);
    GLuint offset = 0;
    GLuint vboIds[2];
// vboIds[0] – used to store vertex attribute data
// vboIds[1] – used to store element indices
    glGenBuffers(2, vboIds);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, vtxStride * numVertices, vVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, indices, GL_STATIC_DRAW);



    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_NORMAL_INDX);
    glEnableVertexAttribArray(VERTEX_TEXCOORD0_INDX);

    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, vtxStride, (const void*)offset);
    offset += VERTEX_POS_SIZE * sizeof(GLfloat);
    glVertexAttribPointer(VERTEX_NORMAL_INDX, VERTEX_NORMAL_SIZE,    GL_FLOAT, GL_FALSE, vtxStride,     (const void*)offset);
    /**/
    offset += VERTEX_NORMAL_SIZE * sizeof(GLfloat);
    glVertexAttribPointer(VERTEX_TEXCOORD0_INDX,     VERTEX_TEXCOORD0_SIZE,    GL_FLOAT, GL_FALSE, vtxStride,    (const void*)offset);

    text_png PNG;
    //glActiveTexture(GL_TEXTURE0);
    loadTexture1(info->app->activity->assetManager, "png/elefante.png", PNG);


    //glBindAttribLocation(programObject, VERTEX_POS_INDX, "vPosition");
    //glBindAttribLocation(programObject, VERTEX_NORMAL_INDX, "v_color");
    //glBindAttribLocation(program, VERTEX_TEXCOORD0_INDX, "v_texcoord");

    //glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    eglSwapBuffers(display, surface);
    //LOGE("hola 2");
    glDeleteBuffers(2, vboIds);
}

void Windows::test1a() {
static int k1=0;
if(k1>0)
    return;

k1=1;


    text_png PNG;
    //glActiveTexture(GL_TEXTURE0);
    loadTexture1(info->app->activity->assetManager, "png/mario.png", PNG);


    glViewport(0, 0, width, height);

    glClearColor(0.0f, 1.0f, 0.28f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
            glm::vec3(0+px,0+py,5), // Camera is at (4,3,-3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);
    //LOGI("aspect1 %d", aspect);
    Model = glm::scale(Model,glm::vec3(1.0f,1.0f*aspect,1.0f));
    Model = glm::rotate(Model,glm::radians(rx),glm::vec3(ex,ey,ez));
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = Projection * View * Model ; // Remember, matrix multiplication is the other way around



    ShadersManager m =  ShadersManager();
    m.mAssetManager = info->app->activity->assetManager;
    m.setVS("shaders/test1a.vs");
    m.setFS("shaders/test1a.fs");


    std::unordered_map<GLushort , std::string> mAttrib;

    mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_POS_INDEX, "aPosition"));
    mAttrib.insert(std::pair<GLushort, std::string>(VERTEX_TEXCOORD0_INDEX, "aTexture"));
    m.Program2(mAttrib);

    GLuint programObject = m.programObject;
    MatrixID = glGetUniformLocation(programObject, "MVP");

    _LOGI("matrixid %d", MatrixID);


    glUseProgram(programObject);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    std::unordered_map<GLushort , GLushort> defAttrib;

    defAttrib.insert(std::pair<GLushort, GLushort>(VERTEX_POS_INDEX, VERTEX_POS_SIZE));
    defAttrib.insert(std::pair<GLushort, GLushort>(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE));
    const float delX = 0.2;
    const float del1 = 0.2;
    const float del0 = 0.0;

    GLfloat vVertices[] = {
            0.0,  0.0,  del0, 0.0,0.0,
            0.5,  0.0,  del0, 1.0, 0.0,
            0.5,  0.5,  del0, 1.0,1.0,
            0.0,  0.5,  0.0, 0.0,1.0,
    };
    GLushort  indices[] = {0,1,2,0,2,3};
    GLHelper fig;
    fig.setVertices(vVertices, 4, 5);
    fig.setIndices(indices, 6);
    fig.defAttrib(defAttrib);

    //LOGE("hola 2");
    //glDeleteBuffers(2, vboIds);
    //
    fig.draw();
    GLfloat dx = 0.4;
    GLfloat dy = 0.4;
    GLfloat vVertices2[] = {
            0.4,  0.4,  del0, 0.0,0.0,
            0.9,  0.4,  del0, 1.0, 0.0,
            0.9,  0.9,  del0, 1.0,1.0,
            0.4,  0.9,  0.0, 0.0,1.0,
    };


    fig.setVertices(vVertices2, 4, 5);

    //text_png PNG;
    //glActiveTexture(GL_TEXTURE0);
    loadTexture1(info->app->activity->assetManager, "png/mickey.png", PNG);

    fig.draw();

    GLfloat vVertices3[] = {
            0.2,  0.2,  del0, 0.0,0.0,
            0.7,  0.2,  del0, 1.0, 0.0,
            0.7,  0.7,  del0, 1.0,1.0,
            0.2,  0.7,  0.0, 0.0,1.0,
    };


    fig.setVertices(vVertices3, 4, 5);

    fig.draw();
    eglSwapBuffers(display, surface);


}

void Windows::test1b() {






    glClearColor(0.9f, 0.8f, 0.0f, 1.0f);
    glClearColor(0.2f, 0.6f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);




    if(isMVP){

        MatrixID = glGetUniformLocation(lProgram[0], "MVP");
        glUseProgram( lProgram[0]);





        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
        // Camera matrix
        glm::mat4 View       = glm::lookAt(
                glm::vec3(0,0,2), // Camera is at (4,3,-3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model      = glm::mat4(1.0f);
        //LOGI("aspect1 %d", aspect);
        Model = glm::scale(Model,glm::vec3(1.0f,1.0f*aspect,1.0f));
        Model = glm::rotate(Model,glm::radians(rx),glm::vec3(ex,ey,ez));

        Model = glm::translate(Model, glm::vec3(0+left,0+up,0));
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP        = Projection * View * Model ; // Remember, matrix multiplication is the other way around

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    }else{
        glUseProgram( lProgram[0]);
    }



    //glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    std::unordered_map<GLushort , GLushort> defAttrib;
    GLHelper fig;
    //int _test = 2;
    GLfloat vVertices1[] = {
            0.0,  0.0,  0.0, 0.0,1.0,0.0,
            0.5,  0.0,  0.0, 0.0,1.0,0.0,
            0.5,  0.5,  0.0, 0.0,1.0,0.8,
            0.0,  0.5,  0.0, 0.0,1.0,0.5,
    };
    GLfloat vVertices[] = {
            0.0,  0.0,  0.0, 0.0,0.0, 0.0,
            0.5,  0.0,  0.0, 1.0,0.0, 0.0,
            0.5,  0.5,  0.0, 1.0,1.0, 0.0,
            0.0,  0.5,  0.0, 0.0,1.0, 0.0,
    };
    GLushort  indices1[] = {0,1,2,0,2,3};
    GLushort  indices[] = {0,1,2,0,2,3};


    switch(_test){
        case 1:
            defAttrib.insert(std::pair<GLushort, GLushort>(VERTEX_POS_INDEX, VERTEX_POS_SIZE));
            defAttrib.insert(std::pair<GLushort, GLushort>(VERTEX_COLOR_INDEX, VERTEX_COLOR_SIZE));
            fig.setVertices(vVertices1, 4, 6);
            fig.setIndices(indices1, 6);
            break;
        case 2:
            defAttrib.insert(std::pair<GLushort, GLushort>(VERTEX_POS_INDEX, VERTEX_POS_SIZE));
            defAttrib.insert(std::pair<GLushort, GLushort>(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE));
            fig.setVertices(vVertices, 4, 6);
            fig.setIndices(indices, 6);
            break;


    }





    fig.defAttrib(defAttrib);





    fig.draw();
    eglSwapBuffers(display, surface);


}

void Windows::loadTexture(){
    png_uint_32 width, height;

        return;
    }


void Windows::test1c() {

   // _LOGE("compiling  test1c PINK");
    static long step=0;
    static long tn=0;


    if(step%100==0){
        tn = (tn+1) % 3;
        //glBindTexture(GL_TEXTURE_2D, text[tn]);
        ry+=10;


    }
    step++;



    glClearColor(0.9f, 0.8f, 0.0f, 1.0f);
    glClearColor(0.2f, 0.6f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);




    if(isMVP){

        MatrixID = glGetUniformLocation(lProgram[0], "MVP");
        glUseProgram( lProgram[0]);





        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
        // Camera matrix
        glm::mat4 View       = glm::lookAt(
                glm::vec3(0,0,6), // Camera is at (4,3,-3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model      = glm::mat4(1.0f);
        //LOGI("aspect1 %d", aspect);
        Model = glm::scale(Model,glm::vec3(1.0f,1.0f*aspect,1.0f));
        Model = glm::rotate(Model,glm::radians(rx),glm::vec3(ex,ey,ez));

        Model = glm::translate(Model, glm::vec3(0+left,0+up,0));
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP        = Projection * View * Model ; // Remember, matrix multiplication is the other way around

        glm::mat4 ModelViewMatrix = View * Model;
        glm::mat4 NormalMatrix = View * glm::transpose(glm::inverse(Model));


        GLuint ModelViewMatrixID = glGetUniformLocation(lProgram[0], "ModelViewMatrix");
        GLuint NormalMatrixID = glGetUniformLocation(lProgram[0], "NormalMatrix");

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glUniformMatrix4fv(ModelViewMatrixID, 1, GL_FALSE, &ModelViewMatrix[0][0]);
        glUniformMatrix4fv(NormalMatrixID, 1, GL_FALSE, &NormalMatrix[0][0]);

    }else{
        glUseProgram( lProgram[0]);
    }
    std::unordered_map<GLushort , GLushort> defAttrib;
    GLHelper fig;

    int light = 1;
    if(light*2*3 == 0){
        GLuint MaterialAmbient = glGetUniformLocation(lProgram[0], "MaterialAmbient");
        GLuint LightAmbient = glGetUniformLocation(lProgram[0], "LightAmbient");

        // Set Red colored material
        if (MaterialAmbient >= 0)
        { glUniform3f(MaterialAmbient, 1.0f, 0.0f, 0.0f); }
        // Set white light
        if (LightAmbient >= 0)
        { glUniform3f(LightAmbient, 1.0f, 1.0f, 1.0f); }


        GLfloat vVertices1[] = {
                0.0,  0.0,  0.0, 0.0,1.0,0.0,
                0.5,  0.0,  0.0, 0.0,1.0,0.0,
                0.5,  0.5,  0.0, 0.0,1.0,0.8,
                0.0,  0.5,  0.0, 0.0,1.0,0.5,
        };
        GLfloat vVertices[] = {
                0.0,  0.0,  0.0, 0.0,0.0, 0.0,
                0.5,  0.0,  0.0, 1.0,0.0, 0.0,
                0.5,  0.5,  0.0, 1.0,1.0, 0.0,
                0.0,  0.5,  0.0, 0.0,1.0, 0.0,
        };
        GLushort  indices1[] = {0,1,2,0,2,3};
        GLushort  indices[] = {0,1,2,0,2,3};


        defAttrib.insert(std::pair<GLushort, GLushort>(VERTEX_POS_INDEX, VERTEX_POS_SIZE));
        defAttrib.insert(std::pair<GLushort, GLushort>(VERTEX_TEXCOORD0_INDEX, VERTEX_TEXCOORD0_SIZE));
        fig.setVertices(vVertices, 4, 6);
        fig.setIndices(indices, 6);
    }else if(light == 1){
        GLuint MaterialDiffuse = glGetUniformLocation(lProgram[0], "MaterialDiffuse");
        GLuint LightDiffuse = glGetUniformLocation(lProgram[0], "LightDiffuse");
        GLuint LightPosition = glGetUniformLocation(lProgram[0], "LightPosition");


        // Set Red colored diffuse material uniform
        //glm::vec3 color = glm::vec3(1.0, 0.0, 0.0);
        if (MaterialDiffuse >= 0) {
            glUniform3f(MaterialDiffuse,1.0, 0.8, 0.8);
        }
        // Set white diffuse light
        if (LightDiffuse >= 0) {
            glUniform3f(LightDiffuse, 1.0f, 1.0f, 1.0f);
        }

        // Set light position
        //glm::vec3 lightPosition(0.0, 0.0, 6.0);
        //glUniform3fv(LightPosition,1,(float*)&lightPosition);
        if (LightPosition >= 0) {
            glUniform3f(LightPosition, 0.0f, 0.0f,-4.0f);
        }
       // _LOGE("compiling PINK");


       GLfloat z = 0.0;
        GLfloat vVertices[] = {
                0.0,  0.0,  z,   0.0,0.0,   0.0, 0.0, 1.0,
                0.5,  0.0,  z,   1.0,0.0,   0.0, 0.0, 1.0,
                0.5,  0.5,  z,   1.0,1.0,   0.0, 0.0, 1.0,
                0.0,  0.5,  z,   0.0,1.0,   0.0, 0.0, 1.0,
        };

        GLushort  indices[] = {0,1,2,0,2,3};


        GLfloat p = 0.5;
        GLfloat cubo[] = {
                -p,-p, p, 1.0, 0.0, 0.0,0.0,0.0,   0.0, 0.0, p,
                 p,-p, p, 1.0, 0.0, 0.0,1.0,0.0,   0.0, 0.0, p,
                 p, p, p, 1.0,0.0,0.0,1.0,1.0,   0.0, 0.0, p,
                -p, p, p, 1.0,0.0,0.5,0.0,1.0,   0.0, 0.0, p,


                -p,-p, -p, 1.0, 0.0, 1.0,0.0,0.0,   0.0, 0.0, -p,
                p,-p, -p, 1.0, 0.0, 0.0,1.0,0.0,   0.0, 0.0, -p,
                p, p, -p, 1.0,0.0,0.0,1.0,1.0,   0.0, 0.0, -p,
                -p, p, -p, 1.0,0.0,0.5,0.0,1.0,   0.0, 0.0, -p,
        };
        GLushort  indices2[] = {0,1,2,0,2,3,
                                1,5,6, 6,2,1,

                // back
                                7, 6, 5,
                                5, 4, 7,
                // left
                                4, 0, 3,
                                3, 7, 4,
                // bottom
                                4, 5, 1,
                                1, 0, 4,
                // top
                                3, 2, 6,
                                6, 7, 3


        };

        fig.setVertices(cubo, 8, 11);
        fig.setIndices(indices2, 6*5);
    }


    fig.defAttrib2(lAttrib);

    fig.draw2();
    eglSwapBuffers(display, surface);


}

void Windows::test1d() {
   // glClearColor(0.9f, 0.8f, 0.0f, 1.0f);
    glClearColor(0.2f, 0.6f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
   // glDepthFunc(GL_LESS);

    MatrixID = glGetUniformLocation(lProgram[0], "MVP");
    glUseProgram( lProgram[0]);
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
            glm::vec3(0,0,8), // Camera is at (4,3,-3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);
    //LOGI("aspect1 %d", aspect);
    Model = glm::scale(Model,glm::vec3(1.0f,1.0f*aspect,1.0f));
    Model = glm::rotate(Model,glm::radians(rx),glm::vec3(ex,ey,ez));

    Model = glm::translate(Model, glm::vec3(0+left,0+up,0));
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = Projection * View * Model ; // Remember, matrix multiplication is the other way around

   glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    GLHelper fig;

    fig.setVertices(mesh.v, mesh.v.size(), 3);
    fig.setIndices(mesh.vi, mesh.vi.size());
    fig.defAttrib2(lAttrib);

    fig.draw4();
    eglSwapBuffers(display, surface);

}


