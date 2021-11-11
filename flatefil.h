#ifndef FLATEFIL_H
#define FLATEFIL_H

#include "visualobject.h"

class FlateFil : public VisualObject
{
public:
    ~FlateFil() override;
    FlateFil(std::string filnavn);
    void readFile(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void makePlain();

private:
    std::vector<gsml::Vector3d> points;

    gsml::Vector2d MapMin;
    gsml::Vector2d MapMax;
    int n = 5;

    float xMin = 9000000;
    float xMax = 0;
    float yMin = 9000000;
    float yMax = 0;
    float zMin = 9000000;
    float zMax = 0;
};

#endif // FLATEFIL_H
