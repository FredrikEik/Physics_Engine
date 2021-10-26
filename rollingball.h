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
    float getHeight(gsml::Vector3d& positon);
    void calculateMovement(const gsml::Vector3d& p, const gsml::Vector3d& q, const gsml::Vector3d& r);
    void checkForCollition();
protected:
    VisualObject* triangle_surface;
private:
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    int current_index{0};

    gsml::Vector3d velocity{};
    gsml::Vector3d acceleration{};
    gsml::Vector3d baseForce{};
    gsml::Vector3d baseAcceleration{0.f, 0.f, -9.81f};
    float mass{2.f};
    const float gravity{-9.81f};
    const float radius{0.25f};
};
#endif // ROLLINGBALL_H
