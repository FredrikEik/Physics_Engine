#ifndef FLATEFIL_H
#define FLATEFIL_H

#include "visualobject.h"


class FlateFil : public VisualObject
{

private:
    static const int X = 100; // (Max.x - Min.x) / n
    static const int Y = 147; // (Max.y - Min.y) / n

public:
    FlateFil(std::string filnavn);

    int getX(){return X;};
    int getY(){return Y;};
    void readPoints(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void makePlain();
    float calcHeight(float x, float y);
    int width;


    float mHPoints[X][Y];


private:

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

    int offsetX{614580};
    int offsetY{6757290};
    int offsetZ{500};

    std::vector<gsml::Vector3d> map[X][Y];

    
};

#endif // FLATEFIL_H
