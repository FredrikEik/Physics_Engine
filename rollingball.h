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
    void move(float dt) override;
    void setSurface(VisualObject* surface) { triangle_surface = surface; }

    void setBallPosition(gsml::Vector3d pos);
    gsml::Vector3d getBallPosition();

    float radius = 0.25;

protected:
    VisualObject* triangle_surface;
private:
    int old_index{0};
    gsml::Vector3d nextPos{0,0,0};
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};   
    gsml::Vector3d point1{0,0,0}, point2{0,0,0}, point3{0,0,0};
    gsml::Vector3d BarycentricCoordinates{0,0,0};

    gsml::Vector3d BallPosition{0,0,0};
    gsml::Vector3d speed{0,0,0};
    gsml::Vector3d acceleration{0,0,0};
    gsml::Vector3d gForce{0,0,0};
    const gsml::Vector3d constGForce{0,0,-9.80565f}; //Gravitational force (downwards) is around -9.8
    const float mass{0.05f};



};
#endif // ROLLINGBALL_H
