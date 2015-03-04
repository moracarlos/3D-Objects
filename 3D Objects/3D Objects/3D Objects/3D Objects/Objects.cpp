//  Objects.cpp
//  3D Objects
//
//  Created by Carlos Mora on 3/2/15.
//  Copyright (c) 2015 Carlos Mora. All rights reserved.
//

#include "Objects.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


CObject::CObject()
{
    bBox=true;
    fObject= false;
    fNormal=false;
    vNormal=false;
    nVertices=0;
    nIndex=0;
    mColor[0]=1.0f;
    mColor[1]=1.0f;
    mColor[2]=1.0f;
    
    quat[0] = 0;
    quat[1] = 0;
    quat[2] = 0;
    quat[3] = 1;
    
    bColor[0]=1.0f;
    bColor[1]=1.0f;
    bColor[2]=1.0f;
    
    nColor[0]=1.0f;
    nColor[1]=1.0f;
    nColor[2]=1.0f;
    
    vNormalColor[0]=1.0f;
    vNormalColor[1]=1.0f;
    vNormalColor[2]=1.0f;
    
    fNormalColor[0]=1.0f;
    fNormalColor[1]=1.0f;
    fNormalColor[2]=1.0f;
    
    
    dx=dy=dz=0;
    scale=1;
    scaleX=scaleY=scaleZ=1;
    estilo = TRIANGULOS;
}

CObject::~CObject()
{
        mVertices.clear();
}

void CObject::setColor(float r, float g, float b)
{
    mColor[0]=r;
    mColor[1]=g;
    mColor[2]=b;
}

float* CObject::getColor()
{
    return mColor;
}

void CObject::viewBox(bool option)
{
    bBox = option;
}

bool CObject::viewBox()
{
    return bBox;
}

void CObject::fillObject(bool option)
{
    fObject = option;
}

bool CObject::fillObject()
{
    return fObject;
}

float* CObject::getXs()
{
    return xs;
}

float* CObject::getYs()
{
    return ys;
}


float* CObject::getZs()
{
    return zs;
}

void CObject::setXs(float x0, float x1)
{
    xs[0] = x0;
    xs[1] = x1;
}

void CObject::setYs(float x0, float x1)
{
    ys[0] = x0;
    ys[1] = x1;
}

void CObject::setZs(float x0, float x1)
{
    zs[0] = x0;
    zs[1] = x1;
}

void CObject::addVertex(float x, float y, float z)
{
    nVertices++;
    mVertices.push_back(x);
    mVertices.push_back(y);
    mVertices.push_back(z);
}

void CObject::addIndex(GLuint i)
{
    nIndex++;
    index.push_back(i);
}

void CObject::setQuat(float* q)
{
    quat[0]=q[0];
    quat[1]=q[1];
    quat[2]=q[2];
    quat[3]=q[3];
}

float* CObject::getQuat()
{
    return quat;
}

float* CObject::getCentro()
{
    return centro;
}

void CObject::setCentro(float x, float y, float z)
{
    centro[0]=x;
    centro[1]=y;
    centro[2]=z;
}

float* CObject::getbColor(){
    return bColor;
}

void CObject::setbColor(float r, float g, float b)
{
    bColor[0]=r;
    bColor[1]=g;
    bColor[2]=b;
}

float* CObject::getnColor(){
    return bColor;
}

void CObject::setnColor(float r, float g, float b)
{
    nColor[0]=r;
    nColor[1]=g;
    nColor[2]=b;
}

void CObject::setEstilo(Estilos e)
{
    estilo=e;
}

void CObject::setD(char c, float num)
{
    if (c=='x') {
        dx+=num;
    }else if (c=='y'){
        dy+=num;
    }else if(c=='z'){
        dz+=num;
    }
}


void CObject::setScale(float f)
{
    scaleX+=f;
    scaleY+=f;
    scaleZ+=f;
}

void CObject::addFaceNormal(float x, float y, float z)
{
    faceNormals.push_back(x);
    faceNormals.push_back(y);
    faceNormals.push_back(z);
}

void CObject::calculateFaceNormals(){
    glm::vec3 v1, v2,baricentro, normal, p1, p2, p3;
    for (int i=0; i<index.size(); i+=3)
    {
        p1=glm::vec3(mVertices[index[i]*3], mVertices[index[i]*3+1], mVertices[index[i]*3+2]);
        p2=glm::vec3(mVertices[index[i+1]*3], mVertices[index[i+1]*3+1], mVertices[index[i+1]*3+2]);
        p3=glm::vec3(mVertices[index[i+2]*3], mVertices[index[i+2]*3+1], mVertices[index[i+2]*3+2]);
        v1=p2-p1;
        v2=p3-p1;
        v1=glm::normalize(v1);
        v2=glm::normalize(v2);
        normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);
        baricentro=(p1+p2+p3);
        baricentro=baricentro/3.0f;
        addFaceNormal(baricentro[0], baricentro[1], baricentro[2]);
        //addFaceNormal(normal[0]+baricentro[0], normal[1]+baricentro[1], normal[2]+baricentro[2]);
        addFaceNormal(baricentro[0]+normal[0]*0.1, baricentro[1]+normal[1]*0.1, baricentro[2]+normal[2]*0.1);
    }
}

void CObject::calculateVertexNormals(){
    int i;
    float x,y,z;
    for (i=0; i<mVertices.size(); i++){
        vertexNormals.push_back(0);
    }
    for (i=0; i<index.size(); i+=3){
        x=faceNormals[i/3*6+3];
        y=faceNormals[i/3*6+4];
        z=faceNormals[i/3*6+5];

        vertexNormals[index[i]*3]+=x;
        vertexNormals[index[i]*3+1]+=y;
        vertexNormals[index[i]*3+2]+=z;
        
        vertexNormals[index[i+1]*3]+=x;
        vertexNormals[index[i+1]*3+1]+=y;
        vertexNormals[index[i+1]*3+2]+=z;
        
        vertexNormals[index[i+2]*3]+=x;
        vertexNormals[index[i+2]*3+1]+=y;
        vertexNormals[index[i+2]*3+2]+=z;
    }
    glm::vec3 toNorm;
    for (i=0; i<vertexNormals.size(); i+=3){
        toNorm = glm::vec3(vertexNormals[i], vertexNormals[i+1], vertexNormals[i+2]);
        toNorm = glm::normalize(toNorm);
        vertexNormals[i]=toNorm[0];
        vertexNormals[i+1]=toNorm[1];
        vertexNormals[i+2]=toNorm[2];
    }
}

void CObject::setvNormal(bool op)
{
    vNormal=op;
}

bool CObject::getvNormal()
{
    return vNormal;
}

void CObject::setfNormal(bool op)
{
    fNormal=op;
}

bool CObject::getfNormal()
{
    return fNormal;
}

void CObject::setvNormalColor(float r, float g, float b)
{
    vNormalColor[0]=r;
    vNormalColor[1]=g;
    vNormalColor[2]=b;
}

void CObject::setfNormalColor(float r, float g, float b)
{
    fNormalColor[0]=r;
    fNormalColor[1]=g;
    fNormalColor[2]=b;
}

float* CObject::getfNormalColor()
{
    return fNormalColor;
}

float* CObject::getvNormalColor()
{
    return vNormalColor;
}

void CObject::setScaleX(float val)
{
    scaleX=val;
}

void CObject::setScaleY(float val)
{
    scaleY=val;
}

void CObject::setScaleZ(float val)
{
    scaleZ=val;
}

float CObject::getScaleX()
{
    return scaleX;
}

float CObject::getScaleY()
{
    return scaleY;
}

float CObject::getScaleZ()
{
    return scaleZ;
}

void CObject::initVBO()
{
    //Generate the buffers
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vindex);
    //Bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //Allocate space for the data
    glBufferData(GL_ARRAY_BUFFER, mVertices.size()*sizeof(float), NULL, GL_STATIC_DRAW);
    //Pass the vertex and the color data
    glBufferSubData(GL_ARRAY_BUFFER, 0, mVertices.size()*sizeof(float), mVertices.data());
    //glBufferSubData(GL_ARRAY_BUFFER, mVertices.size()*sizeof(float), mColor.size()*sizeof(float), mColor.data());
    //Bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vindex);
    //Fill the index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size()*sizeof(GLuint), index.data(), GL_STATIC_DRAW);
    
    //Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void CObject::display()
{
   /*
    //Generate the buffers
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vindex);
    //Bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //Allocate space for the data
    glBufferData(GL_ARRAY_BUFFER, mVertices.size()*sizeof(float) + mColor.size()*sizeof(float), NULL, GL_STATIC_DRAW);
    //Pass the vertex and the color data
    glBufferSubData(GL_ARRAY_BUFFER, 0, mVertices.size()*sizeof(float), mVertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, mVertices.size()*sizeof(float), mColor.size()*sizeof(float), mColor.data());
    //Bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vindex);
    //Fill the index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size()*sizeof(GLuint), index.data(), GL_STATIC_DRAW);
    
    //Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    */
    
    glPushMatrix();
    glTranslatef(centro[0]+dx, centro[1]+dy, centro[2]+dz);
    glTranslatef(centro[0], centro[1], centro[2]);
    glScalef(scaleX, scaleY, scaleZ);
    //Bind the buffer vbo1
    glm::quat q = glm::quat(quat[3], quat[0], quat[1], quat[2]);
    glm::normalize(q);
    glm::mat4 quatMatrix = glm::mat4_cast(q);
    glMultMatrixf(glm::value_ptr(quatMatrix));
    glTranslatef(-1*centro[0], -1*centro[1], -1*centro[2]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vindex);
    
    //Indicate that we are using color and vertex properties
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    //Indicate where is the color data and where is the vertex data
    //glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(mVertices.size()*sizeof(float)));
    glVertexPointer(3, GL_FLOAT, 0, 0);

    //Indicate to OpenGL that we are going to draw 36 elements (vertexs)
    glColor3f(mColor[0], mColor[1], mColor[2]);
    switch (estilo) {
        case TRIANGULOS:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case PUNTOS:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        case MALLADO:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        default:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
    glDrawElements(GL_TRIANGLES, nIndex, GL_UNSIGNED_INT, 0);
    
    //Disable the color and vertex properties
    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
                                            //glDisableClientState(GL_COLOR_ARRAY);
    
    //Unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    if (fNormal){
        glBegin(GL_LINES);
        glColor3f(fNormalColor[0], fNormalColor[1], fNormalColor[2]);
        for (int i = 0; i<faceNormals.size(); i+=6)
        {
            glVertex3f(faceNormals[i], faceNormals[i+1], faceNormals[i+2]);
            glVertex3f(faceNormals[i+3], faceNormals[i+4], faceNormals[i+5]);
        }
        glEnd();
    }
    
    if (vNormal){
        glBegin(GL_LINES);
        glColor3f(vNormalColor[0], vNormalColor[1], vNormalColor[2]);
        for (int i = 0; i<vertexNormals.size(); i+=3)
        {
            glVertex3f(mVertices[i], mVertices[i+1], mVertices[i+2]);
            glVertex3f(mVertices[i]+vertexNormals[i]*0.1, mVertices[i+1]+vertexNormals[i+1]*0.1, mVertices[i+2]+vertexNormals[i+2]*0.1);
        }
        glEnd();
    }
    
    if (bBox){
        glBegin(GL_LINES);
        glColor3f(bColor[0], bColor[1], bColor[2]);
        glVertex3f(xs[0], ys[0], zs[0]);
        glVertex3f(xs[0], ys[1], zs[0]);
        glVertex3f(xs[0], ys[0], zs[0]);
        glVertex3f(xs[1], ys[0], zs[0]);
        glVertex3f(xs[0], ys[0], zs[0]);
        glVertex3f(xs[0], ys[0], zs[1]);
        
        glVertex3f(xs[1], ys[1], zs[1]);
        glVertex3f(xs[0], ys[1], zs[1]);
        glVertex3f(xs[1], ys[1], zs[1]);
        glVertex3f(xs[1], ys[0], zs[1]);
        glVertex3f(xs[1], ys[1], zs[1]);
        glVertex3f(xs[1], ys[1], zs[0]);
    
        glVertex3f(xs[0], ys[1], zs[0]);
        glVertex3f(xs[0], ys[1], zs[1]);
        glVertex3f(xs[0], ys[1], zs[0]);
        glVertex3f(xs[1], ys[1], zs[0]);
    
        glVertex3f(xs[0], ys[1], zs[1]);
        glVertex3f(xs[0], ys[0], zs[1]);

        glVertex3f(xs[1], ys[0], zs[0]);
        glVertex3f(xs[1], ys[0], zs[1]);
    
        glVertex3f(xs[1], ys[0], zs[0]);
        glVertex3f(xs[1], ys[1], zs[0]);
    
        glVertex3f(xs[0], ys[0], zs[1]);
        glVertex3f(xs[1], ys[0], zs[1]);
    
    glEnd();
    }
    glPopMatrix();
    //glDeleteBuffers((GLsizei)(mVertices.size()*sizeof(float) + mColor.size()*sizeof(float)), &vbo);
    //glDeleteBuffers((GLsizei)(index.size()*sizeof(GLuint)), &vindex);
}

