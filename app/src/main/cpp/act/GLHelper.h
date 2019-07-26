//
// Created by yanny on 26/7/2019.
//

#ifndef RINCONADA_GLHELPER_H
#define RINCONADA_GLHELPER_H
#include <map>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GLHelper {
public:

    void setVertices(GLfloat * pVertices, GLint numVertices, GLint vtxStride);
    void setIndices(GLushort * pIndices, GLint numIndices);
    void defAttrib(std::map<GLushort, GLushort>);
    void draw();
    ~GLHelper();
private:
    GLfloat* _vertices;
    GLint _numVertices;
    GLint _vtxStride;
    GLushort* _indices;
    GLint _numIndices;

    GLuint vboIds[2];
    std::map<GLushort, GLushort> _attrib;

};


#endif //RINCONADA_GLHELPER_H
