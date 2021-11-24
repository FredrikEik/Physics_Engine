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

    void draw() override;
    void init(GLint matrixUniform) override;
    //void move(float x, float y, float z) override;
//    std::vector<gsml::Vertex>& get_vertices() { return mVertices; } // 191120

private:

    std::vector<gsml::Vertex> lasData;

    //mins and maxes must be uniform with step
    float xMin{-20.f}, yMin{0.f}, zMin{-20.f}, xMax{20.f}, yMax{6.f}, zMax{20.0f};
    float step{2.f};

    float highestX, highestY, highestZ, lowestX, lowestY, lowestZ;
    //int maxVertices{1000};




};







    ////// prøver igjen

//public:
//    LAS(std::string fileName);

//    std::vector<gsml::Vertex> getPositions() const;
//    std::vector<gsml::Vertex> getmVertices() const;
//    void setPositions(const std::vector<gsml::Vertex> &value);

//    void init(GLint matrixUniform) override;
//    void draw() override;

//private:
//    void ReadDatafromFile(std::string fileName);
//    void populatePosArr();

//    float PosArr[1000][2000];
//    void RemoveDeltaPos();
//    void GenerateHeightMap();
//    float CalcHeight(float x = 0, float z = 0);
//};

#endif // LAS_H
