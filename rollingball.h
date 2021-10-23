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
    bool findTriangle(unsigned int index, gsml::Vector3d& position, gsml::Vector3d& outBaryCords, gsml::Vector3d& outP, gsml::Vector3d& outQ, gsml::Vector3d& outR) const;
    float getHeight() const;
protected:
    VisualObject* triangle_surface;
private:
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
};
#endif // ROLLINGBALL_H
