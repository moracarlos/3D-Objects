
//  main.cpp
//  3D Objects
//
//  Created by Carlos Mora on 3/2/15.
//  Copyright (c) 2015 Carlos Mora. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "Main.h"
#include "UserInterface.h"
#include "Objects.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


GLFWwindow *gWindow;
int gWidth, gHeight;
bool gPress;
CUserInterface *userInterface;
std::vector <CObject*> objects;
int picked;
GLuint vbo;
float NCP;
float FCP;
float fAngle;

void pick (float x, float y)
{
    picked = -1;
}

int loadObject(std::string path){
    std::fstream filePtr(path, std::fstream::in | std::fstream::out);
    std::string fileType;
    if (!filePtr.is_open()) return 1;
    
    //Para el obj
    std::string line;
    std::vector <std::string> tokens;
    std::vector < std::vector <std::string> > tokMatrix;
    char* in;
    char* token;
    std::string faceComp;
    unsigned long numeral;
    
    
    int numVertices, numFaces, numEdges, n, i, j,ind, ind2, ind3;
    float x,y,z;
    float xmin, xmax, ymin, ymax, zmin, zmax;
    xmin = xmax = ymin = ymax = zmin = zmax = 0;
    CObject *obj;
    obj = new CObject();
    filePtr>>fileType;
    if (fileType=="OFF")
    {
        filePtr>>numVertices;
        filePtr>>numFaces;
        filePtr>>numEdges;
        for (i=0; i<numVertices; i++)
        {
            filePtr>>x;
            filePtr>>y;
            filePtr>>z;
            obj->addVertex(x, y, z);
            if (i==0){
                xmin = xmax = x;
                ymin = ymax= y;
                zmin = zmax = z;
            }else{
                if (x<xmin) xmin=x;
                if (x>=xmax) xmax=x;
                if (y<ymin) ymin=y;
                if (y>=ymax) ymax=y;
                if (z<zmin) zmin=z;
                if (z>=zmax) zmax=z;
            }
        }
        for (i=0; i<numFaces; i++)
        {
            filePtr>>n;
            if (n==3){
                for (j=0; j<n; j++){
                    filePtr>>ind;
                    obj->addIndex(ind);
                }
            }
            if (n>3){
                filePtr>>ind;
                filePtr>>ind2;
                filePtr>>ind3;
                obj->addIndex(ind);
                obj->addIndex(ind2);
                obj->addIndex(ind3);
                for (j=0; j<n-3; j++){
                    ind2=ind3;
                    filePtr>>ind3;
                    obj->addIndex(ind);
                    obj->addIndex(ind2);
                    obj->addIndex(ind3);
                }
            }
        }
        obj->setXs(xmin, xmax);
        obj->setYs(ymin, ymax);
        obj->setZs(zmin, zmax);
        obj->setCentro((xmin+xmax)/2, (xmin+ymax)/2, (zmin+zmax)/2);
        
        //Calcular normales por triangulo
        obj->calculateFaceNormals();
        obj->calculateVertexNormals();
        obj->initVBO();
        objects.push_back(obj);
        filePtr.close();
        return 0;
    }else{
        filePtr.seekg(0, filePtr.beg);
        x=y=z=NAN;
        while (!filePtr.eof()){
            getline(filePtr, line);
            numeral = line.find("#");
            if (numeral==std::string::npos) numeral=line.length();
            line = line.substr(0, numeral);
            in = new char [line.length()+1];
            std::strcpy (in, line.c_str());
            token = std::strtok(in, " ");
            while (token != NULL) {
                tokens.push_back(token);
                token = std::strtok(NULL, " ");
            }
            if (tokens.size()>0) tokMatrix.push_back(tokens);
            tokens.clear();
        }
        //TokMatrix contiene todo el archivo sin comentarios linea por palabra
        
        filePtr.close();
        for (int i=0; i<tokMatrix.size(); i++){
            if (tokMatrix[i][0]=="v"){
                if (tokMatrix[i].size()>=3){
                    if (x==NAN) xmin=xmax=std::stof(tokMatrix[i][1]);
                    if (y==NAN) ymin=ymax=std::stof(tokMatrix[i][2]);
                    if (z==NAN) zmin=zmax=std::stof(tokMatrix[i][3]);
                    x=std::stof(tokMatrix[i][1]);
                    y=std::stof(tokMatrix[i][2]);
                    z=std::stof(tokMatrix[i][3]);
                    if (x<xmin) xmin=x;
                    if (x>=xmax) xmax=x;
                    if (y<ymin) ymin=y;
                    if (y>=ymax) ymax=y;
                    if (z<zmin) zmin=z;
                    if (z>=zmax) zmax=z;
                    
                    obj->addVertex(x, y, z);
                }
            }
            if (tokMatrix[i][0]=="f"){
                if (tokMatrix[i].size()>=3){
                    x=y=z=0;
                    for (int j=3; j<tokMatrix[i].size(); j++){
                        faceComp = tokMatrix[i][1];
                        if (faceComp.find("/")==std::string::npos){
                            obj->addIndex(std::stoi(faceComp)-1);
                        }else{
                            obj->addIndex(std::stoi(faceComp)-1);
                        }
                        faceComp = tokMatrix[i][j-1];
                        if (faceComp.find("/")==std::string::npos){
                            obj->addIndex(std::stoi(faceComp)-1);
                        }else{
                            obj->addIndex(std::stoi(faceComp)-1);
                        }
                        
                        faceComp = tokMatrix[i][j];
                        if (faceComp.find("/")==std::string::npos){
                            obj->addIndex(std::stoi(faceComp)-1);

                        }else{
                            obj->addIndex(std::stoi(faceComp)-1);
                        }
                    }
                }
            }
        }
        obj->setXs(xmin, xmax);
        obj->setYs(ymin, ymax);
        obj->setZs(zmin, zmax);
        obj->setCentro((xmin+xmax)/2, (xmin+ymax)/2, (zmin+zmax)/2);
        obj->calculateFaceNormals();
        obj->calculateVertexNormals();
        obj->initVBO();
        objects.push_back(obj);
        return 0;
    }
    return 1;
}

void updateUserInterface()
{
    if (picked > -1)
    {
        float * color = userInterface->getObjectColor();
        objects[picked]->setColor(color[0], color[1], color[2]);
        objects[picked]->fillObject(userInterface->getmFill());
        objects[picked]->viewBox(userInterface->getmBbox());
        objects[picked]->setQuat(userInterface->getObjectQuat());
        color = userInterface->getbColor();
        objects[picked]->setbColor(color[0], color[1], color[2]);
        color = userInterface->getObjectColor();
        objects[picked]->setColor(color[0], color[1], color[2]);
        color = userInterface->getmfNormalColor();
        objects[picked]->setfNormalColor(color[0], color[1], color[2]);
        color = userInterface->getmvNormalColor();
        objects[picked]->setvNormalColor(color[0], color[1], color[2]);
        objects[picked]->setfNormal(userInterface->getfNormal());
        objects[picked]->setvNormal(userInterface->getvNormal());
        objects[picked]->setScaleX(userInterface->getScaleX());
        objects[picked]->setScaleY(userInterface->getScaleY());
        objects[picked]->setScaleZ(userInterface->getScaleZ());
    }
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.15f, 0.15f, 0.15f, 1.f);
    
    glLoadIdentity();
    glTranslatef(0.f,0.f,-10.0f);

    
    for (int i = 0; i < objects.size(); i++){
        objects[i]->display();
    }
}

void gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan( fovY / 360 * pi ) * zNear;	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}



void refresh(int pos, bool tipo)
{
    glm::vec3 eye(objects[picked]->getCentro()[0], objects[picked]->getCentro()[1], objects[picked]->getZs()[1]+1);
    glm::vec3 center(objects[picked]->getCentro()[0],objects[picked]->getCentro()[1],objects[picked]->getCentro()[2]);
    glm::vec3 up(0,1,0);
    float ratio = gWidth / float(gHeight);
    if(gHeight == 0) gHeight = 1;
    glViewport(0, 0, gWidth, gHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (tipo)
        gluPerspective(fAngle, ratio, NCP, FCP);
    else
        glOrtho(-10.0f,10.0f,-10.0f,10.0f,-0.01f,200.0f);
    glm::mat4 LA= glm::lookAt(eye, center, up);
    glMultMatrixf(glm::value_ptr(LA));
    glMatrixMode(GL_MODELVIEW);
}

void reshape(GLFWwindow *window, int width, int height)
{
    gWidth = width;
    gHeight = height;
    userInterface->reshape();
    float ratio = gWidth / float(gHeight);
    if(gHeight == 0) gHeight = 1;
    glViewport(0, 0, gWidth, gHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fAngle, ratio, NCP, FCP);
    //glOrtho(-10.0f,10.0f,-10.0f,10.0f,-10.0f,10.0f);
    glMatrixMode(GL_MODELVIEW);
    //userInterface->setProyeccion(true);
    if (picked>-1)
        refresh(picked, userInterface->getProyeccion());
}


void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (TwEventKeyGLFW(key, action))
        return;
    
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_UP:
                if (picked>-1){
                    objects[picked]->setD('y', 0.5);
                }
                break;
            case GLFW_KEY_DOWN:
                if (picked>-1){
                    objects[picked]->setD('y', -0.5);
                }
                break;
            case GLFW_KEY_LEFT:
                if (picked>-1){
                    objects[picked]->setD('x', -0.5);
                }
                break;
            case GLFW_KEY_RIGHT:
                if (picked>-1){
                    objects[picked]->setD('x', 0.5);
                }
                break;
            case GLFW_KEY_N:
                if (picked>-1){
                    objects[picked]->setD('z', 0.5);
                }
                break;
            case GLFW_KEY_F:
                if (picked>-1){
                    objects[picked]->setD('z', -0.5);
                }
                break;
        }
    }
}

void mouseButton(GLFWwindow* window, int button, int action, int mods) //Falta picked
{
    if (TwEventMouseButtonGLFW(button, action)){
    }
    
    double x, y;
    glfwGetCursorPos(gWindow, &x, &y);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !gPress)
    {
        float ax = x;
        float ay = gHeight - y;
        if (picked == -1)
        {
            gPress=true;
            pick(ax, ay);
            
        }
        
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        gPress = false;
    }
}

void cursorPos(GLFWwindow* window, double x, double y)
{
    if (TwEventMousePosGLFW(x, y))
        return;
}

void scroll (GLFWwindow* window, double x, double y)
{

}

void destroy()
{
    for (int i = 0; i < objects.size(); i++)
        delete objects[i];
    
    glfwDestroyWindow(gWindow);
    glfwTerminate();
}

void charInput(GLFWwindow* window, unsigned int scanChar)
{
    if (TwEventCharGLFW(scanChar, GLFW_PRESS))
        return;
}

bool initGlfw()
{
    if (!glfwInit())
        return false;
    
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    gWindow = glfwCreateWindow(gWidth, gHeight, "3D Objects", NULL, NULL);
    
    if (!gWindow)
    {
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(gWindow);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    
    const GLFWvidmode * vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(gWindow, (vidMode->width - gWidth) >> 1, (vidMode->height - gHeight) >> 1);
    
    glfwSetWindowSizeCallback(gWindow, reshape);
    glfwSetKeyCallback(gWindow, keyInput);
    glfwSetMouseButtonCallback(gWindow, mouseButton);
    glfwSetCursorPosCallback(gWindow, cursorPos);
    glfwSetCharCallback(gWindow, charInput);
    glfwSetScrollCallback(gWindow, scroll);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    return true;
}

void cambiarCull (bool c)
{
    if (c){
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
    }else{
        glDisable(GL_CULL_FACE);
    }
}
void cambiarzBuffer(bool c)
{
    if (c){
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
    }else{
        glDisable(GL_DEPTH_TEST);
    }
}

bool initUserInterface()
{
    if (!TwInit(TW_OPENGL, NULL))
        return false;
    userInterface = new CUserInterface();
    return true;
}




void cambiarProyeccion(bool p)
{
    if (picked>-1){
    refresh(picked, p);
    }else{
        float ratio = gWidth / float(gHeight);
        if(gHeight == 0) gHeight = 1;
        glViewport(0, 0, gWidth, gHeight);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (p)
            gluPerspective(fAngle, ratio, NCP, FCP);
        else
            glOrtho(-10.0f,10.0f,-10.0f,10.0f,0.01f,200.0f);
        glMatrixMode(GL_MODELVIEW);
    }
}

int main(void)
{
    gWidth = 1024;
    gHeight = 768;
    gPress = false;
    picked = -1;
    NCP = 0.01f;
    FCP = 200.0f;
    fAngle = 45.0f;
    
    if (!initGlfw() || !initUserInterface())
        return 1;
    
    reshape(gWindow, gWidth, gHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    while (!glfwWindowShouldClose(gWindow))
    {
        
        //draw2();
        
        display();
        
        TwDraw();
        
        updateUserInterface();
        
        glfwSwapBuffers(gWindow);
        
        glfwPollEvents();
    }
    
    destroy();
    
    return 0;
    
}
