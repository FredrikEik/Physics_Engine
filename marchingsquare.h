#ifndef MARCHINGSQUARE_H
#define MARCHINGSQUARE_H
#include "visualobject.h"
#include "filesurface.h"

class MarchingSquare : public VisualObject
{
public:
    MarchingSquare(VisualObject* mSurf);
    ~MarchingSquare(){};
    void init(GLint matrixUniform) override;
    void draw() override;
    void setSquares();
    int getCase(int a, int b, int c, int d);
    void drawLine(gsml::Vector2d a, gsml::Vector2d b);
private:
    VisualObject* surface;
    int iso = 5;
    int floor = 1;

};

#endif // MARCHINGSQUARE_H
