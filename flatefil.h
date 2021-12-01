#ifndef FLATEFIL_H
#define FLATEFIL_H

#include "visualobject.h"

class CountoureLines : public VisualObject
{
public:
    CountoureLines(std::vector<gsml::Vertex> mCPoints);
    ~CountoureLines();
    void init(GLint matrixUniform) override;
    void draw() override;

    int n{5};

private:

};

class FlateFil : public VisualObject
{
public:
    ~FlateFil() override;
    FlateFil(std::string filnavn);

    int getX(){return X;};
    int getY(){return Y;};
    void readPoints(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void makePlain();
    float calcHeight(float x, float y);
    int width;

    std::vector<gsml::Vertex> mCPoints;
    std::vector<gsml::Vertex> getCountourPoints();
    std::vector<gsml::Vertex> CountourPoints;

    float offsetX{0};
    float offsetZ{0};
    float offsetY{550};
private:
    CountoureLines *mCL;
    void calculateNormals();
    float xMin = 9000000;
    float xMax = 0;
    float yMin = 9000000;
    float yMax = 0;
    float zMin = 9000000;
    float zMax = -900;
    gsml::Vector2d MapMin;
    gsml::Vector2d MapMax;
    int n = 10;
    static const int X = 100; // (Max.x - Min.x) / n
    static const int Y = 147; // (Max.y - Min.y) / n
    std::vector<gsml::Vector3d> map[X][Y];

    
};

#endif // FLATEFIL_H
