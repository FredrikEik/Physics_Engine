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
    float my = 0.4;
    float lilleG = 9.81;

    gsml::Vector3d storeG{0.0, 0.0, -lilleG*mass};
    gsml::Vector3d Acceleration{0.0, 0.0, -lilleG};
    gsml::Vector3d Force{0.0,0.0,0.0};
    gsml::Vector3d Velocity{0,0,0};
    gsml::Vector3d oldVelocity{0,0,0};
    gsml::Vector3d Friction{0,0,0};
    gsml::Vector3d Nforce{0,0,0};

    bool frittfall{false};

    void freeFall()
    {
        frittfall = true;
        Acceleration = gsml::Vector3d(0, 0, -lilleG);
        calculateAirF(Acceleration);
    }
    void onGround(gsml::Vector3d N)
    {
        if(frittfall == true)
            oldVelocity.z = 0;
        frittfall = false;
        Acceleration = gsml::Vector3d(N.x * N.z, N.y * N.z, (N.z*N.z)-1) * lilleG;
        calculateAirF(N);
    }
    void calculateAirF(gsml::Vector3d N)
    {
        float p = 1.29; //mass density of air on earth
        gsml::Vector3d u{0-Velocity.x,0-Velocity.y,0-Velocity.z};//flow velocity
        float A = M_PI * (radius*radius); //Area
        float dc = 0.47; //drag coefficient

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

            Friction = tempF * my * lilleG * mass;

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
    std::string mTxt = "../VSIM101_H21_Rulleball_0/BSpline/BSpline";
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

// Finne trekant
//    for( /* indekser til flaten */ )
//    {
//        // Finn trekantens vertices v0 , v1 , v2
//        // Finn ballens posisjon i xy=planet
//        // Soek etter triangel som ballen er pa na
//        // med barysentriske koordinater
//        if( /* barysentriske koordinater mellom 0 og 1 */ )
//        {
//            // beregne normal
//            // beregn akselerasjonsvektor = ligning(7)
//            // Oppdaterer hastighet og posisjon
//            if( /* ny indeks != forrige indeks */ )
//            {
//                // Ballen har rullet over pa nytt triangel
//                // Beregner normalen til kollisjonsplanet,
//                // se ligning(9)
//                // Korrigere posisjon oppover i normalens retning
//                // Oppdater hastighetsvektoren, se ligning(8)
//                // Oppdatere posisjon i retning den nye
//                // hastighetsvektoren
//            }
//            // Oppdater gammel normal og indeks
//        }
//    }
