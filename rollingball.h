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
    virtual void move(float dx, float dy, float dz) override;
    void startSimulation();
    void setSurface(VisualObject* surface) { triangle_surface = surface; }
protected:
    VisualObject* triangle_surface;
    bool bIsSimulating{false};
private:
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
};
#endif // ROLLINGBALL_H
