#ifndef SURFACE_H
#define SURFACE_H

#include "visualobject.h"


class Surface : public VisualObject
{
public:
    Surface();
    Surface(std::string filename);
    ~Surface() override;
    void writeFile(std::string filename);
    void readFile(std::string filename);
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // SURFACE_H
