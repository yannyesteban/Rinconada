//
// Created by yanny on 18/6/2019.
//

#include <cstdlib>
#include <android/asset_manager.h>
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
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = (char*) malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            //esLogMessage("Error compiling shader:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
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
    glBindAttribLocation(programObject, 1, "v_color");
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

int ShadersManager::setVS(const char *name) {
    vertexShader = Load(GL_VERTEX_SHADER, ReadFile(mAssetManager, name));
    return 1;
}
int ShadersManager::setFS(const char *name) {
    fragmentShader = Load(GL_FRAGMENT_SHADER, ReadFile(mAssetManager, name));
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
        LOGW("------------COOOOOLLLLL----------");
    }else{
        LOGW("ERRRRRRRRRRRRRRROOORRRRRRRRR");
    }

    off_t fileLength = AAsset_getLength(mAsset);
    char *dataBuffer = (char *) malloc(fileLength);

    AAsset_read(mAsset, dataBuffer, fileLength);
    //the data has been copied to dataBuffer2, so , close it
    AAsset_close(mAsset);


    //LOGW("%s",(const char*)dataBuffer);
    return dataBuffer;
    //free(dataBuffer2);

}