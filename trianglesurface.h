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

    gsml::Vector3d RightPlaneVector;
    gsml::Vector3d LeftPlaneVector;
};

#endif // TRIANGLESURFACE_H
