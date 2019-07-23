//
// Created by yanny on 17/6/2019.
//

#ifndef RINCONADA_HEADER_H
#define RINCONADA_HEADER_H

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cstdlib>
#include "Log.h"

GLuint LoadShader( GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;
    // Create the shader object
    shader = glCreateShader(type);
    if(shader == 0)
        return 0;
    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);
    // Compile the shader
    glCompileShader(shader);
    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            char* infoLog = (char*) malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            //esLogMessage("Error compiling shader:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

const char * leer(AAssetManager* AA, const char * pPath){

    const char* mPath;
    AAssetManager* mAssetManager;
    AAsset* mAsset;
    mPath = "vs.glsl";
    mAssetManager = AA;
    mAsset = AAssetManager_open(mAssetManager, pPath, AASSET_MODE_UNKNOWN);
    if(mAsset!= NULL){
       // LOGW("------------COOOOOLLLLL----------");
    }else{
        //LOGW("ERRRRRRRRRRRRRRROOORRRRRRRRR");
    }

    off_t fileLength = AAsset_getLength(mAsset);
    char *dataBuffer2 = (char *) malloc(fileLength);

    AAsset_read(mAsset, dataBuffer2, fileLength);
//the data has been copied to dataBuffer2, so , close it
    AAsset_close(mAsset);


    //LOGW("%s",(const char*)dataBuffer2);
    return dataBuffer2;
    //free(dataBuffer2);

}

#endif //RINCONADA_HEADER_H
