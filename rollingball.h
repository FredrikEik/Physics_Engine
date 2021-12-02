#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"
#include "las.h"
#include "physicscomponent.h"


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

    //Physics* physics;

    PhysicsComponent *physics;
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
