#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H
#include <iostream>
#include "visualobject.h"
#include "vector3d.h"

struct Map
{
    const int X;
    const int Y;
    Map(int dX, int dY) :X(dX), Y(dY)
    {
        std::vector<gsml::Vector3d> map[X][Y];
    };
    std::vector<gsml::Vector3d> map[X][Y];;
};

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    TriangleSurface(std::string filnavn);
    ~TriangleSurface() override;
    void readPoints(std::string filnavn);
    void readFile(std::string filnavn);
    void writeFile(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void construct();
    void construct_cylinder();
    void construct_plane();
    void makePlain();
    float calcHeight(float x, float y);
    std::vector<gsml::Vertex>& get_vertices() { return mVertices; } // 191120
private:
    float xMin = 9000000;
    float xMax = 0;
    float yMin = 9000000;
    float yMax = 0;
    gsml::Vector2d MapMin;
    gsml::Vector2d MapMax;
    int n = 5;
    Map* myMap;
};



#endif // TRIANGLESURFACE_H
