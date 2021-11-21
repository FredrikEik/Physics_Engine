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
    static const int X = 200; // (Max.x - Min.x) / n
    static const int Y = 294; // (Max.y - Min.y) / n
    std::vector<gsml::Vector3d> map[X][Y];
    //gsml::Vector3d m[X][Y];
};



#endif // TRIANGLESURFACE_H
