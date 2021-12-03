#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H
#include "matrix4x4.h"
#include "vector3d.h"

class PhysicsComponent
{
public:
    //lager fysikken til ballen

    PhysicsComponent();
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
    //funksjon for hvis objektet faller
    void Falling()
    {
        IsFalling = true;
        Acceleration = gsml::Vector3d(0, 0, -Gravitation);
        gsml::Vector3d u{0-Velocity.x,0-Velocity.y,0-Velocity.z};
        airFlow = gsml::Vector3d(u.x*u.x, u.y*u.y, u.z * u.z);
        Force = Acceleration * mass;
        Acceleration = {Force.x/mass, Force.y/mass, Force.z/mass};
    }
    //funksjon for hvis objectet er på bakken
    void onGround(gsml::Vector3d N)
    {
        if(IsFalling == true)
            OldVelocity.z = 0;
        IsFalling = false;
        Acceleration = gsml::Vector3d(N.x * N.z, N.y * N.z, (N.z*N.z)-1) * Gravitation;

    }
};

#endif // PHYSICSCOMPONENT_H
