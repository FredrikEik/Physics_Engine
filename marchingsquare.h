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
    void setSquares();
    int getState(int a, int b, int c, int d);
    void drawLine(gsml::Vector2d a, gsml::Vector2d b);
    void draw();

private:
    VisualObject* surface;
    int iso = 1;
    int floor = 1;
    int nrOfFloors = 25;

};

#endif // MARCHINGSQUARE_H
