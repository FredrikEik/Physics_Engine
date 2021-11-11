#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"
#include "vector3d.h"

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    TriangleSurface(std::string filnavn);
    ~TriangleSurface() override;
    void readFile(std::string filnavn);
    void writeFile(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void construct();
    void construct_cylinder();
    void construct_plane();
    std::vector<gsml::Vertex>& get_vertices() { return mVertices; } // 191120
    float xmin=0, xmax=1.0f, ymin=0.0f, ymax=1.0f, h=0.25f;
    double xMinimum = 0, xMaximum = 1, yMinimum = 0, yMaximum=1, zMinimum = 0, zMaximum=1;
};

#endif // TRIANGLESURFACE_H
