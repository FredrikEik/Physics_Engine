#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "matrix4x4.h"

// use this struct?
struct Triangle
{
    Triangle(gsml::Vertex v1, gsml::Vertex v2, gsml::Vertex v3) : V1{v1}, V2{v2}, V3{v3}
    {

    }

    gsml::Vertex V1, V2, V3;
};

struct Transform {
    gsml::Vector3d position;
    gsml::Vector3d scaling;
    gsml::Vector4d rotation;
};
//
class VisualObject : public QOpenGLFunctions_4_1_Core
{
public:
    VisualObject();
    ~VisualObject();
    gsml::Vector3d(baryCoord(const gsml::Vector2d &p1, const gsml::Vector2d &p2, const gsml::Vector2d &p3, const gsml::Vector2d &pos));

    virtual void init(GLint matrixUniform)=0;
    virtual void draw()=0;
    virtual void move(float dt) { }
    std::vector<gsml::Vertex> mVertices;
    std::vector<Triangle> mTriangles;
        gsml::Matrix4x4 mMatrix;
protected:
    GLuint mVAO{0};
    GLuint mVBO{0};
   //     GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)
    GLint mMatrixUniform{0};

    // Flyttet fra Disc 12/2/19
    gsml::Matrix4x4 mPosition;
    gsml::Matrix4x4 mRotation;
    gsml::Vector3d mVelocity;
    //
    gsml::Matrix4x4 mScale;

    float mTime{0};
};

#endif // VISUALOBJECT_H

