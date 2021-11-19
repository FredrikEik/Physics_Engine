#include <QDebug>
#include <QImage>
#include <cstdio>
#include <iostream>
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

    int n = 0;
    int linesRead = 0;
    //    int j=0;
    //    int l=0;
    //    int min_index = 0;
    float x = 0;
    float y = 0;
    float z = 0;
    //    float a[245592];
    //    float b[245592];
    //    float c[245592];

    if (inn.is_open())
    {
        inn >> n;
        inn >> x >> y >> z;
        xMaximum = x;
        xMinimum = x;
        yMaximum = y;
        yMinimum = y;
        zMaximum = z;
        zMinimum = z;
        //        inn >> a[0] >> b[0] >> c[0];
        //        xMaximum = a[0];
        //        xMinimum = a[0];
        //        yMaximum = b[0];
        //        yMinimum = b[0];
        //        zMaximum = c[0];
        //        zMinimum = c[0];
        mVertices.reserve(n);
        //vertex.set_xyz(x,y,z);
        //mVertices.push_back(vertex);
        // vertex.set_xyz(a[0],b[0],c[0]);
        //  mVertices.push_back(vertex);
        for (int i=0; i<n; i++)
        {
            inn >> x >> y >> z;
            //inn >> a[i] >> b[i] >> c[i];
            linesRead++;
            //qDebug() << x << y << z;
            vertex.set_xyz(x,y,z);
            //vertex.set_xyz(a[i],b[i],c[i]);
            mVertices.push_back(vertex);
            if (x > xMaximum)
                xMaximum = x;
            else if(x < xMinimum)
                xMinimum = x;
            if (y > yMaximum)
                yMaximum = y;
            else if(y < yMinimum)
                yMinimum = y;
            if(z > zMaximum)
                zMaximum = z;
            else if(z < zMinimum)
                zMinimum = z;
            //             if (x > xMaximum)
            //                 xMaximum = a[i];
            //             else if(x < xMinimum)
            //                 xMinimum = a[i];
            //             if (y > yMaximum)
            //                 yMaximum = b[i];
            //             else if(y < yMinimum)
            //                 yMinimum = b[i];
            //             if(z > zMaximum)
            //                 zMaximum = c[i];
            //             else if(z < zMinimum)
            //                 zMinimum = c[i];


        }
        inn.close();
    }

        //minMaxScale();


    int squarecounter=0;
    float step = 10;
    float xOffset = (xMaximum-xMinimum)/step;
    float yOffset = (yMaximum-yMinimum)/step;
    double squareMinY = yMinimum;
    double squareMaxY = squareMinY+yOffset;
    double squareMinX = xMinimum;
    double squareMaxX = squareMinX +xOffset;

    for (double j=squareMinY; j<=yMaximum-yOffset; j+=yOffset){
        //qDebug() << "hei";

        for (double i =squareMinX; i<= xMaximum-xOffset; i+=xOffset){
            numberofPointsInside=0;
            float tempX=0;
            float tempY=0;
            float tempZ=0;
            squarecounter++;
            for ( int k = 0 ; k<mVertices.size(); k++){
                if ( mVertices[k].getXYZ().x < squareMaxX &&
                     mVertices[k].getXYZ().x > i &&
                     mVertices[k].getXYZ().y < squareMaxY &&
                     mVertices[k].getXYZ().y > j){
                    ++numberofPointsInside;

                    tempX += mVertices[k].getXYZ().x;
                    tempY += mVertices[k].getXYZ().y;
                    tempZ += mVertices[k].getXYZ().z;

                }

            }
            //qDebug () << numberofPointsInside;
            if(numberofPointsInside <= 0){
                vertex2.set_xyz(squareMinX + xOffset/2, squareMinY + yOffset/2, zMinimum+(zMaximum-zMinimum)/2);
            }
            else if(numberofPointsInside > 0)  {
                tempX = tempX/numberofPointsInside;
                tempY = tempY/numberofPointsInside;
                tempZ = tempZ/numberofPointsInside;

                vertex2.set_xyz(tempX, tempY, tempZ);
            }

            //qDebug() << squarecounter << xOffset << yOffset;
             qDebug () << numberofPointsInside;
            //qDebug() << vertex2.getXYZ().x << vertex2.getXYZ().y << vertex2.getXYZ().z;

            Vertices.push_back(vertex2);


           // squareMinX = squareMinX +xOffset;
            //squareMaxX = squareMaxX +xOffset;
        }
       // squareMinY = squareMinY +yOffset;
       // squareMaxY = squareMaxY +yOffset;

    }


    //qDebug() << Vertices.size();


    //    for (l = 0; l < n-1; l++)
    //    {
    //        min_index = l;
    //        for (j=l+1 ; j<n; j++)
    //        {
    //            if (a[j] < a[min_index]){
    //             min_index = j;
    //            }
    //        }

    //        vertex.swap(&mVertices[min_index], &mVertices[l]);
    //        for(int i=0; i<n; i++)
    //            qDebug () << mVertices[i].getXYZ().x;
    //    }

    //qDebug() << xMinimum << xMaximum << yMinimum << yMaximum << zMinimum << zMaximum;
    // qDebug() << linesRead;

    //    if (inn.is_open())
    //    {
    //        long int n;
    //        gsml::Vertex vertex;
    //        inn >> n;
    //        inn >> vertex;
    //        xmin = vertex.getXYZ().x;
    //        xmax = vertex.getXYZ().x;
    //        ymin = vertex.getXYZ().y;
    //        ymax = vertex.getXYZ().y;
    //        mVertices.reserve(n);
    //        for (int i=0; i<n; i++)
    //        {
    //             inn >> vertex;
    //             if (xmin > vertex.getXYZ().x)
    //                 xmin = vertex.getXYZ().x;
    //             if (xmax < vertex.getXYZ().x)
    //                 xmax = vertex.getXYZ().x;
    //             if(ymin > vertex.getXYZ().y)
    //                 ymin = vertex.getXYZ().y;
    //             if(ymax < vertex.getXYZ().y)
    //                 ymax = vertex.getXYZ().y;

    //             mVertices.push_back(vertex);



    //        }
    //        inn.close();
    //    }
    //    qDebug() << xmin << xmax << ymin << ymax;
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

void TriangleSurface::minMaxScale()
{
    for(int index=0; index<=mVertices.size(); index++){
        mVertices[index].set_xyz((mVertices[index].getXYZ().x-xMinimum)*(valueMax-valueMin)/(xMaximum-xMinimum),
                                 (mVertices[index].getXYZ().y-yMinimum)*(valueMax-valueMin)/(yMaximum-yMinimum),
                                 mVertices[index].getXYZ().z);

    }
}

