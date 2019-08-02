//
// Created by yanny on 26/7/2019.
//

#ifndef RINCONADA_GLHELPER_H
#define RINCONADA_GLHELPER_H
#include <map>
#include <unordered_map>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "types.h"

class GLHelper {
public:

    void setVertices(GLfloat * pVertices, GLint numVertices, GLint vtxStride);
    void setIndices(GLushort * pIndices, GLint numIndices);
    void defAttrib(std::unordered_map<GLushort, GLushort>);
    void defAttrib2(std::list<GLAttrib>);
    void draw();
    void draw2();
    void draw3();
    ~GLHelper();
private:
    GLfloat* _vertices;
    GLint _numVertices;
    GLint _vtxStride;
    GLushort* _indices;
    GLint _numIndices;

    GLuint vboIds[2];
    std::unordered_map<GLushort, GLushort> _attrib;
    std::list<GLAttrib> _attrib2;

};


#endif //RINCONADA_GLHELPER_H
