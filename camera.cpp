#include "camera.h"

Camera::Camera()
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    //mInputComp = new InputComponent;
}

Camera::~Camera()
{
}

void Camera::pitch(float degrees)
{
    //  rotate around mRight
    mPitch -= degrees;
    updateForwardVector();
}

void Camera::yaw(float degrees)
{
    // rotate around mUp
    mYaw -= degrees;
    updateForwardVector();
}

void Camera::updateRightVector()
{
    mRight = mForward^mUp;
    mRight.normalize();
    //    qDebug() << "Right " << mRight;
}

void Camera::updateForwardVector()
{
    mRight = gsml::Vector3d(1.f, 0.f, 0.f);
    mRight.rotateY(mYaw);
    mRight.normalize();



    mUp = gsml::Vector3d(0.f, 1.f, 0.f);
    mUp.rotateX(mPitch);
    mUp.normalize();

    mForward = mUp^mRight;

    updateRightVector();
}

void Camera::update()
{
    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    mPitchMatrix.rotate(mPitch, 1, 0, 0);
    mYawMatrix.rotate(mYaw, 0, 0, 1);
    //mPitchMatrix.rotateX(mPitch);
    //mYawMatrix.rotateY(mYaw);

    mPosition = mPosition + (mForward * mSpeed);

    mViewMatrix = mPitchMatrix* mYawMatrix;
    mViewMatrix.translate(mPosition.x, mPosition.y, mPosition.z);
}

void Camera::calculateProjectionMatrix()
{
    mProjectionMatrix.perspective(60, 1, 1, 50);
}

void Camera::setPosition(const gsml::Vector3d &position)
{
    mPosition = position;
}

void Camera::setSpeed(float speed)
{
    mSpeed = speed;
}

void Camera::updateHeigth(float deltaHeigth)
{
    mPosition.z += deltaHeigth;
}

void Camera::moveRight(float delta)
{
    //This fixes a bug in the up and right calculations
    //so camera always holds its height when straifing
    //should be fixed thru correct right calculations!
    gsml::Vector3d right = mRight;
    right.z = 0.f;
    mPosition = mPosition + (right * delta);
}

gsml::Vector3d Camera::position() const
{
    return mPosition;
}

gsml::Vector3d Camera::up() const
{
    return mUp;
}

gsml::Vector3d Camera::right() const
{
    return mRight;
}

