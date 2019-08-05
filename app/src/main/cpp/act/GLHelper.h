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
#include "FileRead.h"

class GLHelper {
public:

    void setVertices(GLfloat * pVertices, GLint numVertices, GLint vtxStride);
    void setVertices(std::vector<glm::vec3> pVertices, GLint numVertices, GLint vtxStride);
    void setVertices(std::vector<GMv8> pVertices, GLint numVertices, GLint vtxStride);

    void setIndices(GLushort * pIndices, GLint numIndices);
    void setIndices(std::vector<GLushort> pIndices2, GLint numIndices);


    void defAttrib(std::unordered_map<GLushort, GLushort>);
    void defAttrib2(std::list<GLAttrib>);
    void draw();
    void draw2();
    void draw3();
    void draw4();
    ~GLHelper();
private:
    GLfloat* _vertices;
    std::vector<glm::vec3> _vertices2;

    std::vector<GMv8> _vertices3;

    GLint _numVertices;
    GLint _vtxStride;
    GLushort* _indices;
    std::vector<GLushort> _indices2;

    GLint _numIndices;

    GLuint vboIds[2];
    std::unordered_map<GLushort, GLushort> _attrib;
    std::list<GLAttrib> _attrib2;

};


#endif //RINCONADA_GLHELPER_H
