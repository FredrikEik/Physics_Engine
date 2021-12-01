#ifndef MARCHINGSQUARE_H
#define MARCHINGSQUARE_H

#include "flatefil.h"
#include "visualobject.h"

class MarchingSquare : public VisualObject
{
public:
    MarchingSquare(VisualObject* mSurf);
    ~MarchingSquare();
    void init(GLint matrixUniform) override;
    void draw() override;
    void setSquares();
    int getState(int a, int b, int c, int d);
    void drawLines(gsml::Vector2d v1, gsml::Vector2d v2);
    std::vector<std::vector<float>> fields;

private:
    VisualObject* surface;
    int iso = 5;

};

#endif // MARCHINGSQUARE_H
