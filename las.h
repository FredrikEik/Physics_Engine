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

    std::vector<gsml::Vertex> getPositions() const;
    std::vector<gsml::Vertex> getmVertices() const;
    void setPositions(const std::vector<gsml::Vertex> &value);

    void init(GLint matrixUniform) override;
    void draw() override;

private:
    void ReadDatafromFile(std::string fileName);
    void populatePosArr();

    float PosArr[1000][2000];
    void RemoveDeltaPos();
    void GenerateHeightMap();
    float CalcHeight(float x = 0, float z = 0);
};

#endif // LAS_H
