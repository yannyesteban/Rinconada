//
// Created by yanny on 26/7/2019.
//

#include "GLHelper.h"
#include "Log.h"

void GLHelper::setVertices(GLfloat *pVertices, GLint numVertices, GLint vtxStride) {
    _vertices = pVertices;
    _numVertices = numVertices;
    _vtxStride = vtxStride;

}

void GLHelper::setIndices(GLushort *pIndices, GLint numIndices) {
    _indices = pIndices;
    _numIndices = numIndices;

}

void GLHelper::draw() {

    GLint numIndices=_numIndices;
    GLint numVertices = _numVertices;
    GLint vtxStride = (_vtxStride)*sizeof(GLfloat);

    //GLuint vboIds[2];
// vboIds[0] – used to store vertex attribute data
// vboIds[1] – used to store element indices
    glGenBuffers(2, vboIds);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, vtxStride * numVertices, _vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, _indices, GL_STATIC_DRAW);

    std::map<GLushort, GLushort>::iterator it;
    GLuint offset = 0;
    for (it = _attrib.begin(); it != _attrib.end(); ++it) {
        glEnableVertexAttribArray(it->first);
        glVertexAttribPointer(it->first, it->second, GL_FLOAT, GL_FALSE, vtxStride, (const void*)offset);
        offset += it->second * sizeof(GLfloat);

    }



    //glBindAttribLocation(programObject, VERTEX_POS_INDX, "vPosition");
    //glBindAttribLocation(programObject, VERTEX_NORMAL_INDX, "v_color");
    //glBindAttribLocation(program, VERTEX_TEXCOORD0_INDX, "v_texcoord");

    //glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
    for (it = _attrib.begin(); it != _attrib.end(); ++it) {

        glDisableVertexAttribArray(it->first);
    }


}

void GLHelper::defAttrib(std::map<GLushort, GLushort> pAttrib) {
    _attrib = pAttrib;
}

GLHelper::~GLHelper() {
    _LOGI("borrando");
    glDeleteBuffers(2, vboIds);
}


