// VisualObject.cpp
#include "visualobject.h"

VisualObject::VisualObject()
{
    mMatrix.setToIdentity();
    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
}

VisualObject::~VisualObject()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}



gsml::Vector3d VisualObject::baryCoord(const gsml::Vector2d &p1, const gsml::Vector2d &p2, const gsml::Vector2d &p3, const gsml::Vector2d &pos)
{
    gsml::Vector2d p12 = p2 - p1;
    gsml::Vector2d p13 = p3 - p1;
    gsml::Vector3d n = {0.f, 0.f, (p13.x * p12.y) - (p13.y * p12.x)};
    float A = n.length();

    //u
    gsml::Vector2d p = p2 - pos;
    gsml::Vector2d q = p3 - pos;
    gsml::Vector3d n1 = {0.f, 0.f, (q.x * p.y) - (q.y * p.x)};

    //v
    p = p3 - pos;
    q = p1 - pos;
    gsml::Vector3d n2 = {0.f, 0.f, (q.x * p.y) - (q.y * p.x)};

    //w
    p = p1 - pos;
    q = p2 - pos;
    gsml::Vector3d n3 = {0.f, 0.f, (q.x * p.y) - (q.y * p.x)};

    return gsml::Vector3d{n1.z/A, n2.z/A, n3.z/A};
}
