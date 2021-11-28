#ifndef FILESURFACE_H
#define FILESURFACE_H

#include "visualobject.h"
#include "vertex.h"
struct ContourLines
{
    int n{5};
    std::vector<gsml::Vertex> mPoints;
    int offsetX{0};
    int offsetY{0};
    int offsetZ{0};
    float mMin{5};
    float mMax{5000};
};

class FileSurface : public VisualObject
{
public:
    FileSurface(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void readPoints(std::string filnavn);
    void makePlain();
    float calcHeight(float x, float y);
    std::vector<gsml::Vertex>& get_vertices() { return mMesh->mVertices; } // 191120

private:
    ContourLines* mCL;
    void calculateNormals();
    float xMin = 9000000;
    float xMax = 0;
    float yMin = 9000000;
    float yMax = 0;
    gsml::Vector2d MapMin;
    gsml::Vector2d MapMax;
    int n = 10;
    static const int X = 100; // (Max.x - Min.x) / n
    static const int Y = 147; // (Max.y - Min.y) / n
    std::vector<gsml::Vector3d> map[X][Y];
    //gsml::Vector3d m[X][Y];
    float mNormal[3];
};

#endif // FILESURFACE_H
