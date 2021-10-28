#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"
//#include "vector3d.h" //for barycentric cordinates in gsml

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override;
    void setSurface(VisualObject* surface) { triangle_surface = surface; }

    //gsml::Vector3d getClosestTriangleToBall(std::vector<gsml::Vertex>);
protected:
    VisualObject* triangle_surface;
private:

    gsml::Vector3d velocity{0.0f, 0.0f, 0.0f};
    gsml::Vector3d gravity{0.f, 0.f, 9.8f};

    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
};
#endif // ROLLINGBALL_H
