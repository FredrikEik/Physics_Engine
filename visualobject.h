#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "math.h"
#include "vertex.h"
#include "matrix4x4.h"

// use this struct?
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
    virtual void init(GLint matrixUniform)=0;
    virtual void draw()=0;
    virtual void move(float dt) { }
    virtual void move(float dx, float dy, float dz) { }
    //QVector3D barycentricCoordinates(const QVector2D &dette, const QVector2D &p1, const QVector2D &p2, const QVector2D &p3);
    gsml::Vector3d barycentricCoords(const gsml::Vector2d &p1, const gsml::Vector2d &p2, const gsml::Vector2d &p3, const gsml::Vector2d &position);

protected:
    std::vector<gsml::Vertex> mVertices;
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLint mMatrixUniform{0};
    gsml::Matrix4x4 mMatrix;
    // Flyttet fra Disc 12/2/19
    gsml::Matrix4x4 mPosition;
    gsml::Matrix4x4 mRotation;
    gsml::Vector3d mVelocity;
    gsml::Vector3d mVecPosition;
    //
    gsml::Matrix4x4 mScale;
};

#endif // VISUALOBJECT_H

