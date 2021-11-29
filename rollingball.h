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
    float mass = 5;
    //float friction = 0.9;
    float lilleG = 9.81;

    gsml::Vector3d Acceleration{0.0, 0.0, -lilleG};
    gsml::Vector3d Force{0.0,0.0,0.0};
    gsml::Vector3d Velocity{0,0,0};
    gsml::Vector3d oldVelocity{0,0,0};
    gsml::Vector3d airF{0,0,0};

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
        //airF = 1/2 * p * (u * u) * dc * A;
        airF = gsml::Vector3d(u.x*u.x, u.y*u.y, u.z * u.z);
        airF = airF * (1/2 * p);
        airF = airF * dc;
        airF = airF * A;

        Force = Acceleration * mass;
        Force = Force - airF;
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
    void contructBspline(gsml::Vector3d dP);
    void saveRoute(std::string filnavn);
//    BSplineCurve *mBSpline;
protected:
    VisualObject* triangle_surface;
private:
    int mID{0};

    std::string mTxt = "../VSIM101_H21_Rulleball_0/BSpline";
    std::vector<gsml::Vector3d> mbsPoint;
    gsml::Vector3d bsPoint{0, 0, 0};

    gsml::Vector3d m_normal{0, 0, 1};
    gsml::Vector3d old_normal{0, 0, 1};
    gsml::Vector3d mN{0, 0, 1};

    int m_index{0};
    int old_index{0};
    std::vector<gsml::Vertex> vertices;

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
