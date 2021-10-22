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

gsml::Vector3d VisualObject::barycentricCoords(const gsml::Vector2d &p1, const gsml::Vector2d &p2, const gsml::Vector2d &p3, const gsml::Vector2d &position)
{
    gsml::Vector2d p12 = p2-p1;
    gsml::Vector2d p13 = p3-p1;

    gsml::Vector3d n = {0,0,(p13.x*p12.y)-(p13.y*p12.x)};

    float areal_123 = n.length();

    gsml::Vector3d baryc;
    gsml::Vector2d p = p2 - position;
    gsml::Vector2d q = p3 - position;
    n = {0,0,(q.x*p.y)-(q.y*p.x)};
    baryc.x = (n.z / areal_123);

    p = p3 - position;
    q = p1 - position;
    n = n = {0,0,(q.x*p.y)-(q.y*p.x)};
    baryc.y = (n.z / areal_123);

    p = p1 - position;
    q = p2 - position;
    n = n = {0,0,(q.x*p.y)-(q.y*p.x)};
    baryc.z = (n.z / areal_123);

    return baryc;
}

float VisualObject::barycentricHeight(const gsml::Vector3d &point, const gsml::Vector3d &corner1, const gsml::Vector3d &corner2, const gsml::Vector3d &corner3)
{
    // This calculation assumes that height is in the Y axis.
    // We also assume that the point is projected down the Y axis onto the triangle
    // Because of this we flatten the problem down to 2D
    gsml::Vector2d p{point.x, point.y};
    gsml::Vector2d a{corner1.x, corner1.y};
    gsml::Vector2d b{corner2.x, corner2.y};
    gsml::Vector2d c{corner3.x, corner3.y};

    gsml::Vector2d v0 = b - a;
    gsml::Vector2d v1 = c - a;
    gsml::Vector2d v2 = p - a;


    float d00 = (v0.x * v0.x +  v0.y * v0.y);
    float d01 = (v0.x * v1.x +  v0.y * v1.y);
    float d11 = (v1.x * v1.x +  v1.y * v1.y);
    float d20 = (v2.x * v0.x +  v2.y * v0.y);
    float d21 = (v2.x * v1.x +  v2.y * v1.y);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);

    // u, v, w are the barycentric coordinates
    float v = (d11 * d20 - d01 * d21) * invDenom;
    float w = (d00 * d21 - d01 * d20) * invDenom;
    float u = 1.0f - v - w;

    //        qDebug() << "BaryCords:" << u << v << w;

    float heightOut;
    if(u >= 0.f && v >= 0.f && w >= 0.f )
        heightOut = corner1.z * u + corner2.z * v + corner3.z * w;
    else
        heightOut = -10000.f;

    return heightOut;
}

std::vector<gsml::Vertex> VisualObject::get_vertices()
{
    return mVertices;
}
