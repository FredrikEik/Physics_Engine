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
    gsml::Vector3d airF{0,0,0};

    bool IsFalling{false};

    void Falling()
    {
        IsFalling = true;
        Acceleration = gsml::Vector3d(0, 0, -Gravitation);
        calculateAirF();
    }
    void onGround(gsml::Vector3d N)
    {
        if(IsFalling == true)
            OldVelocity.z = 0;
        IsFalling = false;
        Acceleration = gsml::Vector3d(N.x * N.z, N.y * N.z, (N.z*N.z)-1) * Gravitation;
        calculateAirF();
    }
    void calculateAirF()
    {
        float p = 1.29; //mass density of air on earth
        gsml::Vector3d u{0-Velocity.x,0-Velocity.y,0-Velocity.z};//flow velocity
        float A = M_PI * (radius*radius); //Area
        float dc = 0.47; //drag coefficient
        //airF = 1/2 p * (u^2) * dc * A;
        airF = gsml::Vector3d(u.x*u.x, u.y*u.y, u.z * u.z);
        airF = airF * (0.5 * p);
        airF = airF * dc;
        airF = airF * A;

        Force = Acceleration * mass;
        Force = Force - airF;
        Acceleration = {Force.x/mass, Force.y/mass, Force.z/mass};
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
    void move(float dt) override ;
    void move(float x, float y, float z);
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
    
    bool switchVersion = false;
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
