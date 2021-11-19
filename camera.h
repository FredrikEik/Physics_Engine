#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
#include "visualobject.h"
/**
  This class still have some bugs. It mostly work, but when you rotate the camera 180 degrees
  the forward / backward is wrong, when steered with W and S.
 */


class Camera
{
public:
    Camera();
    ~Camera();
    void pitch(float degrees);
    void yaw(float degrees);
    void updateRightVector();
    void updateForwardVector();
    void update();
    void calculateProjectionMatrix();

    gsml::Matrix4x4 mViewMatrix;
    gsml::Matrix4x4 mProjectionMatrix;

    void setPosition(const gsml::Vector3d &position);
    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);

    gsml::Vector3d position() const;
    gsml::Vector3d up() const;
    gsml::Vector3d right() const;

private:
    gsml::Vector3d mForward{0.f, 1.f, 0.f};
    gsml::Vector3d mRight{1.f, 0.f, 0.f};
    gsml::Vector3d mUp{0.f, 0.f, 1.f};

    gsml::Vector3d mPosition{0.f, 0.f, 0.f};
    float mPitch{0.f};
    float mYaw{0.f};

    gsml::Matrix4x4 mYawMatrix;
    gsml::Matrix4x4 mPitchMatrix;

    float mSpeed{0.f}; //camera will move by this speed along the mForward vector
};

#endif // CAMERA_H

