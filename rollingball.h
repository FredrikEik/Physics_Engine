#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include <stdio.h>
#include "octahedronball.h"
#include "trianglesurface.h"

//struct Physics
//{
//    float lilleG = 9.81;
//    float friction = 0.8;
//    float mass = 1;
//    gsml::Vector3d mAcceleration{0.0, 0.0, -lilleG};
//    gsml::Vector3d mForce{0.0,0.0,0.0};
//    gsml::Vector3d mVelocity = {0,0,0};
//    gsml::Vector3d oldVelocity = {0,0,0};
//};

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
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
protected:
    VisualObject* triangle_surface;
private:
    float mRadius = 0.25;
    float mass = 5;
    float friction = 0.9;
    float lilleG = 9.81;

    gsml::Vector3d mAcceleration{0.0, 0.0, -lilleG};
    gsml::Vector3d mForce{0.0,0.0,0.0};
    gsml::Vector3d mVelocity = {0,0,0};
    gsml::Vector3d oldVelocity = {0,0,0};
    gsml::Vector3d m_normal{0.0, 0.0, 1.0};
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    gsml::Vector3d mN{0.0, 0.0, 1.0};

    int m_index{0};
    int old_index{0};

    bool frittfall{false};
    std::vector<gsml::Vertex> vertices;
    //Physics* ph;
};



#endif // ROLLINGBALL_H
