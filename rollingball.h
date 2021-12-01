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
    float storeG = lilleG * mass;
    gsml::Vector3d Acceleration{0.0, 0.0, -lilleG};
    gsml::Vector3d Force{0.0,0.0,0.0};
    gsml::Vector3d Velocity{0,0,0};
    gsml::Vector3d oldVelocity{0,0,0};
    gsml::Vector3d Friction{0,0,0};
    //gsml::Vector3d Nforce{0,0,0};

    bool frittfall{false};

    void freeFall()
    {
        frittfall = true;
        Acceleration = gsml::Vector3d(0, 0, -lilleG);
        calculateAirF();
    }
    void onGround(gsml::Vector3d N)
    {
        if(frittfall == true)
            oldVelocity.z = 0;
        frittfall = false;
        Acceleration = gsml::Vector3d(N.x * N.z, N.y * N.z, (N.z*N.z)-1) * lilleG;
        calculateAirF();
    }
    void calculateAirF()
    {
        float p = 1.29; //mass density of air on earth
        gsml::Vector3d u{0-Velocity.x,0-Velocity.y,0-Velocity.z};//flow velocity
        float A = M_PI * (radius*radius); //Area
        float dc = 0.47; //drag coefficient
        //airF = 1/2 p * (u^2) * dc * A;
        Friction = gsml::Vector3d(u.x*u.x, u.y*u.y, u.z * u.z);
        Friction = Friction * (0.5 * p);
        Friction = Friction * dc;
        Friction = Friction * A;

        Force = Acceleration * mass;

        if(!frittfall)
        {
            gsml::Vector3d tempF = Force;
            tempF.normalize();
            tempF = tempF * -1;

            Friction = tempF * storeG;
            Friction = Friction * my;
        }

        Force = Force + Friction;

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
    std::string mTxt = "../VSIM101_H21_Rulleball_0/BSpline";
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
