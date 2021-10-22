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
    
        void calculateBarycentricCoordinates(VisualObject *surface);
    
    void setSurface(VisualObject* surface) { triangle_surface = surface; }
protected:
    VisualObject* triangle_surface;
private:
    float radius = 1.f;
    float speed = 0.001f;
    float mass = 1.f;
    gsml::Vector3d velocity{};
    gsml::Vector3d acceleration{};
    gsml::Vector3d gravity{0,-9.81f,0};
    
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    gsml::Vector3d prevTriangleNormal{0};

};
#endif // ROLLINGBALL_H
