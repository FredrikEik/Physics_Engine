#ifndef SURF_H
#define SURF_H

#include <iostream>
#include <vector>
#include <QVector3D>
#include <fstream>
#include "Matrix4x4/vertex.h"
#include "visualobject.h"

class surf : public VisualObject
{
public:
    surf(std::string filnavn);
    float calcHeight(float x, float y);
    std::vector<gsml::Vertex>& get_vertices() { return mVertices; }

    void init(GLint matrixUniform) override;
    void draw() override;
    void read(std::string filnavn);
    void make();


private:
    void calculateNormals();

    gsml::Vector2d Min;
    gsml::Vector2d Max;
    int n = 5;
    static const int X = 200;
    static const int Y = 294;

    std::vector<gsml::Vector3d> map[X][Y];
    float mNormal[3];

    float xMin = 9000000;
    float xMax = 0;
    float yMin = 9000000;
    float yMax = 0;
};

#endif // SURF_H
