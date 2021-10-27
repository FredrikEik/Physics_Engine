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
    void move(float dt) override ;
        float radius = 0.25f;

        void calculateBarycentricCoordinates(VisualObject *surface);
    void barycentricCords(float dt);
    void setSurface(VisualObject* surface) { triangle_surface = surface; }

protected:
    VisualObject* triangle_surface;
private:

    float speed = 0.001f;
    float mass = .007f;
    gsml::Vector3d velocity{};
    gsml::Vector3d acceleration{};
    gsml::Vector3d gravity{0,-9.81f,0};

    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    gsml::Vector3d prevTriangleNormal{0};

    gsml::Vector3d hastighet{0.0f,0.0f,0.0f};
    gsml::Vector3d akselerasjon{0.0f,0.0f,0.0f};
    gsml::Vector3d gAkselerasjon{0.0f,0.0f,-9.81f};
    gsml::Vector3d gKraft;


};
#endif // ROLLINGBALL_H
