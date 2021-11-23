#ifndef LIGHT_H
#define LIGHT_H
#include <QOpenGLFunctions_4_1_Core>
#include "visualobject.h"

class Light : public VisualObject
{
public:
    Light();
    ~Light(){};
    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;

    GLfloat mAmbientStrenght{0.7f};
    gsml::Vector3d mAmbientColor{0.9f, 0.2f, 0.9f};

    GLfloat mLightStrenght{0.7f};
    gsml::Vector3d mLightColor{0.3f, 0.5f, 0.15f};

    GLfloat mSpecularStrenght{0.3f};
    GLint mSpecularExponent{4};
};

#endif // LIGHT_H
