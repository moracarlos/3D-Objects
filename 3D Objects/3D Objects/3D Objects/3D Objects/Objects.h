#include <stdio.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <stdlib.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum Estilos {TRIANGULOS, PUNTOS, MALLADO};

class CObject
{
    std::vector<float> mVertices;
    float mColor[3];
    std::vector<GLuint> index;
    std::vector<float> faceNormals;
    std::vector<float> vertexNormals;
    float bColor[3];
    float nColor[3];
    bool bBox;
    bool fObject;
    bool vNormal;
    bool fNormal;
    float xs[2];
    float ys[2];
    float zs[2];
    float quat[4];
    float centro[3];
    int nVertices;
    int nIndex;
    int nFaces;
    GLuint vbo;
    GLuint vindex;
    Estilos estilo;
    float dx, dy, dz;
    float scale;
    float scaleX;
    float scaleY;
    float scaleZ;
    float fNormalColor[3];
    float vNormalColor[3];
public:
    CObject();
    ~CObject();
    void initVBO();
    void setColor(float r, float g, float b);
    float* getColor();
    void viewBox(bool option);
    bool viewBox();
    void fillObject(bool option);
    bool fillObject();
    float* getXs();
    float* getYs();
    float* getZs();
    void setXs(float x0, float x1);
    void setYs(float y0, float y1);
    void setZs(float x0, float z1);
    void addVertex(float x, float y, float z);
    void setQuat(float *quat);
    float* getQuat ();
    void setCentro(float x, float y, float z);
    float* getCentro();
    void display();
    void addIndex(GLuint i);
    void setQuatMatrix(float* matrix);
    float* getbColor();
    float* getnColor();
    void setbColor(float r, float g, float b);
    void setnColor(float r, float g, float b);
    void setEstilo(Estilos e);
    void setD(char c, float num);
    void setScale(float f);
    void calculateFaceNormals();
    void addVertexNormal(float x, float y, float z);
    void addFaceNormal (float x, float y, float z);
    void setvNormal(bool op);
    bool getvNormal();
    void setfNormal(bool op);
    bool getfNormal();
    void setvNormalColor(float r, float g, float b);
    float* getvNormalColor();
    void setfNormalColor(float r, float g, float b);
    float* getfNormalColor();
    void calculateVertexNormals();
    
    void setScaleX(float val);
    void setScaleY(float val);
    void setScaleZ(float val);
    
    float getScaleX();
    float getScaleY();
    float getScaleZ();
};




