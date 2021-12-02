#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"
#include "las.h"


struct Physics
{
    float radius = 0.25;
    float mass = 100;
    float friction = 0.009;
    float Gravitation = 9.81;

    gsml::Vector3d Acceleration{0.0, 0.0, -Gravitation};
    gsml::Vector3d Force{0.0,0.0,0.0};
    gsml::Vector3d Velocity{0,0,0};
    gsml::Vector3d OldVelocity{0,0,0};
    gsml::Vector3d airFlow{0,0,0};

    bool IsFalling{false};

    void Falling()
    {
        IsFalling = true;
        Acceleration = gsml::Vector3d(0, 0, -Gravitation);
        gsml::Vector3d u{0-Velocity.x,0-Velocity.y,0-Velocity.z};
        airFlow = gsml::Vector3d(u.x*u.x, u.y*u.y, u.z * u.z);
        Force = Acceleration * mass;
        Acceleration = {Force.x/mass, Force.y/mass, Force.z/mass};
    }
    void onGround(gsml::Vector3d N)
    {
        if(IsFalling == true)
            OldVelocity.z = 0;
        IsFalling = false;
        Acceleration = gsml::Vector3d(N.x * N.z, N.y * N.z, (N.z*N.z)-1) * Gravitation;

    }

};

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    RollingBall(int n, VisualObject *surface);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float x, float y, float z) ;
    void moveAlongLAs( float dt);
    void baryMove(float x, float y, float z);
    void setSurface2(VisualObject* surface);
    gsml::Vector3d Get_position();
    void setPosition(gsml::Vector3d v);
    void setHeight(float z);
    void heightAt();
    Physics* physics;
    float radius = 0.25f;
    float r = 1.f;
    
    void calculateBarycentricCoordinates(VisualObject *surface);
    void barycentricCords(float dt);
    void setSurface(VisualObject* surface);


protected:
    VisualObject* triangle_surface;
    VisualObject* _las;
private:
    std::vector<gsml::Vertex> surfVertices;
    gsml::Vector3d m_normal{0.0, 0.0, 1.0};
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    gsml::Vector3d mN{0.0, 0.0, 1.0};
    int old_index{0};
    int m_index{0};



};
#endif // ROLLINGBALL_H
