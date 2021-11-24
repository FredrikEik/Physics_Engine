#include "las.h"
#include <qdebug.h>

#include <QDebug>
#include "numeric"
#include "vector"

LAS::LAS(std::string fileName)
{
    readLAS(fileName);
    minMaxNormalize();
    triangulate();
}

LAS::~LAS()
{

}

void LAS::readLAS(std::string fileName)
{
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
        //print las data
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

    // Using min, max and step to find how many quads the TriangleSurface has in each direction
    const int quadAmountZ = abs(zMax-zMin)/step;
    //const int numOfQuads = quadAmountX*quadAmountZ; //qDebug() << numOfQuads;

    // An array of vectors that hold all the heights for each quad
    // Size of array is hardcoded, unsure how to do it from numOfQuads
    std::array<std::vector<float>,800> heights;
    //std::array<std::vector<float>,numOfQuads> heights;

    //An array that holds the average height of each quad
    float averageHeights[800];

    //For every vertices
    //qDebug() << "Vertices size" << lasData.size();
    for(int i = 0; i < lasData.size(); i++)
    {
        //column
        for(int j = xMin; j < xMax; j+=step)
        {
            if(lasData[i].getXYZ().getX() > j && lasData[i].getXYZ().getX() < j + step)
            {
                quadX = (j-xMin)/step;
            }
        }
        //row
        for(int j = zMin; j < zMax; j+=step)
        {
            if(lasData[i].getXYZ().getZ() > j && lasData[i].getXYZ().getZ() < j + step)
            {
                quadZ = (j-zMin)/step;
            }
        }

        //index to vector array
        int vectorIndex = quadZ*quadAmountZ + quadX;

        //Pushes the height(y) to the correct vector in the array
        heights[vectorIndex].push_back(lasData[i].getXYZ().getY());
    }

    for(int i = 0; i < heights.size(); i++)
    {
        //qDebug() << "size of height vector " << i << heights[i].size();

        //Calculate the average of all heights in quad
        //.. and put it in the array of averageHeights


        float sum = 0;
        for(int j = 0; j < heights[i].size(); j++)
        {
            sum += heights[i][j];
        }
        sum = sum/heights[i].size();
        averageHeights[i] = sum;
    }


    //Create triangulated surface

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

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void LAS::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());

}












/*
LAS::LAS(std::string fileName)
{
    ReadDatafromFile(fileName);
    GenerateHeightMap();
    mMatrix.setToIdentity();
}

void LAS::ReadDatafromFile(std::string fileName)
{

    std::string line;
    std::ifstream myfile (fileName);


    std::string x,y,z;
    if (myfile.is_open())
    {
        qDebug() << "file read.";
        getline (myfile,line);
        while ( getline (myfile,line) )
        {
            //getting x positions
            x += line[0];
            x += line[1];
            x += line[2];
            x += line[3];
            x += line[4];
            x += line[5];
            x += line[6];
            x += line[7];
            x += line[8];

            //getting y positions
            y += line[10];
            y += line[11];
            y += line[12];
            y += line[13];
            y += line[14];
            y += line[15];
            y += line[16];
            y += line[17];
            y += line[18];
            y += line[19];
            //getting z positions
            z += line[21];
            z += line[22];
            z += line[23];
            z += line[24];
            z += line[25];
            z += line[26];

            //std::cout <<x<< " "<< y << " "<<z<< '\n';
            positions.push_back(gsml::Vertex(std::stof(x),std::stof(z),std::stof(y), (line[4] - '0')*0.1f ,(line[23]- '0')*0.1f ,(line[16]- '0')*0.1f ,0 ,0)); // setting data into Vertex
            x = "";
            y = "";
            z = "";

        }
        myfile.close();
        //delete delta in the vertex, as it starts with insaneley big values :D
        RemoveDeltaPos();
    }
    else
    {
        QString string = QString::fromStdString( fileName);
        qDebug() << "Unable to open file: " << string;
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

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}


std::vector<gsml::Vertex> LAS::getPositions() const
{
    return positions;
}
std::vector<gsml::Vertex> LAS::getmVertices() const
{
    return mVertices;
}

void LAS::setPositions(const std::vector<gsml::Vertex> &value)
{
    positions = value;
}

void LAS::RemoveDeltaPos()
{
    float smallestX{0}, biggestX{0};
    float smallestY{0}, biggestY{0};
    float smallestZ{0}, biggestZ{0};

    //we take biggest delta between x,y and z, then we remove it from the vertexes to get a more appropreate number.
    for(auto i = 0; i<positions.size(); i++)
    {

        //gsml::Vector3d temp = positions[i].getVertex();
        float tempX = positions[i].getX();
        float tempY = positions[i].getY();
        float tempZ = positions[i].getZ();
        //qDebug() << "xyz "<< temp;
        //check size of x
        if(tempX < smallestX)
            smallestX = tempX;
        if(tempX > biggestX)
            biggestX = tempX;
        //check size of y
        if(tempY < smallestY)
            smallestY = tempY;
        if(tempY > biggestY)
            biggestY = tempY;
        //check size of Z
        if(tempZ < smallestZ)
            smallestZ = tempZ;
        if(tempZ > biggestZ)
            biggestZ = tempZ;
    }

    for(auto i = 0; i<positions.size(); i++)
    {

        gsml::Vector3d temp = positions[i].getXYZ();
        //qDebug() << "xyz "<< temp;
        //check size of x
        if(temp.getX() < smallestX)
            smallestX = temp.getX();
        if(temp.getX() > biggestX)
            biggestX = temp.getX();
        //check size of y
        if(temp.getY() < smallestY)
            smallestY = temp.getY();
        if(temp.getY() > biggestY)
            biggestY = temp.getY();
        //check size of Z
        if(temp.getZ() < smallestZ)
            smallestZ = temp.getZ();
        if(temp.getZ() > biggestZ)
            biggestZ = temp.getZ();
    }

    float deltaX{biggestX - smallestX}, deltaY{biggestY - smallestY }, deltaZ{biggestZ - smallestZ };
    qDebug() << "Delta x : "<<deltaX<< " Delta y: " << deltaY<< " Delta z: "<< deltaZ<< "\n";
    qDebug() << "Biggest x : "<< biggestX<< " smallest x: " << smallestX<< "\n";
    qDebug() << "Biggest y : "<<biggestY<< " smallest Y: " << smallestY<< "\n";
    qDebug() << "Biggest z : "<<biggestZ<< " smallest z: " << smallestZ<< "\n";


    for(auto i = 0; i<positions.size(); i++)
    {
        positions[i].set_xyz(deltaX - positions[i].getXYZ().getX(),
                             deltaY - positions[i].getXYZ().getY() ,
                             deltaZ - positions[i].getXYZ().getZ());
        //qDebug() << positions[i].getVertex();//.getX() << positions[i].getVertex().getY() << positions[i].getVertex().getZ() ;
    }

    float deltaX{biggestX - smallestX}, deltaY{biggestY - smallestY }, deltaZ{biggestZ - smallestZ };
    qDebug() << "Delta x : "<<deltaX<< " Delta y: " << deltaY<< " Delta z: "<< deltaZ<< "\n";
    qDebug() << "Biggest x : "<< biggestX<< " smallest x: " << smallestX<< "\n";
    qDebug() << "Biggest y : "<<biggestY<< " smallest Y: " << smallestY<< "\n";
    qDebug() << "Biggest z : "<<biggestZ<< " smallest z: " << smallestZ<< "\n";


    float smallestX1{0}, biggestX1{0};
    float smallestY1{0}, biggestY1{0};
    float smallestZ1{0}, biggestZ1{0};
    for(auto i = 0; i<positions.size(); i++)
    {
        float tempX = positions[i].getX();
        float tempY = positions[i].getY();
        float tempZ = positions[i].getZ();

        positions[i].set_xyz(deltaX - tempX
        , deltaY - tempY
        , deltaZ - tempZ);
        //qDebug() << positions[i].getVertex();//.getX() << positions[i].getVertex().getY() << positions[i].getVertex().getZ() ;
    }

}

void LAS::GenerateHeightMap()
{
    gsml::Vertex v{};

    float ofsetx = -100;
    float ofsetz = -100;
    float ofsety = -15;

    for(float x = 100; x<200; x+=1)
        for(float z =100; z<200; z+=1)
        {
            //get all height data :D
            float height1 = CalcHeight(    x,    z);
            float height2 = CalcHeight(  x+1,    z);
            float height3 = CalcHeight(    x,  z+1);
            float height4 = CalcHeight(    x,  z+1);
            float height5 = CalcHeight(  x+1,    z);
            float height6 = CalcHeight(  x+1,  z+1);

                                                                                        //use height date for colouring   //This order is like this because our
            mVertices.push_back(gsml::Vertex{ofsetx +  x, ofsety +height1,ofsetz +   z,       x/900, height1/100, z/1000,0,0}); //1
            mVertices.push_back(gsml::Vertex{ofsetx +x+1, ofsety +height2,ofsetz +   z,       x/900, height2/100, z/1000,0,0}); //2
            mVertices.push_back(gsml::Vertex{ofsetx +  x, ofsety +height3,ofsetz + z+1,       x/900, height3/100, z/1000,0,0}); //3
            mVertices.push_back(gsml::Vertex{ofsetx +  x, ofsety +height4,ofsetz + z+1,       x/900, height4/100, z/1000,0,0}); //4
            mVertices.push_back(gsml::Vertex{ofsetx +x+1, ofsety +height5,ofsetz +   z,       x/900, height5/100, z/1000,0,0}); //5
            mVertices.push_back(gsml::Vertex{ofsetx +x+1, ofsety +height6,ofsetz + z+1,       x/900, height6/100, z/1000,0,0}); //6
        }
}


float LAS::CalcHeight(float x, float z)
{
    float height = 10.0f;


    int X = static_cast<int>(x);
    int Z = static_cast<int>(z);
    int counter =0;
    float collected = 0;
    height = PosArr[X][Z];

    if((X>15 && Z>15) && height <= 2.0f
    {

        for(int i = -14; i < 14 ; i++)
        {
            for (int j = -14 ; j <14; j++ )
            {
                if(PosArr[X + i][Z + j] != 0)
                {
                    collected += PosArr[X + i][Z + j];
                    counter++;
                }
            }
        }
        height = ( collected)/(counter);
        //average height of surrounding area
    }
    else
    {
        height = PosArr[X][Z] ;
    }


    //do the average calc
    //qDebug() << height;
    //then return its
    return height;
}
void LAS::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());

}
*/

