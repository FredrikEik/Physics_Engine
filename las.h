#ifndef LAS_H
#define LAS_H

#include <iostream>
#include <vector>
#include <QVector3D>
#include <fstream>
#include "Matrix4x4/vertex.h"
#include "visualobject.h"


class LAS : public VisualObject
{
public:
    LAS(std::string fileName);
    ~LAS() override;

    void readLAS(std::string fileName);
    void minMaxNormalize();
    void triangulate();

    void draw() override;
    void init(GLint matrixUniform) override;

private:
    float red = 10;
    float green = 30;
    float blue = 50;
    std::vector<gsml::Vertex> las_vertices;

    float xMin{-20.f}, yMin{0.f}, zMin{-20.f}, xMax{20.f}, yMax{6.f}, zMax{20.0f};
    float step{2.f};

    float highestX, highestY, highestZ, lowestX, lowestY, lowestZ;


};



#endif // LAS_H
