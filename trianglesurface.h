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
    void readLasFile(std::string filnavn);
    void writeFile(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void construct();
    void construct_cylinder();
    void construct_plane();
    void minMaxScale();
    std::vector<gsml::Vertex>& get_vertices() { return mVertices; } // 191120
    float xmin=-20.f, xmax=20.f, ymin=-20.f, ymax=20.f, zmin = -5.f, zmax =5.f, h=5.f;
    double xMinimum = 0, xMaximum = 1, yMinimum = 0, yMaximum=1, zMinimum = 0, zMaximum=1;
    int numberofPointsInside;
    gsml::Vertex vertex;
    gsml::Vertex lasVertex;
    std::vector<gsml::Vertex> tempVertices;
    std::vector<gsml::Vertex> lasVertices;
};

#endif // TRIANGLESURFACE_H
