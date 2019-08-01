//
// Created by yanny on 18/6/2019.
//

#include <cstdlib>
#include <gestureDetector.h>
#include <android/asset_manager.h>
#include "Asset.h"
#include "ShadersManager.h"
#include "Log.h"
ShadersManager::ShadersManager(){

    programObject = glCreateProgram();

}

GLuint ShadersManager::Load( GLenum type, const char *shaderSrc) {

    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);
    if(shader == 0){
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, nullptr);
    // Compile the shader
    glCompileShader(shader);
    // Check the compile status

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(!compiled) {
        _LOGE("compile OK(BAD) %s", shaderSrc);
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = (char*) malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            _LOGE("Error Length %d, compiling shader: %s",infoLen, infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    _LOGE("compile OK %s", shaderSrc);
    return shader;
}

int ShadersManager::Program1(){
    if(programObject == 0) {
        return 0;
    }

    GLint linked;


    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    // Bind vPosition to attribute 0
    glBindAttribLocation(programObject, 0, "vPosition");
    //glBindAttribLocation(programObject, 1, "v_color");
    glBindAttribLocation(programObject, 2, "aTexture");


/*
    GLuint kk = glGetUniformLocation(programObject, "KKK");
    LOGE("yanny kkk => %d", kk );
*/
    //glUniform4fv(location, coord);
    // Link the program
    glLinkProgram(programObject);



    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if(!linked) {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            //esLogMessage("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return false;
    }
    //GLuint MatrixID = glGetUniformLocation(programObject, "MVP");
    //LOGE("yanny MVP => %d", MatrixID );
    return true;
}


int ShadersManager::Program2(std::unordered_map<GLushort , std::string> pAttrib){
    _LOGE("Program2 dos");
    if(programObject == 0) {
        return 0;
    }

    GLint linked;

    glAttachShader(programObject, vertexShader);
   // _LOGE("shader %s %s", programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    std::unordered_map<GLushort, std::string>::iterator it;
    _LOGE("yanny Bien 2");
    for (it = pAttrib.begin(); it != pAttrib.end(); ++it) {
        glBindAttribLocation(programObject, it->first, it->second.c_str());
        _LOGE("YANNY glBindAttribLocation %d, %s",it->first, it->second.c_str());

    }

    glLinkProgram(programObject);
int n;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
    _LOGE("compiling GL_MAX_VERTEX_ATTRIBS %d", n);
    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if(!linked) {
        _LOGE("YANNY ERROR ONE");
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            //esLogMessage("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return false;
    }
    //GLuint MatrixID = glGetUniformLocation(programObject, "MVP");
    //LOGE("yanny MVP => %d", MatrixID );

    _LOGE("yanny Bien");
    return true;
}

int ShadersManager::Program3(std::list<GLAttrib> pAttrib){
    _LOGE("Program2 dos");
    if(programObject == 0) {
        return 0;
    }

    GLint linked;

    glAttachShader(programObject, vertexShader);
    // _LOGE("shader %s %s", programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    std::list<GLAttrib>::iterator it;
    _LOGE("yanny Bien 2");
    for (it = pAttrib.begin(); it != pAttrib.end(); ++it) {
        glBindAttribLocation(programObject, it->index, it->name.c_str());
        _LOGE("YANNY glBindAttribLocation %d, %s",it->index, it->name.c_str());

    }

    glLinkProgram(programObject);
    int n;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
    _LOGE("compiling GL_MAX_VERTEX_ATTRIBS %d", n);
    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if(!linked) {
        _LOGE("YANNY ERROR ONE");
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            //esLogMessage("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return false;
    }
    //GLuint MatrixID = glGetUniformLocation(programObject, "MVP");
    //LOGE("yanny MVP => %d", MatrixID );

    _LOGE("yanny Bien");
    return true;
}

int ShadersManager::setVS(const char *name) {




    vertexShader = ReadShader(GL_VERTEX_SHADER, name);

    return 1;
}
int ShadersManager::setFS(const char *name) {
    fragmentShader = ReadShader(GL_FRAGMENT_SHADER, name);
    return 1;
}
int ShadersManager::createProgram(const char* vShaderStr, const char* fShaderStr) {

    GLuint vertexShader;
    GLuint fragmentShader;
    //GLuint programObject;
    GLint linked;
    // Load the vertex/fragment shaders
    vertexShader = Load(GL_VERTEX_SHADER, vShaderStr);
    fragmentShader = Load(GL_FRAGMENT_SHADER, fShaderStr);
    // Create the program object
    programObject = glCreateProgram();
    if(programObject == 0) {
        return 0;
    }
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    // Bind vPosition to attribute 0
    glBindAttribLocation(programObject, 0, "vPosition");
    // Link the program
    glLinkProgram(programObject);
    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if(!linked) {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            //esLogMessage("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return false;
    }

    return true;
}

const char * ShadersManager::ReadFile(AAssetManager* mAssetManager, const char * pPath){

    AAsset* mAsset;
    mAsset = AAssetManager_open(mAssetManager, pPath, AASSET_MODE_UNKNOWN);

    if(mAsset!= NULL){
        //LOGW("------------COOOOOLLLLL----------");
    }else{
        _LOGE("YANNY ERRRRRRRRRRRRRRROOORRRRRRRRR");
    }

    off_t fileLength = AAsset_getLength(mAsset);
    char *dataBuffer = (char *) malloc(fileLength);
    memset(dataBuffer, 0, fileLength);
    AAsset_read(mAsset, dataBuffer, fileLength);
    //the data has been copied to dataBuffer2, so , close it
    AAsset_close(mAsset);
    _LOGE("YANNY BUFFER *************");
    _LOGE("YANNY BUFFER %s, tamaño %d",pPath, fileLength);
    //_LOGE("YANNY BUFFER %s",(const char*)dataBuffer);
    _LOGE("YANNY BUFFER -------------");
    return dataBuffer;
    //free(dataBuffer2);

}

ShadersManager::~ShadersManager() {
    glDeleteProgram(programObject);
}

GLuint ShadersManager::ReadShader(GLenum type, const char *pPath) {

    Asset asset = Asset(pPath);


    asset.open();
    off_t fileLength = asset.getLength();
    char *shaderSrc = (char *) malloc(sizeof(char)*fileLength+1);

    int ii = asset.read((char *)shaderSrc, fileLength);
    shaderSrc[fileLength] = '\0';
    _LOGE("44444 ?(ii %d) %d, %s", ii, fileLength,shaderSrc);
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);
    if(shader == 0){
        free(shaderSrc);
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, nullptr);
    // Compile the shader
    glCompileShader(shader);
    // Check the compile status

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(!compiled) {
        free(shaderSrc);
        _LOGE("compile 44444 OK(BAD) %s", shaderSrc);
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = (char*) malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            _LOGE("Error Length %d, compiling shader: %s",infoLen, infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    _LOGE("compile OK %s", shaderSrc);
    //

    free(shaderSrc);
    return shader;
}
