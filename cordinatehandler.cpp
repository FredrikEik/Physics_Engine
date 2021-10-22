#include "cordinatehandler.h"

CordinateHandler::CordinateHandler()
{

}

QVector3D WhichTriangleToBarysentrificate(VisualObject* ballObject, VisualObject* triangleObject)
{

}

QVector3D BarysentricCordinates(VisualObject* ballObject, VisualObject* triangleObject)
{
    // Compute barycentric coordinates (u, v, w) for
    // point p with respect to triangle (a, b, c)
    QPoint p, a, b, c;
    float u, v, w;

    p = ballObject->Transform.position.x - 0.25;
    //a = triangleObject.kantA
    //b = triangleObject.kantB
    //c = triangleObject.kantC

    QVector v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = Dot(v0, v0);
    float d01 = Dot(v0, v1);
    float d11 = Dot(v1, v1);
    float d20 = Dot(v2, v0);
    float d21 = Dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;

    QVector3D BaricentricCordinates = {u, v, w};
    return BaricentricCordinates;
}
