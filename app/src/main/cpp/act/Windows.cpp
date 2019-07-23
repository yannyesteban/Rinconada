//
// Created by yanny on 14/6/2019.
//


#include <cstdlib>
#include <JNIHelper.h>
#include<gestureDetector.h>
#include "Log.h"
#include "Windows.h"
#include "Header.h"
#include "ShadersManager.h"
#include <png.h>
#include "loadpng.h"


#define VERTEX_POS_SIZE 3 // x, y and z
#define VERTEX_NORMAL_SIZE 3 // x, y and z
#define VERTEX_TEXCOORD0_SIZE 2 // s and t
#define VERTEX_POS_INDX 0
#define VERTEX_NORMAL_INDX 1
#define VERTEX_TEXCOORD0_INDX 2

GLuint MatrixID;
GLfloat px=0;
GLfloat py=0;
GLfloat rx=0.0f;
GLfloat ry=0.0f;
GLfloat rz=0.0f;

GLfloat ex=1.0f;
GLfloat ey=0.0f;
GLfloat ez=0.0f;
GLfloat aspect=1.0f;

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



    ndk_helper::DragDetector drag;
    ndk_helper::GESTURE_STATE xxx = drag.Detect(event);
    //ndk_helper::Vec2 v2;
    //drag.GetPointer(v2);
    //v2.Dump();
    //LOGI("iii %d", v2.);
    LOGII("gesture xxx %d", xxx);

    Windows & windows = *(Windows*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        windows.touchX = AMotionEvent_getX(event, 0);
        windows.touchY = AMotionEvent_getY(event, 0);
        LOGII("TOCA AQUI x %d\ty %d\n",windows.touchX, windows.touchY);
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
                //LOGI("trans PX px-");
                px -= 0.05;

            }
        }
        if(windows.touchX>=800 && windows.touchX<1000){
            if(windows.touchY>=0 && windows.touchY<200){
               // LOGI("trans PX px+");
                px += 0.05;
            }
        }


        if(windows.touchX>=0 && windows.touchX<200){
            if(windows.touchY>=200 && windows.touchY<400){
                //LOGI("trans PY py-");
                py -= 0.05;

            }
        }
        if(windows.touchX>=800 && windows.touchX<1000){
            if(windows.touchY>=200 && windows.touchY<400){
                //LOGI("trans PY py+");
                py += 0.05;

            }
        }


        if(windows.touchX>=0 && windows.touchX<200){
            if(windows.touchY>=400 && windows.touchY<600){
               // LOGI("trans Rota x-");

                rx -= 2.0;
            }
        }
        if(windows.touchX>=800 && windows.touchX<1000){
            if(windows.touchY>=400 && windows.touchY<600){
               //LOGI("trans Rota x+");
                rx += 2.0;
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

        // Draw the current frame
        draw_frame();
    }
    //return 0;
}
bool Windows::init(){

    const char* vShaderStr = leer(info->app->activity->assetManager, "vs.glsl");
    const char* fShaderStr = leer(info->app->activity->assetManager, "fs.glsl");
/*
    const char* vShaderStr =
            "attribute vec4 vPosition; \n"
            "void main() \n"
            "{ \n"
            " gl_Position = vPosition; \n"
            "} \n";
    const char* fShaderStr =
            "precision mediump float; \n"
            "void main() \n"
            "{ \n"
            " gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
            "} \n";
*/

    GLuint vertexShader;
    GLuint fragmentShader;
    //GLuint programObject;
    GLint linked;
// Load the vertex/fragment shaders
    vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);
// Create the program object
    programObject = glCreateProgram();

    if(programObject == 0){
        //LOGE("--ONE--------->");
        return 0;
    }
    //LOGE("--PASS--------->");
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    // Bind vPosition to attribute 0
    glBindAttribLocation(programObject, 0, "vPosition");
    glBindAttribLocation(programObject, 1, "v_color");
    // Link the program
    glLinkProgram(programObject);
    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            //esLogMessage("Error linking program:\n%s\n", infoLog);
            //LOGE("\n\n(((%s)))\n\n", infoLog);
            free(infoLog);
        }else{
            //LOGE("\n\n(==========PASÓ SIIII==========)\n\n");

        }
        glDeleteProgram(programObject);
        //LOGE("--TWO--------->");
        return false;
    }else{
       // LOGE("\n\n(==========PASÓ SIIII (2)==========)\n\n");
    }
// Store the program object
    //programObject = programObject;
    //LOGE("programa:::: %d", programObject);
    //LOGE("BIENNNNNNNNNNNNN--------->");
    glClearColor(0.0f, 0.0f, 0.28f, 1.0f);
    return true;

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
        //LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    // Grab the width and height of the surface
    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    if(height>width){
        aspect = (float)width/(float)height;
    }else{
        aspect = (float)height/(float)width;
    }

   // LOGI("aspect2 %f w:%d, h:%d ", aspect, width, height);
    // Initialize GL state.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glViewport(100, 100, width, height);
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
       // LOGE("************************ CLOSE");
        return;
    }

    //glViewport(0, 0, 400,400);
    //glClearColor(0.1f,0.4f,0.3f, 1);
    //glClear(GL_COLOR_BUFFER_BIT);
    //LOGE("ancho = %d, alto = %d",width, height);
    glClearColor(100,0,0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    //init();

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
    MatrixID = glGetUniformLocation(programObject, "MVP");
    //LOGE("yanny => %d", MatrixID );
    glUseProgram(programObject);
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



void Windows::loadTexture(){
    png_uint_32 width, height;

        return;
    }


