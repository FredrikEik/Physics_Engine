#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"
#include "flatefil.h"

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override;
   // void move(float dx, float dy, float dz) override;
    void setSurface(VisualObject* surface); //{ triangle_surface = surface; }
    gsml::Vector3d Get_position();
    void setPosition(gsml::Vector3d v);
    void setHeight(float z);




protected:
    VisualObject* FalteFil;
private:

    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    gsml::Vector3d m_normal{0.0, 0.0, 1.0};
    gsml::Vector3d mAcceleration{0.0, 0.0, -lilleG};
    gsml::Vector3d mVelocity = {0,0,0};
    gsml::Vector3d oldVelocity = {0,0,0};
    gsml::Vector3d mGravForce{0,0, -(lilleG * mMass)};
    gsml::Vector3d m;
    std::vector<gsml::Vertex> vertices;

    int old_index{0};
    int m_index{0};
    float lilleG = 9.81;
    float mMass = 2;
    float friction = 0.9;


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
