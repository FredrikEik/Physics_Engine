#ifndef STATICMATH_H
#define STATICMATH_H
#include "qvector3d.h"
namespace gsml
{

class StaticMath
{
public:
    StaticMath();

    static constexpr QVector3D GRAVITY = QVector3D(0.f, -9.81f, 0);
    static QVector3D getAcceleration (const QVector3D& surfaceNormal, float mass = 1.f);

};

}

#endif // STATICMATH_H
