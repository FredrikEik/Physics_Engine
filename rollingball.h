#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    RollingBall(int n, VisualObject *surface);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override ;
        float radius = 0.25f;
        float r = 1.f;
    
        bool switchVersion = false;
        void calculateBarycentricCoordinates(VisualObject *surface);
    void barycentricCords(float dt);
    void barycentricCords2(float dt);
    void setSurface(VisualObject* surface) { triangle_surface = surface; }

        int mPrevIndex{0};
            VisualObject *mTriangleSurface{nullptr};

protected:
    VisualObject* triangle_surface;
private:

    float speed = 0.001f;
    float mMass{0.5};
    float mass = .01f;
    gsml::Vector3d velocity{};
    gsml::Vector3d acceleration{};
    gsml::Vector3d gravity{0,-9.81f,0};
    float gravitation = 9.81;
    
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    gsml::Vector3d prevTriangleNormal{0};

    gsml::Vector3d speed{0.0f,0.0f,0.0f};
    gsml::Vector3d velocity{0.0f,0.0f,0.0f};
    gsml::Vector3d gAkselerasjon{0.0f,0.0f,-9.81f};
    gsml::Vector3d gKraft;

    bool notOnSurface = false;

};
#endif // ROLLINGBALL_H
