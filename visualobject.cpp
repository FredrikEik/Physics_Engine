// VisualObject.cpp
#include "visualobject.h"
#include "Matrix4x4/matrix4x4.h"
VisualObject::VisualObject()
{
    mMatrix.setToIdentity();
    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
}

VisualObject::~VisualObject()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

QVector3D VisualObject::getPosition()
{

    return mPosition.getRow(3).toVector3D();
}



