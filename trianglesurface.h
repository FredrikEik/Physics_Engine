#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"
#include "vector3d.h"

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    TriangleSurface(std::string filnavn);
    ~TriangleSurface() override;
    void readFile(std::string filnavn);
    void writeFile(std::string filnavn);
    void init(GLint matrixUniform) override;
    void draw() override;
    void construct();
    void construct_cylinder();
    void construct_plane();
    std::vector<gsml::Vertex>& get_vertices() { return mVertices; } // 191120
    float getTerrainHeight(const QVector3D &position);
    int getFirstTriangleIndex(const QVector3D &position);
    QVector3D getTriangleNormal(size_t startIndex);
protected:
    bool findTriangle(unsigned int index, const QVector3D &position, QVector3D &outBaryCoord,
                      QVector3D &outP, QVector3D &outQ, QVector3D &outR);
    QVector3D barycentricCoordinates(const QVector2D &position, const QVector2D &p1, const QVector2D &p2, const QVector2D &p3);
};

#endif // TRIANGLESURFACE_H
