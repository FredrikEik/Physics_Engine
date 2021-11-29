#ifndef FLATEFIL_H
#define FLATEFIL_H

#include "visualobject.h"

struct CountoureLines
{
    int n{5};

    std::vector<gsml::Vertex> mPoints;
    float ofsetX = -10;
    float ofsetZ = -10;
    float ofsetY = -10;
    float mMin{5};
    float mMax{5000};

};

class FlateFil : public VisualObject
{
public:
    ~FlateFil() override;
    FlateFil(std::string filnavn);
    void readPoints(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void makePlain();
    float calcHeight(float x, float y);
    int width;

    std::vector<gsml::Vertex> getCountourPoints();
    std::vector<gsml::Vertex> CountourPoints;

private:
    CountoureLines *mCL;
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
    float mNormal[3];

    
};

#endif // FLATEFIL_H
