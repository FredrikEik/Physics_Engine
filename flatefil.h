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
    float calcHeight(float x, float y);

    //std::vector<gsml::Vertex>& get_vertices() { return mVertices; } // 191120

private:
    constexpr int getColumn(){float temp = MapMax.x-MapMin.x; temp = temp/n;
                   int X = static_cast<int>(temp);
                   return X;};
    constexpr int getRow()  {float temp = MapMax.y-MapMin.y; temp = temp/n;
                   int Y = static_cast<int>(temp);
                   return Y;};

    std::vector<gsml::Vector3d> myMap[getColumn()][getRow()];
    std::vector<gsml::Vector3d> points;
    float xMin = 9000000;
    float xMax = 0;
    float yMin = 9000000;
    float yMax = 0;
    gsml::Vector2d MapMin;
    gsml::Vector2d MapMax;
    int n = 5;
};

#endif // FLATEFIL_H
