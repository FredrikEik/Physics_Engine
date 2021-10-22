#ifndef CORDINATEHANDLER_H
#define CORDINATEHANDLER_H

#include "vector3d.h"
#include "visualobject.h"

class CordinateHandler
{
public:
    CordinateHandler();
    QVector3D WhichTriangleToBarysentrificate();
    QVector3D BarysentricCordinates(VisualObject* ballObject, VisualObject* triangleObject);

};

#endif // CORDINATEHANDLER_H
