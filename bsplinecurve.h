#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H

#include "visualobject.h"

class BSplineCurve : public VisualObject
{
public:
    BSplineCurve(int dID);
    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;
    void initialize();
    gsml::Vector3d deBoor(double x);
    int findKnotInterval(float x);
    void addC(std::string filnavn);
private:
    std::string mTxt = "../VSIM101_H21_Rulleball_0/BStxt/BSpline";
    int n = 9; //Antall kontrollpunkter
    int d = 2; //grad
    std::vector<gsml::Vector3d> c; //Kontrollpunkter
    std::vector<double> t {0,0,0,1,2,3,4,5,6,7,7,7}; //Skjøtevektor
};

#endif // BSPLINECURVE_H
