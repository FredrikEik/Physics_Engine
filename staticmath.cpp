#include "staticmath.h"

gsml::StaticMath::StaticMath()
{

}

QVector3D gsml::StaticMath::getAcceleration(const QVector3D &surfaceNormal, float mass)
{
    return (1.f/mass)*((mass * GRAVITY * surfaceNormal * surfaceNormal.z()) + mass * GRAVITY);
}

