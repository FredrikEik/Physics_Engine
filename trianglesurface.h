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
    void read(std::string filnavn);
    void write(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;

    //konstruerer
    void construct();
    void construct_cylinder();
    void construct_plane();
    std::vector<gsml::Vertex>& get_vertices() { return mVertices; } // 191120

private:
        std::vector<int> mNeighbor;
};

#endif // TRIANGLESURFACE_H
