#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H
#include <iostream>
#include "visualobject.h"
#include "vector3d.h"
#include "math.h"

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    ~TriangleSurface() override;
    void readFile(std::string filnavn);
    void writeFile(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void construct();
    void construct_cylinder();
    void construct_plane();
};



#endif // TRIANGLESURFACE_H
