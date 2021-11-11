#include <QDebug>
#include <QImage>
#include <cstdio>
#include "trianglesurface.h"
#include "vertex.h"

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
    //readFile(filnavn);
    mMatrix.setToIdentity();
    //mMatrix.translate(0,0,5);

    readPoints(filnavn);
    qDebug() << xMin << xMax << yMin << yMax;
    MapMin.x = 614580; MapMax.x = 615580;
    MapMin.y = 6757290; MapMax.y = 6758760;

    makePlain();

    float tempX = MapMin.x + MapMax.x;
    tempX = tempX/2;
    float tempY = MapMin.y + MapMax.y;
    tempY = tempY/2;
    mMatrix.setPosition(-tempX, -tempY, 0);
    //move(-tempX, -tempY, 0);
}

TriangleSurface::~TriangleSurface()
{
    //qDebug() << "TriangleSurface::~TriangleSurface()";
    //delete [] m_vertices;
    //qDebug() << "TriangleSurface::~TriangleSurface() - vertices deleted";
}

void TriangleSurface::readPoints(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open())
    {
        int n;
        gsml::Vector3d temp;
        inn >> n;
        for (int i=0; i<n; i++)
        {
            inn >> temp.x;
            inn >> temp.y;
            inn >> temp.z;
            points.push_back(temp);
        }
        inn.close();
    }

    for(int i{0}; i<(int)points.size(); i++){
        if(points[i].x < xMin)
            xMin = points[i].x;
        if(points[i].x > xMax)
            xMax = points[i].x;
        if(points[i].y < yMin)
            yMin = points[i].y;
        if(points[i].y > yMax)
            yMax = points[i].y;
    }
}

void TriangleSurface::makePlain()
{
    float z = 0;
    for(auto x = MapMin.x; x<MapMax.x; x+=n)
        for(auto y = MapMin.y; y<MapMax.y; y+=n)
        {
            mVertices.push_back(gsml::Vertex{  x,   y, z,   1, 0, 0});
            mVertices.push_back(gsml::Vertex{x+n,   y, z,   1, 0, 0});
            mVertices.push_back(gsml::Vertex{  x, y+n, z,   1, 0, 0});
            mVertices.push_back(gsml::Vertex{  x, y+n, z,   1, 0, 0});
            mVertices.push_back(gsml::Vertex{x+n,   y, z,   1, 0, 0});
            mVertices.push_back(gsml::Vertex{x+n, y+n, z,   1, 0, 0});
        }
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

