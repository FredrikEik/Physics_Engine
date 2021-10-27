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
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setSurface(VisualObject* surface) { triangle_surface = surface; }

protected:

    VisualObject* triangle_surface;
private:

    gsml::Vector3d velocity{0.0f, 0.0f, 0.0f};
    gsml::Vector3d acceleration{0.0f, 0.0f, 0.0f};
    gsml::Vector3d gForce;
    gsml::Vector3d gAcceleration{0.0f, 0.0f, -9.80565f};
    gsml::Vector3d friction{0.0f, 0.0f, 0.0f};
    float frictionStrength{2.0f};
    float massInKg {2.5f};


    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
};
#endif // ROLLINGBALL_H
