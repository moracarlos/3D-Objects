//
//  UserInterface.cpp
//  3D Objects
//
//  Created by Carlos Mora on 3/2/15.
//  Copyright (c) 2015 Carlos Mora. All rights reserved.
//

#include "UserInterface.h"
#include "Objects.h"
#import <Cocoa/Cocoa.h>

using namespace std;

extern int gWidth, gHeight;
extern CUserInterface* userInterface;
extern std::vector <CObject *> objects;
extern int loadObject(std::string path);
extern int picked;
extern void cambiarProyeccion(bool p);
extern void cambiarCull (bool c);
extern void cambiarzBuffer(bool c);
extern void refresh(int pos, bool tipo);

void TW_CALL load(void* args)
{
    std::cout<<"LOAD"<<std::endl;
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    
    // Set array of file types
    NSArray *fileTypesArray;
    fileTypesArray = [NSArray arrayWithObjects:@"obj", @"off", nil];
    
    // Enable options in the dialog.
    [openDlg setCanChooseFiles:YES];
    [openDlg setAllowedFileTypes:fileTypesArray];
    [openDlg setAllowsMultipleSelection:TRUE];
    
    // Display the dialog box.  If the OK pressed,
    // process the files.
    while ([openDlg runModal]!=NSModalResponseOK){}
    
    
        // Gets list of all files selected
        NSArray *files = [openDlg URLs];
        
        // Loop through the files and process them.
        for(int i = 0; i < [files count]; i++ ) {
            // Do something with the filename.
            // paths.push_back(std::string([[[files objectAtIndex:i] path] UTF8String]));
            if (!loadObject(std::string([[[files objectAtIndex:i] path] UTF8String])))
              userInterface->addButton();
        }
    
    [openDlg close];
}

void TW_CALL quatSet(const void *value, void *clientData)
{
    userInterface->setObjectQuat((float*)value);
    if (picked>-1){
            objects[picked]->setQuat((float*)value);
    }
}

void TW_CALL quatGet(void *value, void *clientData)
{
    ((float *)value)[0] = userInterface->getObjectQuat()[0];
    ((float *)value)[1] = userInterface->getObjectQuat()[1];
    ((float *)value)[2] = userInterface->getObjectQuat()[2];
    ((float *)value)[3] = userInterface->getObjectQuat()[3];
}

void TW_CALL buttonCB (void *args)
{
    int x = *((int *)(&args));
    picked=x;
    if (x>-1){
        userInterface->setObjectQuat(objects[picked]->getQuat());
        userInterface->setbColor(objects[picked]->getbColor());
        userInterface->setmBbox(objects[picked]->viewBox());
        userInterface->setmvNormalColor(objects[picked]->getvNormalColor());
        userInterface->setmfNormalColor(objects[picked]->getfNormalColor());
        userInterface->setfNormal(objects[picked]->getfNormal());
        userInterface->setvNormal(objects[picked]->getvNormal());
        userInterface->setScaleX(objects[picked]->getScaleX());
        userInterface->setScaleY(objects[picked]->getScaleY());
        userInterface->setScaleZ(objects[picked]->getScaleZ());
        
        
        glm::vec3 eye(objects[picked]->getCentro()[0], objects[picked]->getCentro()[1], objects[picked]->getZs()[1]);
        glm::vec3 center(objects[picked]->getCentro()[0],objects[picked]->getCentro()[1],objects[picked]->getCentro()[2]);
        glm::vec3 up(0,1,0);
        
        refresh(picked, userInterface->getProyeccion());
    }
}

void TW_CALL boxColorSet(const void *value, void *clientData)
{
    userInterface->setbColor((float*)value);
}

void TW_CALL boxColorGet(void *value, void *clientData)
{
    ((GLfloat*) value)[0] = userInterface->getbColor()[0];
    ((GLfloat*) value)[1] = userInterface->getbColor()[1];
    ((GLfloat*) value)[2] = userInterface->getbColor()[2];
}

void TW_CALL bboxSet(const void *value, void *clientData)
{
    userInterface->setmBbox(*(const bool*)value);
    
}

void TW_CALL bboxGet(void *value, void *clientData)
{
    *(bool *)value = userInterface->getmBbox();  // for instance
    
}

void TW_CALL fColorSet(const void *value, void *clientData)
{
    userInterface->setObjectColor((float*)value);
}

void TW_CALL fColorGet(void *value, void *clientData)
{
    ((GLfloat*) value)[0] = userInterface->getObjectColor()[0];
    ((GLfloat*) value)[1] = userInterface->getObjectColor()[1];
    ((GLfloat*) value)[2] = userInterface->getObjectColor()[2];
}

void TW_CALL setEstilo(void *args)
{
    int x = *((int *)(&args));
    if (picked>-1){
        switch (x) {
            case 1:
                objects[picked]->setEstilo(TRIANGULOS);
                break;
            case 2:
                objects[picked]->setEstilo(PUNTOS);
                break;
            case 3:
                objects[picked]->setEstilo(MALLADO);
                break;
            default: break;
        }
    }
}

void TW_CALL proyeccionSet(const void *value, void *clientData)
{
    userInterface->setProyeccion(*(const bool*)value);
    cambiarProyeccion(*(const bool*)value);
}

void TW_CALL proyeccionGet(void *value, void *clientData)
{
    *(bool *)value = userInterface->getProyeccion();
}

void TW_CALL zSet(const void *value, void *clientData)
{
    userInterface->setzbuffer(*(const bool*)value);
    cambiarzBuffer(*(const bool*)value);
}

void TW_CALL zGet(void *value, void *clientData)
{
    *(bool *)value = userInterface->getzbuffer();
}

void TW_CALL cullSet(const void *value, void *clientData)
{
    userInterface->setCull(*(const bool*)value);
    cambiarCull(*(const bool*)value);
}

void TW_CALL cullGet(void *value, void *clientData)
{
    *(bool *)value = userInterface->getCull();
}

void TW_CALL fNormalSet(const void *value, void *clientData)
{
    userInterface->setfNormal(*(const bool*)value);
    
}

void TW_CALL fNormalGet(void *value, void *clientData)
{
    *(bool *)value = userInterface->getfNormal();  // for instance
    
}

void TW_CALL vNormalSet(const void *value, void *clientData)
{
    userInterface->setvNormal(*(const bool*)value);
    
}

void TW_CALL vNormalGet(void *value, void *clientData)
{
    *(bool *)value = userInterface->getvNormal();  // for instance
}

void TW_CALL fNormalColorSet(const void *value, void *clientData)
{
    userInterface->setmfNormalColor((float*)value);
}

void TW_CALL fNormalColorGet(void *value, void *clientData)
{
    ((GLfloat*) value)[0] = userInterface->getmfNormalColor()[0];
    ((GLfloat*) value)[1] = userInterface->getmfNormalColor()[1];
    ((GLfloat*) value)[2] = userInterface->getmfNormalColor()[2];
}

void TW_CALL vNormalColorSet(const void *value, void *clientData)
{
    userInterface->setmvNormalColor((float*)value);
}

void TW_CALL vNormalColorGet(void *value, void *clientData)
{
    ((GLfloat*) value)[0] = userInterface->getmvNormalColor()[0];
    ((GLfloat*) value)[1] = userInterface->getmvNormalColor()[1];
    ((GLfloat*) value)[2] = userInterface->getmvNormalColor()[2];
}


void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
    // Copy the content of souceString handled by the AntTweakBar library to destinationClientString handled by your application
    destinationClientString = sourceLibraryString;
}

void TW_CALL scaleXSet(const void *value, void *clientData)
{
    userInterface->setScaleX(*(const float*)value);

}
void TW_CALL scaleXGet(void *value, void *clientData)
{
    *(float *)value = userInterface->getScaleX();  // for instance

}

void TW_CALL scaleYSet(const void *value, void *clientData)
{
    userInterface->setScaleY(*(const float*)value);
    
}
void TW_CALL scaleYGet(void *value, void *clientData)
{
    *(float *)value = userInterface->getScaleY();  // for instance
    
}

void TW_CALL scaleZSet(const void *value, void *clientData)
{
    userInterface->setScaleZ(*(const float*)value);
    
}
void TW_CALL scaleZGet(void *value, void *clientData)
{
    *(float *)value = userInterface->getScaleZ();  // for instance
    
}

CUserInterface::CUserInterface()
{
    mObjectColor[0]=1.0f;
    mObjectColor[1]=1.0f;
    mObjectColor[2]=1.0f;
    
    mBoxColor[0]=1.0f;
    mBoxColor[1]=1.0f;
    mBoxColor[2]=1.0f;
    
    mfNormalColor[0]=1.0f;
    mfNormalColor[1]=1.0f;
    mfNormalColor[2]=1.0f;
    
    mvNormalColor[0]=1.0f;
    mvNormalColor[1]=1.0f;
    mvNormalColor[2]=1.0f;
    
    mBbox=false;
    mFill=false;
    proyeccion = true;
    cull=true;
    zbuffer=true;
    
    mvNormal=mfNormal=false;
    
    quat[0]=0;
    quat[1]=0;
    quat[2]=0;
    quat[3]=1;
    
    scaleX=scaleY=scaleZ=1;
    
    mUserInterface = TwNewBar("Object");
    TwDefine("Object refresh = '0.0001f'");
    TwDefine("Object resizable = false");
    TwDefine("Object fontresizable = false");
    TwDefine("Object movable = false");
    TwDefine("Object visible = true");
    TwDefine("Object position = '20 20'");
    TwDefine("Object size = '220 500'");
    TwAddButton(mUserInterface, "Load", load, NULL, "Label = 'Load'");
    
    TwAddVarCB(mUserInterface, "Proyeccion", TW_TYPE_BOOLCPP, proyeccionSet, proyeccionGet, &proyeccion, "label = 'Proyeccion' true=Perspectiva false=Ortogonal group='General'");
    
    TwAddVarCB(mUserInterface, "Z-Buffer", TW_TYPE_BOOLCPP, zSet, zGet, &zbuffer, "label = 'Z-Buffer' true=Activado false=Desactivado group='General'");
    
    TwAddVarCB(mUserInterface, "Culling", TW_TYPE_BOOLCPP, cullSet, cullGet, &cull, "label = 'Culling' true=Activado false=Desactivado group='General'");
    TwAddButton(mUserInterface, "Triangulos", setEstilo, (void*)1, "group='Estilo'");
    TwAddButton(mUserInterface, "Puntos", setEstilo, (void*)2, "group='Estilo'");
    TwAddButton(mUserInterface, "Mallado", setEstilo, (void*)3, "group='Estilo'");
    
    TwAddVarCB(mUserInterface, "quat", TW_TYPE_QUAT4F, quatSet, quatGet, &quat, "label = 'Rotacion'");

    TwAddVarCB(mUserInterface, "bbox", TW_TYPE_BOOLCPP, bboxSet, bboxGet, &mBbox, "label = 'Bounding Box' true=Enabled false=Disabled");
    TwAddVarCB(mUserInterface, "Show Face Normals", TW_TYPE_BOOLCPP, fNormalSet, fNormalGet, &mfNormal, "true=Enabled false=Disabled");
    TwAddVarCB(mUserInterface, "Show Vertex Normals", TW_TYPE_BOOLCPP, vNormalSet, vNormalGet, &mvNormal, "true=Enabled false=Disabled");

    TwAddVarCB(mUserInterface, "color", TW_TYPE_COLOR3F, fColorSet, fColorGet, &mObjectColor[0], "label = Relleno group='Color'");
    TwAddVarCB(mUserInterface, "Bounding Box Color", TW_TYPE_COLOR3F, boxColorSet, boxColorGet, &mBoxColor[0], "group='Color'");
    TwAddVarCB(mUserInterface, "Face Normal Color", TW_TYPE_COLOR3F, fNormalColorSet, fNormalColorGet, &mfNormalColor[0], "group='Color'");
    TwAddVarCB(mUserInterface, "Vertex Normal Color", TW_TYPE_COLOR3F, vNormalColorSet, vNormalColorGet, &mvNormalColor[0], "group='Color'");
    
    TwAddVarCB(mUserInterface, "Scale X", TW_TYPE_FLOAT, scaleXSet, scaleXGet, &scaleX, "group='Escalamientos' step=0.05");
    TwAddVarCB(mUserInterface, "Scale Y", TW_TYPE_FLOAT, scaleYSet, scaleYGet, &scaleY, "group='Escalamientos' step=0.05");
    TwAddVarCB(mUserInterface, "Scale Z", TW_TYPE_FLOAT, scaleZSet, scaleZGet, &scaleZ, "group='Escalamientos' step=0.05");
}

CUserInterface::~CUserInterface()
{
}

void CUserInterface::addButton()
{
    std::string str;
    str += "Object ";
    str += std::to_string(objects.size()-1);
    const char * c = str.c_str();
    TwAddButton(mUserInterface, c, buttonCB, (void*)(objects.size()-1), NULL);
}

void CUserInterface::reshape()
{
    TwWindowSize(gWidth, gHeight);
}

void CUserInterface::show()
{
    TwDefine("Figure visible = true");
}

void CUserInterface::setProyeccion(bool p)
{
    proyeccion = p;
}

bool CUserInterface::getProyeccion()
{
    return proyeccion;
}

void CUserInterface::hide()
{
    TwDefine("Figure visible = false");
}

void CUserInterface::setObjectColor(float *color)
{
    mObjectColor[0] = color[0];
    mObjectColor[1] = color[1];
    mObjectColor[2] = color[2];
}

float* CUserInterface::getObjectColor()
{
    return mObjectColor;
}

void CUserInterface::setObjectQuat(float *q)
{
    quat[0] = q[0];
    quat[1] = q[1];
    quat[2] = q[2];
    quat[3] = q[3];
}

float* CUserInterface::getObjectQuat()
{
    return quat;
}

bool CUserInterface::getmFill()
{
    return mFill;
}

bool CUserInterface::getmBbox()
{
    return mBbox;
}

void CUserInterface::setmFill(bool option)
{
    mFill = option;
}

void CUserInterface::setmBbox(bool option)
{
    mBbox = option;
}

void CUserInterface::setbColor(float *color)
{
    mBoxColor[0]=color[0];
    mBoxColor[1]=color[1];
    mBoxColor[2]=color[2];
}

float* CUserInterface::getbColor()
{
    return mBoxColor;
}

bool CUserInterface::getmNormal()
{
    return mNormal;
}

void CUserInterface::setmNormal(bool op)
{
    mNormal=op;
}

void CUserInterface::setzbuffer(bool z)
{
    zbuffer =z;
}

void CUserInterface::setCull(bool c)
{
    cull= c;
}

bool CUserInterface::getzbuffer()
{
    return zbuffer;
}

bool CUserInterface::getCull()
{
    return cull;
}
               
void CUserInterface::setmfNormalColor(float* b)
{
    mfNormalColor[0]=b[0];
    mfNormalColor[1]=b[1];
    mfNormalColor[2]=b[2];
}

float* CUserInterface::getmfNormalColor()
{
    return mfNormalColor;
}

void CUserInterface::setmvNormalColor(float* b)
{
    mvNormalColor[0]=b[0];
    mvNormalColor[1]=b[1];
    mvNormalColor[2]=b[2];
}
float* CUserInterface::getmvNormalColor()
{
    return mvNormalColor;
}

void CUserInterface::setvNormal(bool op)
{
    mvNormal=op;
}
               
bool CUserInterface::getvNormal()
{
    return mvNormal;
}
               
void CUserInterface::setfNormal(bool op)
{
    mfNormal=op;
}
               
bool CUserInterface::getfNormal()
{
    return mfNormal;
}

void CUserInterface::setScaleX(float val)
{
    scaleX=val;
}

float CUserInterface::getScaleX()
{
    return scaleX;
}

void CUserInterface::setScaleY(float val)
{
    scaleY=val;
}

void CUserInterface::setScaleZ(float val)
{
    scaleZ=val;
}

float CUserInterface::getScaleY()
{
    return scaleY;
}

float CUserInterface::getScaleZ()
{
    return scaleZ;
}