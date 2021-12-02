#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include <cmath>
#include <stdio.h>
#include "octahedronball.h"
#include "trianglesurface.h"
#include "bsplinecurve.h"

struct Physics
{
    float radius = 0.25;
    float mass = 1;
    float my = 0.3;
    float lilleG = 9.81;
    gsml::Vector3d storeG{0.0, 0.0, -lilleG * mass};
    gsml::Vector3d Acceleration{0.0, 0.0, -lilleG};
    gsml::Vector3d Force{0.0,0.0,0.0};
    gsml::Vector3d Velocity{0,0,0};
    gsml::Vector3d oldVelocity{0,0,0};
    gsml::Vector3d Friction{0,0,0};
    gsml::Vector3d airF{0,0,0};
    gsml::Vector3d Nforce{0,0,0};

    void freeFall()
    {
        Nforce = {0,0,0};
        Friction = {0,0,0};

        calculateAirF();
    }
    void onGround(gsml::Vector3d N)
    {
        Nforce = N * -storeG.z;

        Friction = oldVelocity * -1;
        Friction.normalize();
        Friction = Friction * (Nforce.length() * my);

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

        Force = storeG + Nforce + Friction + airF;
        Acceleration = {Force.x/mass, Force.y/mass, Force.z/mass};
    }
};

class RollingBall : public VisualObject
{
public:
    RollingBall(int dID);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override;
    void move(float dx, float dy, float dz) override;
    void baryMove(float x, float y, float z);
    void setSurface(VisualObject* surface); //{ triangle_surface = surface; }
    gsml::Vector3d Get_position();
    void setPosition(gsml::Vector3d v);
    void setHeight(float z);
    void heightAt();
    Physics* p;
    void setMesh(Mesh* uMesh);
    void constructBSpline(gsml::Vector3d dP);
    void saveRoute(std::string filnavn);
protected:
    VisualObject* triangle_surface;
private:
    std::string mTxt = "../VSIM101_H21_Rulleball_0/BStxt/BSpline";
    gsml::Vector3d bsPoint{0,0,0};
    std::vector<gsml::Vector3d> mbsPoints;
    gsml::Vector3d m_normal{0.0, 0.0, 1.0};
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    gsml::Vector3d mN{0.0, 0.0, 1.0};
    int m_index{0};
    int old_index{0};
    std::vector<gsml::Vertex> surfVertices;
};



#endif // ROLLINGBALL_H
