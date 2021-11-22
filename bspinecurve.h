#ifndef BSPINECURVE_H
#define BSPINECURVE_H

#include "visualobject.h"

class BSpineCurve : public VisualObject
{

public:
    BSpineCurve();
    void initialize();
    gsml::Vector3d deBoor(double x);
    //Beregn et punkt på kurven
    //vha deboor's algoritme

    int findKnotInterval(float x);
    void draw();


private:

    int n; //Antall kontrollpunkter
    int d =2; //grad
    std::vector<gsml::Vector3d> c; //Kontrollpunkter
    std::vector<double> t; //Skjøtevektor
    //Posisjon etc i visualObject
};

#endif // BSPINECURVE_H
