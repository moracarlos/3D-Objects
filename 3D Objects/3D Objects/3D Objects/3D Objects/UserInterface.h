#pragma once

#include <AntTweakBar.h>
#include <string>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CUserInterface
{
    TwBar *mUserInterface;
    float mObjectColor[3];
    float mBoxColor[3];
    float mfNormalColor[3];
    float mvNormalColor[3];
    bool mBbox;
    bool mFill;
    bool mNormal;
    float quat[4];
    bool proyeccion;
    bool zbuffer, cull;
    bool mvNormal;
    bool mfNormal;
    std::string path;
    float scaleX;
    float scaleY;
    float scaleZ;
    
public:
    CUserInterface();
    ~CUserInterface();
    void reshape();
    void show();
    void hide();
    void setObjectColor(float *color);
    float *getObjectColor();
    bool getmFill();
    void setmFill(bool option);
    bool getmBbox();
    void setmBbox(bool option);
    bool getmNormal();
    void setmNormal(bool op);
    void setObjectQuat(float* value);
    float* getObjectQuat();
    void hideGroup();
    void addButton();
    void setbColor(float* color);
    float* getbColor();
    void setProyeccion(bool p);
    bool getProyeccion();
    void setzbuffer(bool z);
    void setCull(bool c);
    bool getzbuffer();
    bool getCull();
    void setmfNormalColor(float* b);
    void setmvNormalColor(float* b);
    float* getmfNormalColor();
    float* getmvNormalColor();
    void setvNormal(bool op);
    bool getvNormal();
    void setfNormal(bool op);
    bool getfNormal();
    void setScaleX(float val);
    float getScaleX();
    void setScaleY(float val);
    float getScaleY();
    void setScaleZ(float val);
    float getScaleZ();
};
