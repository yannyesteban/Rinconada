//
// Created by yanny on 18/6/2019.
//

#ifndef RINCONADA_SHADERSMANAGER_H
#define RINCONADA_SHADERSMANAGER_H

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class ShadersManager {
public:
    ShadersManager();
    GLuint Load( GLenum type, const char *shaderSrc);
    int createProgram(const char* vShaderStr, const char* fShaderStr);
    const char * ReadFile(AAssetManager* AA, const char * pPath);
    int Program1();
    GLuint programObject;
    GLuint vertexShader;
    GLuint fragmentShader;

    int setVS(const char *);
    int setFS(const char *);
    AAssetManager *mAssetManager;
private:
};


#endif //RINCONADA_SHADERSMANAGER_H
