#include "las.h"
#include <qdebug.h>

#include <QDebug>
#include "numeric"
#include "vector"

LAS::LAS(std::string fileName)
{
    read(fileName);
    minMaxNormalize();
    triangulate();
}

LAS::~LAS()
{

}

void LAS::read(std::string fileName)
{
    // tar inn data fra txt fil og setter x,y,z vertex
    // til høyest og lavest
    std::ifstream inn;
    inn.open(fileName.c_str());


    if (inn.is_open()) {
        int numberOfVertices;
        inn >> numberOfVertices;
        lasData.reserve(numberOfVertices);
        gsml::Vertex vertex{0,0,0};
        double tempX, tempY, tempZ;
        float x,y,z;
        for (int i=0; i < numberOfVertices; i++)
        {

             inn >> tempX >> tempZ >> tempY;

             x = float(tempX);
             y = float(tempY);
             z = float(tempZ);

             if(i == 0)
             {
                 highestX = x;
                 highestY = y;
                 highestZ = z;
                 lowestX = x;
                 lowestY = y;
                 lowestZ = z;
             }

             if(tempX > highestX){ highestX = x;}
             if(tempY > highestY){ highestY = y;}
             if(tempZ > highestZ){ highestZ = z;}
             if(tempX < lowestX){ lowestX = x;}
             if(tempY < lowestY){ lowestY = y;}
             if(tempZ < lowestZ){ lowestZ = z;}

             vertex.set_xyz(x, y, z);
             lasData.push_back(vertex);
        }
        float deltaX{highestX - lowestX}, deltaY{highestY - lowestY }, deltaZ{highestZ - lowestZ };
        qDebug() << "Delta x : "<<deltaX<< " Delta y: " << deltaY<< " Delta z: "<< deltaZ<< "\n";
        qDebug() << "Biggest x : "<< highestX<< " smallest x: " << lowestX<< "\n";
        qDebug() << "Biggest y : "<<highestY<< " smallest Y: " << lowestY<< "\n";
        qDebug() << "Biggest z : "<<highestZ<< " smallest z: " << lowestZ<< "\n";
        inn.close();
    }
    else
        qDebug() << "file not found..";

}

void LAS::minMaxNormalize()
{
    //normaliserer x,y,z
    for(int i = 0; i < lasData.size(); i++)
    {
        float nX = xMin+(((lasData[i].getXYZ().getX() - lowestX)*(xMax-xMin)) / (highestX - lowestX));
        float nY = yMin+(((lasData[i].getXYZ().getY() - lowestY)*(yMax-yMin)) / (highestY - lowestY));
        float nZ = zMin+(((lasData[i].getXYZ().getZ() - lowestZ)*(zMax-zMin)) / (highestZ - lowestZ));

        lasData[i].set_xyz(nX,nY,nZ);
    }


}

void LAS::triangulate()
{
    int quadX = 0;
    int quadZ = 0;

    //finner hvor mange kvadranter trianglesurf har i hver rettning
    const int quadAmountZ = abs(zMax-zMin)/step;

    std::array<std::vector<float>,800> heights;

    //array for avg høyden
    float averageHeights[800];

    for(int i = 0; i < lasData.size(); i++)
    {
        //kollone
        for(int j = xMin; j < xMax; j+=step)
        {
            if(lasData[i].getXYZ().getX() > j && lasData[i].getXYZ().getX() < j + step)
            {
                quadX = (j-xMin)/step;
            }
        }
        //rad
        for(int j = zMin; j < zMax; j+=step)
        {
            if(lasData[i].getXYZ().getZ() > j && lasData[i].getXYZ().getZ() < j + step)
            {
                quadZ = (j-zMin)/step;
            }
        }

        //indeksen til vectorens array
        int vectorIndex = quadZ*quadAmountZ + quadX;

        //pusher høyden til den vectoren
        heights[vectorIndex].push_back(lasData[i].getXYZ().getY());
    }

    for(int i = 0; i < heights.size(); i++)
    {

        // finner avg av alle høydene og putter den i averageheights
        float sum = 0;
        for(int j = 0; j < heights[i].size(); j++)
        {
            sum += heights[i][j];
        }
        sum = sum/heights[i].size();
        averageHeights[i] = sum;
    }


    //lager overflaten

    for (float x = xMin; x < xMax-step; x+= step)
    {
        for(float z = zMin; z < zMax; z+= step)
        {
            int quadCoordX = (x-xMin)/step;
            int quadCoordZ = (z-zMin)/step;
            float u{(x + abs(xMin)) / (xMax + abs(xMin) + step)};
            float v{(z + abs(zMin)) / (zMax + abs(zMin) + step)};

            mVertices.push_back(gsml::Vertex(x, averageHeights[quadCoordZ*quadAmountZ + quadCoordX], z,
            R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX]*G/255, B/255,u,v));

            mVertices.push_back(gsml::Vertex(x, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX], z+step,
            R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX]*G/255, B/255, u, v+step));

            mVertices.push_back(gsml::Vertex(x+step, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1], z,
            R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step,v));

            mVertices.push_back(gsml::Vertex(x+step, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX+1], z+step,
            R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step, v+step));

            mVertices.push_back(gsml::Vertex(x+step, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1], z,
            R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step,v));

            mVertices.push_back(gsml::Vertex(x, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX], z+step,
            R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX]*G/255, B/255, u, v+step));



        }
    }
}
void LAS::init(GLint matrixUniform)
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


    glBindVertexArray(0);
}

void LAS::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());

}


