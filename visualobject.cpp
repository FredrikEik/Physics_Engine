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

gsml::Vector3d VisualObject::VisualObject::barycentricCoords(const gsml::Vector2d &p1, const gsml::Vector2d &p2, const gsml::Vector2d &p3, const gsml::Vector2d &position)
{
    gsml::Vector2d p12 = p2-p1;
    gsml::Vector2d p13 = p3-p1;

    qDebug() << "P12" << p12.x << p12.y << "P13" << p13.x << p13.y;

    gsml::Vector3d n = p12.operator^(p13);

    qDebug() << "n" << n.x << n.y << n.z;
    float areal_123 = n.length();

    qDebug() << "areal_123" << areal_123;
    gsml::Vector3d baryc;
    gsml::Vector2d p = p2 - position;
    gsml::Vector2d q = p3 - position;
    n = p.operator^(q);// {0,0,(q.xp.y)-(q.yp.x)};
    baryc.x = (n.z / areal_123);

    p = p3 - position;
    q = p1 - position;
    n = p.operator^(q);//n = {0,0,(q.xp.y)-(q.yp.x)};
    baryc.y = (n.z / areal_123);

    p = p1 - position;
    q = p2 - position;
    n = p.operator^(q);//n = {0,0,(q.xp.y)-(q.yp.x)};
    baryc.z = (n.z / areal_123);

    return baryc;
}

// Funksjon fra matte 3. Barysentrisk koordinater ift den trakanten jeg har sendt inn.
//QVector3D VisualObject::barycentricCoordinates(const QVector2D &dette, const QVector2D &p1, const QVector2D &p2, const QVector2D &p3)
//{
//    QVector2D p12a = p2-p1;
//    QVector2D p13a = p3-p1;
//    QVector3D p12{p12a.x(),p12a.y(),0};
//    QVector3D p13{p13a.x(),p13a.y(),0};
//    QVector3D n = QVector3D::crossProduct(p12, p13);
//    float areal_123 = n.length(); // dobbelt areal
//    QVector3D baryc; // til retur. Husk

//    // u
//    QVector2D p = p2 - dette;
//    QVector2D q = p3 - dette;
//    QVector3D pa{p.x(), p.y(), 0};
//    QVector3D qa{q.x(), q.y(), 0};
//    n = QVector3D::crossProduct(pa,qa);
//    baryc.setX(n.z()/areal_123);

//    // v
//    p = p3 - dette;
//    q = p1 - dette;
//    pa = {p.x(), p.y(), 0};
//    qa = {q.x(), q.y(), 0};
//    n = QVector3D::crossProduct(pa,qa);
//    baryc.setY(n.z()/areal_123);

//    // w
//    p = p1 - dette;
//    q = p2 - dette;
//    pa = {p.x(), p.y(), 0};
//    qa = {q.x(), q.y(), 0};
//    n = QVector3D::crossProduct(pa,qa);
//    baryc.setZ(n.z()/areal_123);
//    return baryc;
//}



