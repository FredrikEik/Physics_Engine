#include <QDebug>
#include <QImage>
#include <cstdio>
#include <iostream>
#include "trianglesurface.h"
#include "vertex.h"
#include "qvector2d.h"
TriangleSurface::TriangleSurface() : VisualObject()
{
    gsml::Vertex v{};
    v.set_xyz(0,0,0); v.set_rgb(1,0,0); mVertices.push_back(v);
    v.set_xyz(0.5,0,0); v.set_rgb(0,1,0); mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(0,0,1); mVertices.push_back(v);
    v.set_xyz(0,0,0); v.set_rgb(0,1,0); mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(1,0,0); mVertices.push_back(v);
    v.set_xyz(0,0.5,0); v.set_rgb(0,0,1); mVertices.push_back(v);
}

TriangleSurface::TriangleSurface(std::string filnavn) : VisualObject()
{
    readFile(filnavn);
    //mMatrix.setToIdentity();
    //mMatrix.translate(0,0,5);
}

TriangleSurface::~TriangleSurface()
{
    //qDebug() << "TriangleSurface::~TriangleSurface()";
   //delete [] m_vertices;
    //qDebug() << "TriangleSurface::~TriangleSurface() - vertices deleted";
}

void TriangleSurface::readFile(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open())
    {
        int n;
        gsml::Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (int i=0; i<n; i++)
        {
             inn >> vertex;
             mVertices.push_back(vertex);
        }
        inn.close();
    }
}

void TriangleSurface::writeFile(std::string filnavn)
{
    std::ofstream ut;
    ut.open(filnavn.c_str());

    if (ut.is_open())
    {

        auto n = mVertices.size();
        gsml::Vertex vertex;
        ut << n << std::endl;
        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            //vertex = *it;
            ut << *it << std::endl;
        }
        ut.close();
    }
}
void TriangleSurface::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(gsml::Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());

}

void TriangleSurface::construct()
{
   float xmin=0.0f, xmax=1.0f, ymin=0.0f, ymax=1.0f, h=0.25f;
   for (auto x=xmin; x<xmax; x+=h)
       for (auto y=ymin; y<ymax; y+=h)
       {
           float z = sin(M_PI*x)*sin(M_PI*y);
           mVertices.push_back(gsml::Vertex{x,y,z,x,y,z});
           z = sin(M_PI*(x+h))*sin(M_PI*y);
           mVertices.push_back(gsml::Vertex{x+h,y,z,x,y,z});
           z = sin(M_PI*x)*sin(M_PI*(y+h));
           mVertices.push_back(gsml::Vertex{x,y+h,z,x,y,z});
           mVertices.push_back(gsml::Vertex{x,y+h,z,x,y,z});
           z = sin(M_PI*(x+h))*sin(M_PI*y);
           mVertices.push_back(gsml::Vertex{x+h,y,z,x,y,z});
           z = sin(M_PI*(x+h))*sin(M_PI*(y+h));
           mVertices.push_back(gsml::Vertex{x+h,y+h,z,x,y,z});
       }
}

void TriangleSurface::construct_cylinder()
{
    float h=0.5;
    const int SEKTORER=12;
    float t=2*M_PI/SEKTORER;
    mVertices.clear();
    for (int k=0; k<2; k++)
    {
        for (int i=0; i<SEKTORER; i++)
        {
            float x0=cos(i*t);
            float y0=sin(i*t);
            float z0=h*k;
            float x1=cos((i+1)*t);
            float y1=sin((i+1)*t);
            float z2=h*(k+1);
            mVertices.push_back(gsml::Vertex{x0,y0,z0,0,0,1,0,0});
            mVertices.push_back(gsml::Vertex{x0,y0,z2,0,0,1,1,0});
            mVertices.push_back(gsml::Vertex{x1,y1,z0,0,0,1,0,1});
            mVertices.push_back(gsml::Vertex{x0,y0,z2,1,1,0,0,1});
            mVertices.push_back(gsml::Vertex{x1,y1,z2,1,1,0,1,0});
            mVertices.push_back(gsml::Vertex{x1,y1,z0,1,1,0,1,1});
/*            float x0=cos(i*t);
            float y0=sin(i*t);
            float z0=h*k;
            float x1=cos((i+1)*t);
            float y1=sin((i+1)*t);
            float z2=h*(k+1);
            mVertices.push_back(Vertex{x0,y0,z0,0,0,1});
            mVertices.push_back(Vertex{x1,y1,z0,0,0,1});
            mVertices.push_back(Vertex{x0,y0,z2,0,0,1});
            mVertices.push_back(Vertex{x0,y0,z2,1,1,0});
            mVertices.push_back(Vertex{x1,y1,z0,1,1,0});
            mVertices.push_back(Vertex{x1,y1,z2,1,1,0});
*/        }
    }
}

void TriangleSurface::construct_plane()
{
    float dx=2.0;
    float dy=2.0;
    mVertices.clear();
    for (float y=-2.0; y<2.0; y+=dy)
    {
        for (float x=-3.0; x<3.0; x+=dx)
        {
            float x0=x;
            float y0=y;
            float x1=x0+dx;
            float y1=y0+dy;
            mVertices.push_back(gsml::Vertex{x0,y0,0,0,0.5,0});
            mVertices.push_back(gsml::Vertex{x1,y0,0,0,0.5,0});
            mVertices.push_back(gsml::Vertex{x0,y1,0,0,0.5,0});
            mVertices.push_back(gsml::Vertex{x0,y1,0,0,0.5,0});
            mVertices.push_back(gsml::Vertex{x1,y0,0,0,0.5,0});
            mVertices.push_back(gsml::Vertex{x1,y1,0,0,0.5,0});
        }
    }
}

float TriangleSurface::getTerrainHeight(const QVector3D &position)
{
    // Finding the height of the position, using barycentric coordinates
    float height{};
    QVector2D p, q, r;
    QVector3D baryCoord{};

    QVector3D p3, q3, r3;



    for(unsigned int i{};i<get_vertices().size();i+=3)
    {
        //Don't bother looping over the same triangles twice

        if(findTriangle(i, position, baryCoord, p3, q3, r3))
        {
            break;
        }
    }

    //x = u, y = v, z = w
    // f(x,y) = u*f(px, py) + v*f(qx, qy) + w*f(rx, ry)
    return baryCoord.x()*p3.y() + baryCoord.y()*q3.y() + baryCoord.z()*r3.y();
}

int TriangleSurface::getFirstTriangleIndex(const QVector3D &position)
{
    // Finding the height of the position, using barycentric coordinates
    QVector3D baryCoord{};
    QVector3D p3, q3, r3;

    for(unsigned int i{};i<get_vertices().size();i+=3)
    {

        if(findTriangle(i, position, baryCoord, p3, q3, r3))
        {
            return i;
        }
    }

    //x = u, y = v, z = w
    // f(x,y) = u*f(px, py) + v*f(qx, qy) + w*f(rx, ry)
    return -1;
}

QVector3D TriangleSurface::getTriangleNormal(size_t startIndex)
{
    QVector3D a (get_vertices().at(startIndex).getXYZ().x, get_vertices().at(startIndex).getXYZ().y,
                 get_vertices().at(startIndex).getXYZ().z);
    QVector3D b (get_vertices().at(startIndex+1).getXYZ().x, get_vertices().at(startIndex+1).getXYZ().y,
                 get_vertices().at(startIndex+1).getXYZ().z);
    QVector3D c (get_vertices().at(startIndex+2).getXYZ().x, get_vertices().at(startIndex+2).getXYZ().y,
                 get_vertices().at(startIndex+2).getXYZ().z);

    QVector3D v1 = (b)-(a);
    QVector3D w1 = (c)-(a);

    QVector3D normal = QVector3D::crossProduct(w1,v1);
    if(normal.length() < 0.1f)
    {
        normal = QVector3D(0.f, 1.f, 0.f);
    }
    else
    {
        normal.normalize();
    }
    return normal;
}


bool TriangleSurface::findTriangle(unsigned int index, const QVector3D& position, QVector3D &outBaryCoord,
                                   QVector3D &outP, QVector3D &outQ, QVector3D &outR)
{
    // TODO: Make sure this is correct with the up axis being the z
    QVector2D p, q, r;

//    get_vertices().at(index).getXYZ().x

    // first finding the triangle by searching with 2d vector
    // then get the height of all 3 vertices when the triangle is found
    p=QVector2D(get_vertices().at(index).getXYZ().x, get_vertices().at(index).getXYZ().y);
    q=QVector2D(get_vertices().at(index+1).getXYZ().x, get_vertices().at(index+1).getXYZ().y);
    r=QVector2D(get_vertices().at(index+2).getXYZ().x, get_vertices().at(index+2).getXYZ().y);


    outBaryCoord = barycentricCoordinates(QVector2D(position.x(), position.y()), p, q, r);
    if(outBaryCoord.x()>=0 && outBaryCoord.y()>=0 && outBaryCoord.z()>=0)
    {
        outP=QVector3D(get_vertices().at(index).getXYZ().x, get_vertices().at(index).getXYZ().y, get_vertices().at(index).getXYZ().z);
        outQ=QVector3D(get_vertices().at(index+1).getXYZ().x, get_vertices().at(index+1).getXYZ().y, get_vertices().at(index+1).getXYZ().z);
        outR=QVector3D(get_vertices().at(index+2).getXYZ().x, get_vertices().at(index+2).getXYZ().y, get_vertices().at(index+2).getXYZ().z);
        return true;
    }
    return false;
}

QVector3D TriangleSurface::barycentricCoordinates(const QVector2D &position, const QVector2D &p1,
                                                  const QVector2D &p2, const QVector2D &p3)
{
    // TODO: If it doesn't work, try to manually set p1, p2 and p3 to x and z of barycentric coordinates

    QVector2D p12 = p2 - p1;
    QVector2D p13 = p3-p1;
//    qDebug() << position;
    QVector3D n = QVector3D::crossProduct(p12.toVector3D(), p13.toVector3D());
    float areal_123 = n.length(); // dobbelt areal
    QVector3D baryc; // til retur. Husk// u
    QVector2D p = p2 - position;
    QVector2D q = p3 - position;
    n = QVector3D::crossProduct(p.toVector3D(),q.toVector3D());
    baryc.setX(n.z()/areal_123);// v
    p = p3 - position;
    q = p1 - position;

    n = QVector3D::crossProduct(p.toVector3D(),q.toVector3D());
    baryc.setY(n.z()/areal_123);// w
    p = p1 - position;
    q = p2 - position;
    n = QVector3D::crossProduct(p.toVector3D(),q.toVector3D());
    baryc.setZ(n.z()/areal_123); // u?
    return baryc;
}
