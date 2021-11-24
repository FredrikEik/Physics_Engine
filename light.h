#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"

//using namespace gsls21;
class Light : public VisualObject
{
public:
    Light();
    ~Light() override;
    virtual void init(GLint matrixUniform) ;
    virtual void draw() override;

    GLfloat mAmbientStrenght{0.3f};
    QVector3D mAmbientColor{0.3f, 0.3f, 0.3f};

    GLfloat mLightStrenght{0.7f};
    QVector3D mLightColor{0.9f, 0.9f, 0.3f};

     QVector3D mLightPosition{8,5,0};

    GLfloat mSpecularStrenght{0.3f};
    GLint mSpecularExponent{4};
};

#endif // LIGHT_H
