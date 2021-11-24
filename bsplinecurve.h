#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H

#include "visualobject.h"

//skjøtvektor eksempel {0,0,0,1,2,2}

class bSplineCurve : public VisualObject
{
private:
    int n; //antall kontrollpunkter
    int d; //degrees
    std::vector<QVector3D> c; //kontrollpunkter
    std::vector<double> t; //skjøtvektor

public:
    bSplineCurve();
    void initialize();
    QVector3D deBoor(double t);
    //beregner et punkt på kurven
    //rha deBoor's algoritme
    void draw();
};

#endif // BSPLINECURVE_H
