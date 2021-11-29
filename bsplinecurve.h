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
    //Beregn et punkt på kurven
    //vha deboor's algoritme

    int findKnotInterval(float x);
    void addC(std::string filnavn);
private:
    std::string mTxt = "../VSIM101_H21_Rulleball_0/BSpline";
    int n = 9; //Antall kontrollpunkter
    int d = 2; //grad
    std::vector<gsml::Vector3d> c; //Kontrollpunkter
    std::vector<double> t {0,0,0,1,2,2,2}; //Skjøtevektor
    //Posisjon etc i visualObject
};

#endif // BSPLINECURVE_H
