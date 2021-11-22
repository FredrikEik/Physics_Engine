#ifndef FILESURFACE_H
#define FILESURFACE_H

#include "visualobject.h"

class FileSurface : public VisualObject
{
public:
    FileSurface(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void readPoints(std::string filnavn);
    void makePlain();
    float calcHeight(float x, float y);
    std::vector<gsml::Vertex>& get_vertices() { return mVertices; } // 191120

private:
    void calculateNormals();
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

#endif // FILESURFACE_H
