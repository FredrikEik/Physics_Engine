#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include "vector3d.h"
#include "visualobject.h"
/**
  This class still have some bugs. It mostly work, but when you rotate the camera 180 degrees
  the forward / backward is wrong, when steered with W and S.
 */

//struct FrustumComponent
//{
//    float mFOV{90.f};
//    float mAspectRatio{1.f};
//    float mFarPlaneDistance{100.f};
//    float mNearPlaneDistance{0.1};
//};

//class Camera;

//class FrustumSystem : public VisualObject
//{
//public:
//    FrustumSystem(Camera* C);
//    ~FrustumSystem();
//    virtual void init(GLint matrixUniform) override;
//    void calculateFrustumVectors();
//    void makeFrustumLines();
//    void updateFrustumPos();
//    bool insideFrustum(gsml::Vector3d pos);

//private:
//    Camera* mCam;
//    FrustumComponent* mFrustum;
//    gsl::Vector3D mNearPlane;
//    gsl::Vector3D mFarPlane;

//    gsl::Vector3D mRightTopNear;
//    gsl::Vector3D mRightTopFar;
//    gsl::Vector3D mRightBotNear;
//    gsl::Vector3D mRightBotFar;
//    gsl::Vector3D mLeftTopNear;
//    gsl::Vector3D mLeftTopFar;
//    gsl::Vector3D mLeftBotNear;
//    gsl::Vector3D mLeftBotFar;

//};

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
    //FrustumComponent mFrustumComp;

    gsml::Matrix4x4 mViewMatrix;
    gsml::Matrix4x4 mProjectionMatrix;

    void setPosition(const gsml::Vector3d &position);
    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);

    //InputComponent* mInputComp;
    gsml::Vector3d position() const;
    gsml::Vector3d up() const;
    gsml::Vector3d right() const;

private:
    gsml::Vector3d mForward{0.f, 0.f, -1.f};
    gsml::Vector3d mRight{1.f, 0.f, 0.f};
    gsml::Vector3d mUp{0.f, 1.f, 0.f};

    gsml::Vector3d mPosition{0.f, 0.f, 0.f};
    float mPitch{0.f};
    float mYaw{0.f};

    gsml::Matrix4x4 mYawMatrix;
    gsml::Matrix4x4 mPitchMatrix;

    float mSpeed{0.f}; //camera will move by this speed along the mForward vector
};

#endif // CAMERA_H

