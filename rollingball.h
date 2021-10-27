#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override;
    void setSurface(VisualObject* surface) { triangle_surface = surface; }

    //gsml::Vector3d calculateNewForce(gsml::Vector3d normal1, gsml::Vector3d normal2, float dt);

protected:
    VisualObject* triangle_surface;
private:
    gsml::Vector3d force;
    gsml::Vector3d velocity{0.f, 0.0f, 0.0f};
    gsml::Vector3d accel{0.0f, 0.0f, 0.0f}; //acceleration
    gsml::Vector3d gAccel{0.0f, 0.0f, -9.80565f}; //gravity
    float kiloMass {2.0f};

    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};

};
#endif // ROLLINGBALL_H
