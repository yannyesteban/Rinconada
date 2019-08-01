//
// Created by yanny on 18/6/2019.
//

#ifndef RINCONADA_SHADERSMANAGER_H
#define RINCONADA_SHADERSMANAGER_H
#include <map>
#include <unordered_map>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <list>
#include "types.h"

class ShadersManager {
public:
    ShadersManager();
    ~ShadersManager();
    GLuint Load( GLenum type, const char *shaderSrc);
    int createProgram(const char* vShaderStr, const char* fShaderStr);
    const char * ReadFile(AAssetManager* AA, const char * pPath);
    int Program1();
    int Program2(std::unordered_map<GLushort, std::string> pAttrib);
    int Program3(std::list<GLAttrib> pAttrib);
    GLuint programObject;
    GLuint vertexShader;
    GLuint fragmentShader;

    GLuint ReadShader(GLenum type, const char * pPath);

    int setVS(const char *);
    int setFS(const char *);
    AAssetManager *mAssetManager;

private:
};


#endif //RINCONADA_SHADERSMANAGER_H
