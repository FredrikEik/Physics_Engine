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
protected:
    VisualObject* triangle_surface;
private:
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    gsml::Vec3 velocity{0.0,0.0,0.0};
    gsml::Vec3 acceleration{0.0,0.0,0.0};
    gsml::Vec3 gForce{0.0,0.0,0.0};
    gsml::Vec3 gAcceleration{0.0,0.0,9.8};
    float mass = 1;
};
#endif // ROLLINGBALL_H
