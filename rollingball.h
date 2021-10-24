#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"
#include "vector3d.h"
#include "matrix4x4.h"



class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override;
    void setSurface(VisualObject* surface) { triangle_surface = surface; }
    void barycentricCoordinates();
    float nextYPos;
    double radius = 0.25;
    gsml::Vector3d barycentricCord;
protected:
    VisualObject* triangle_surface;

private:
    gsml::Vector3d hastighet{0.0f,0.0f,0.0f};
    gsml::Vector3d akselerasjon{0.0f,0.0f,0.0f};
    gsml::Vector3d gAkselerasjon{0.0f,0.0f,-9.80565f};
    gsml::Vector3d gKraft;
    float masseIKG{0.01f};


    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
};
#endif // ROLLINGBALL_H
