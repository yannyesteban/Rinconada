//
// Created by yanny on 26/7/2019.
//

#include <list>
#include "GLHelper.h"
#include "Log.h"

void GLHelper::setVertices(GLfloat *pVertices, GLint numVertices, GLint vtxStride) {
    _vertices = pVertices;
    _numVertices = numVertices;
    _vtxStride = vtxStride;

}
void GLHelper::setVertices(std::vector<glm::vec3> pVertices, GLint numVertices, GLint vtxStride) {
    _vertices2 = pVertices;
    _numVertices = numVertices;
    _vtxStride = vtxStride;

}
void GLHelper::setVertices(std::vector<GMv8> pVertices, GLint numVertices, GLint vtxStride) {
    _vertices3 = pVertices;
    _numVertices = numVertices;
    _vtxStride = vtxStride;

}
void GLHelper::setIndices(GLushort *pIndices, GLint numIndices) {
    _indices = pIndices;
    _numIndices = numIndices;

}
void GLHelper::setIndices(std::vector<GLushort> pIndices2, GLint numIndices) {
    _indices2 = pIndices2;
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

    std::unordered_map<GLushort, GLushort>::iterator it;
    GLuint offset = 0;
    _LOGE("compiling __, __, compiling: __");
    for (it = _attrib.begin(); it != _attrib.end(); ++it) {

        _LOGE("compiling %d, %d, compiling: %d", it->first, it->second, offset);

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


void GLHelper::draw2() {

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

    std::list<GLAttrib>::iterator it;
    GLuint offset = 0;
    _LOGE("compiling __, __, compiling: __");
    for (it = _attrib2.begin(); it != _attrib2.end(); ++it) {

        _LOGE("compiling %d, %d, compiling: %d", it->index, it->size, offset);

        glEnableVertexAttribArray(it->index);
        glVertexAttribPointer(it->index, it->size, GL_FLOAT, GL_FALSE, vtxStride, (const void*)offset);


        offset += it->size * sizeof(GLfloat);


    }



    //glBindAttribLocation(programObject, VERTEX_POS_INDX, "vPosition");
    //glBindAttribLocation(programObject, VERTEX_NORMAL_INDX, "v_color");
    //glBindAttribLocation(program, VERTEX_TEXCOORD0_INDX, "v_texcoord");

    //glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
    for (it = _attrib2.begin(); it != _attrib2.end(); ++it) {

        glDisableVertexAttribArray(it->index);
    }


}


void GLHelper::draw3() {

    GLint numIndices=_numIndices;
    GLint numVertices = _numVertices;
    GLint vtxStride = (_vtxStride)*sizeof(GLfloat);

    //GLuint vboIds[2];
// vboIds[0] – used to store vertex attribute data
// vboIds[1] – used to store element indices
    glGenBuffers(2, vboIds);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    numVertices = 3;
    GLfloat vv[] = {
            0.0,0.0,0.0,

            0.5,0.0,0.0,

            0.5,0.5,0.0,

    };

    std::vector<glm::vec3> q;

    q.push_back(glm::vec3(0.0, 0.0,0.0));
    q.push_back(glm::vec3(0.5, 0.0,0.0));
    q.push_back(glm::vec3(0.5, 0.9,0.0));


    vtxStride = 3 *sizeof(GLfloat);

    glBufferData(GL_ARRAY_BUFFER, vtxStride * numVertices, &q[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, _indices, GL_STATIC_DRAW);

    std::list<GLAttrib>::iterator it;
    GLuint offset = 0;
    _LOGE("compiling __, __, compiling: __");
    for (it = _attrib2.begin(); it != _attrib2.end(); ++it) {

        _LOGE("compiling %d, %d, compiling: %d", it->index, it->size, offset);

        glEnableVertexAttribArray(it->index);
        glVertexAttribPointer(it->index, it->size, GL_FLOAT, GL_FALSE, vtxStride, (const void*)offset);


        offset += it->size * sizeof(GLfloat);


    }



    //glBindAttribLocation(programObject, VERTEX_POS_INDX, "vPosition");
    //glBindAttribLocation(programObject, VERTEX_NORMAL_INDX, "v_color");
    //glBindAttribLocation(program, VERTEX_TEXCOORD0_INDX, "v_texcoord");

    //glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
    for (it = _attrib2.begin(); it != _attrib2.end(); ++it) {

        glDisableVertexAttribArray(it->index);
    }


}

void GLHelper::draw4() {

    GLint numIndices = _numIndices;
    GLint numVertices = _numVertices;
    GLint vtxStride = (_vtxStride)*sizeof(GLfloat);
/*
     numIndices = 6;
     numVertices = 4;
     vtxStride = (6)*sizeof(GLfloat);
    */
    GLfloat v[] ={
            0.0, 0.0, 0.0,1.0,0.0,0.0,
            0.5, 0.0, 0.0,1.0,0.0,0.0,
            0.5, 0.5, 0.0,1.0,0.0,1.0,
            0.0, 0.5, 0.0,1.0,1.0,0.0,
    };
    GLushort i[] ={
            0,1,2,0,2,3
    };

    std::vector<GMv6> q;
    q.push_back({glm::vec3(0.0,0.0,0.0), glm::vec3(1.0,0.0,1.0)});

    q.push_back({glm::vec3(1.5,0.0,0.0), glm::vec3(0.0,1.0,1.0)});
    q.push_back({glm::vec3(1.5,0.5,0.0), glm::vec3(0.0,0.0,1.0)});
    q.push_back({glm::vec3(0.0,0.5,0.0), glm::vec3(1.0,0.5,1.0)});




    //GLuint vboIds[2];
    // vboIds[0] – used to store vertex attribute data
    // vboIds[1] – used to store element indices
    glGenBuffers(2, vboIds);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    //glBufferData(GL_ARRAY_BUFFER, vtxStride * numVertices, &q[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vtxStride * numVertices, &_vertices3[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, i, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, &_indices2[0], GL_STATIC_DRAW);

    std::list<GLAttrib>::iterator it;
    GLuint offset = 0;

    for (it = _attrib2.begin(); it != _attrib2.end(); ++it) {

        _LOGE("compiling %d, %d, compiling: %d", it->index, it->size, offset);

        glEnableVertexAttribArray(it->index);
        glVertexAttribPointer(it->index, it->size, GL_FLOAT, GL_FALSE, vtxStride, (const void*)offset);

        offset += it->size * sizeof(GLfloat);
    }

    //glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

    _LOGE("numIndices.. %d", numIndices);
    for (it = _attrib2.begin(); it != _attrib2.end(); ++it) {

        glDisableVertexAttribArray(it->index);
    }
    glDeleteBuffers(2,vboIds);

}


void GLHelper::defAttrib(std::unordered_map<GLushort, GLushort> pAttrib) {
    _attrib = pAttrib;
}
void GLHelper::defAttrib2(std::list<GLAttrib> pAttrib) {
    _attrib2 = pAttrib;
}
GLHelper::~GLHelper() {
    _LOGI("borrando");
    glDeleteBuffers(2, vboIds);
}


