#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H

#include "visualobject.h"

class BSplineCurve : public VisualObject
{

public:
    BSplineCurve(int dID);
    ~BSplineCurve();

    void initialize();
    gsml::Vector3d deBoor(double x);

    virtual void init(GLint matrixUniform)override;
    virtual void draw() override;

    int findKnotInterval(float x);
    void addC(std::string filnavn);

private:
    std::string mTxt = "../VSIM101_H21_Rulleball_0/BSpline.txt";
    std::vector<gsml::Vector3d> c; //Kontrollpunkter
    int n = 4; //Antall kontrollpunkter
    int d =2; //grad
    std::vector<double> t{0,0,0,1,2,2,2}; //Skjøtevektor
    //Posisjon etc i visualObject
};

#endif // BSPLINECURVE_H
