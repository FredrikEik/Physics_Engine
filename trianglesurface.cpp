#include <QDebug>
#include <QImage>
#include <cstdio>
#include <iostream>
#include <sstream>
#include "trianglesurface.h"
#include "vertex.h"
#include <QDebug>
#include <string>

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
    readFile2(filnavn);
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

void TriangleSurface::readFile2(std::string filename)
{
    std::vector<gsml::Vector3d> tempPos;
    std::vector<gsml::Vector3d> sortedPos;

    std::ifstream inn;
    inn.open(filename.c_str());

    if (inn.is_open())
    {
        int n; //antall punkter?
        inn >> n;

        std::string coord = "";

        float x = 0.0f, y = 0.0f, z = 0.0f;
        float xMin = 0.0f, xMax = 0.0f, yMin = 0.0f, yMax = 0.0f;

        const int rows = 100;
        const int cols = 100;

        mVertices.reserve(n);
        tempPos.reserve(n);

        for (int i=0; i<n; i++)
        {
            inn >> x >> y >> z;

            tempPos.push_back(gsml::Vector3d{x, y, z});

//if i == 0, xMin and yMin is 0. result would be xMin and yMin == 0 at the end of loop
            if(x < xMin || i == 0)
                xMin = x;
            if(y < yMin || i == 0)
                yMin = y;
            if(x > xMax || i == 0)
                xMax = x;
            if(y > yMax || i == 0)
                yMax = y;
        }
        float xStep = (xMax-xMin)/rows; //avstanden mellom hvert hjørne i et kvadrant (i x-planet)
        float yStep = (yMax-yMin)/cols; //avstanden mellom hvert hjørne i et kvadrant (i y-planet)
        int vertexesInQuad[rows][cols]; //antall vertexer i kvadrant [x][y]
        float tempForAvg[rows][cols];   //holder summen av z verdiene i kvadrant [x][y]
        for (int y=0; y<cols; y++){
            for (int x=0; x<rows; x++) {
                tempForAvg[x][y]=0.0f;
                vertexesInQuad[x][y]=0;
             }
        }
        for (int i = 0; i < tempPos.size(); i++)
        {
            int x = (int)((tempPos[i].x-xMin) / xStep);
            int y = (int)((tempPos[i].y-yMin) / yStep);
            tempForAvg[x][y] += tempPos[i].z;
            vertexesInQuad[x][y]++;
        }
        for (int y=0; y<cols; y++)
        {
            for (int x=0; x<rows; x++)
            {
                if(tempForAvg[x][y] != 0)
                    sortedPos.push_back(gsml::Vector3d{ x*xStep + (xStep / 2), y*yStep + (yStep / 2), tempForAvg[x][y] / vertexesInQuad[x][y]});
            }
        }

        int c = 0;
        for(int i = 0; i<sortedPos.size()-rows-1; i++)
        {
            mVertices.push_back(gsml::Vertex{sortedPos[i].x, sortedPos[i].y, sortedPos[i].z, 0,0.5,0});
            mVertices.push_back(gsml::Vertex{sortedPos[i+1].x, sortedPos[i+1].y, sortedPos[i+1].z, 0,0.5,0});
            mVertices.push_back(gsml::Vertex{sortedPos[i+rows].x, sortedPos[i+rows].y, sortedPos[i+rows].z, 0,0.5,0});

            mVertices.push_back(gsml::Vertex{sortedPos[i+rows].x, sortedPos[i+rows].y, sortedPos[i+rows].z, 0,0.5,0});
            mVertices.push_back(gsml::Vertex{sortedPos[i+1].x, sortedPos[i+1].y, sortedPos[i+1].z, 0,0.5,0});
            mVertices.push_back(gsml::Vertex{sortedPos[i+rows+1].x, sortedPos[i+rows+1].y, sortedPos[i+rows+1].z, 0,0.5,0});
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

