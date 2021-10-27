﻿#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(1, -0.75, 0.25);
    mScale.scale(0.25, 0.25, 0.25);
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& triangleVertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();
    //qDebug() << vertices.size();

    mMatrix = mPosition * mScale;


//Find the three vector3d of the closest triangle
    gsml::Vector3d ballPosition3d = mMatrix.getPosition3D();
    //qDebug() << "Ballposition3d:" << ballPosition3d.x << ballPosition3d.y << ballPosition3d.z;;



//Find the distance between the balls position and vertices of the trianglesurface
    gsml::Vector3d distanceBetweenBallAndVert[6]; //using vertices.size() instead of hardcoding 6 would be better.
    for (int i = 0; i < triangleVertices.size(); i++)
    {
        distanceBetweenBallAndVert[i] = triangleVertices[i].getXYZ() - ballPosition3d;

    //qDebug() << "Vert nr                 " << i+1 << "X" << triangleVertices[i].getXYZ().x << "Y" << triangleVertices[i].getXYZ().y << "z" << triangleVertices[i].getXYZ().z;
    //qDebug() << "Distance ball to vert nr" << i+1 << "X" << distanceBetweenBallAndVert[i].x << "Y" << distanceBetweenBallAndVert[i].y << "Z" << distanceBetweenBallAndVert[i].z;
    }



//Keep the three closest points
    gsml::Vector3d closestTrianglePoint[3]; //Used to store the three points of the closest triangle

    closestTrianglePoint[0] =     distanceBetweenBallAndVert[0] + distanceBetweenBallAndVert[1] + distanceBetweenBallAndVert[3];
    gsml::Vector3d comparePoint = distanceBetweenBallAndVert[3] + distanceBetweenBallAndVert[4] + distanceBetweenBallAndVert[5];

    if (closestTrianglePoint[0].x < comparePoint.x && closestTrianglePoint[0].y < comparePoint.y && closestTrianglePoint[0].z < comparePoint.z)
    {
        closestTrianglePoint[0] = triangleVertices[0].getXYZ();
        closestTrianglePoint[1] = triangleVertices[1].getXYZ();
        closestTrianglePoint[2] = triangleVertices[2].getXYZ();
        //qDebug() << "First triangle closest and its 3 corners stored";
    }
        else
    {
        closestTrianglePoint[0] = triangleVertices[3].getXYZ();
        closestTrianglePoint[1] = triangleVertices[4].getXYZ();
        closestTrianglePoint[2] = triangleVertices[5].getXYZ();
        qDebug() << "Second triangle closest and its 3 corners stored";
    }

//    doublechecking the triangle vert variable sent into barycentricCordinates() function;
//    qDebug() << "ballPosition2d" << ballPosition2d.x          << ballPosition2d.y
//             << "Triangle vert 1" << triangleVertices[0].getXYZ().x << triangleVertices[0].getXYZ().y << triangleVertices[0].getXYZ().z
//             << "Triangle vert 2" << triangleVertices[1].getXYZ().x << triangleVertices[1].getXYZ().y << triangleVertices[1].getXYZ().z
//             << "Triangle vert 3" << triangleVertices[2].getXYZ().x << triangleVertices[2].getXYZ().y << triangleVertices[2].getXYZ().z;

//    //doublechecking the closest triangle point variable sent into barycentricCordinates() function;
//    qDebug()<< "ballPosition3d" << ballPosition3d.x << ballPosition3d.y << ballPosition3d.z;
//    qDebug()<< "Closest vert 1" << closestTrianglePoint[0].x << closestTrianglePoint[0].y << closestTrianglePoint[0].z;
//    qDebug()<< "Closest vert 2" << closestTrianglePoint[1].x << closestTrianglePoint[1].y << closestTrianglePoint[1].z;
//    qDebug()<< "Closest vert 3" << closestTrianglePoint[2].x << closestTrianglePoint[2].y << closestTrianglePoint[2].z;



//Getting barycentric cordinates of ball position and closest triangle
    gsml::Vector2d ballPosition2d = mMatrix.getPosition2D(); //Get position of ball in x,y space
    gsml::Vector3d baryCordinates;
    baryCordinates = ballPosition2d.barycentricCoordinates(closestTrianglePoint[0], closestTrianglePoint[1], closestTrianglePoint[2]);
    //qDebug() << "Barycentric cordinates to closest triangle" << baryCordinates.x << baryCordinates.y << baryCordinates.z;



//Calculating the normalvector of the triangle the ball is on
    gsml::Vector3d tempNormal[2];
    gsml::Vector3d triangleNormal;

    tempNormal[0] = closestTrianglePoint[1] - closestTrianglePoint[0];
    tempNormal[1] = closestTrianglePoint[2] - closestTrianglePoint[0];
    triangleNormal.x = tempNormal[0].y * tempNormal[1].z - tempNormal[0].z * tempNormal[1].y;
    triangleNormal.y = tempNormal[0].z * tempNormal[1].x - tempNormal[0].x * tempNormal[1].z;
    triangleNormal.z = tempNormal[0].x * tempNormal[1].y - tempNormal[0].y * tempNormal[1].x;

    triangleNormal.normalize();
    //qDebug() << "Triangle normal is:" << triangleNormal.x << triangleNormal.y << triangleNormal.z;



//Update ball speed across triangle
    gsml::Vector3d BallSpeed;
    if(baryCordinates.x >= 0.0f && baryCordinates.y >= 0.0f && baryCordinates.z >= 0.0f)
    {
        BallSpeed = triangleNormal * dt; //Ballspeed, framerate-dependent beacuse DT is set at 0.017 (in theory 16 1/3ms = 60hz)
        qDebug() << "ball is moving";
        mPosition.translate(BallSpeed.x, BallSpeed.y, BallSpeed.z); //Based on calculations in either collision or free-fall apply translation to ball.
    }
}

void RollingBall::init(GLint matrixUniform)
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

   glBindBuffer(GL_ARRAY_BUFFER, mVBO);
   glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(gsml::Vertex), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
   glEnableVertexAttribArray(1);

   glBindVertexArray(0);
}

void RollingBall::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
}

//gsml::Vector3d RollingBall::getClosestTriangleToBall(std::vector<gsml::Vertex> triangleVertices)
//{
//    gsml::Vector3d ballPosition = mMatrix.getPosition3D();
////    qDebug() << ballPosition.x << ballPosition.y << ballPosition.z;;

//    gsml::Vector3d closestTrianglePoint;
//    gsml::Vector3d distanceBetweenBallAndVert[6]; //using vertices.size() instead of hardcoding 6 would be better. On to more difficult things.

//    //Run through the vertices of the trianglesurface
//    for (int i = 0; i < triangleVertices.size(); i++)
//    {
//        qDebug() << "Vert nr" << i+1 << triangleVertices[i].getXYZ().x << triangleVertices[i].getXYZ().y << triangleVertices[i].getXYZ().z;

//        //Find distance between the balls position and the vertices of the ground-triangles
//        distanceBetweenBallAndVert[i] = ballPosition - gsml::Vector3d (triangleVertices[i].getXYZ());

//        qDebug() << "Distance ball to vert nr" << i+1 << "X" << distanceBetweenBallAndVert[i].x << "Y" << distanceBetweenBallAndVert[i].y << "Z" << distanceBetweenBallAndVert[i].z;
//    }

////    for (int i = 0; i < vertices.size()/2; i++)
////    {
//        //Keep the three closest points
//    closestTrianglePoint = distanceBetweenBallAndVert[0] + distanceBetweenBallAndVert[1] + distanceBetweenBallAndVert[2];
//    gsml::Vector3d comparePoint = distanceBetweenBallAndVert[3] + distanceBetweenBallAndVert[4] + distanceBetweenBallAndVert[5];

//        if (closestTrianglePoint.x <= comparePoint.x)
//        {
//            closestTrianglePoint = triangleVertices[0].getXYZ().x, triangleVertices[0].getXYZ().y, triangleVertices[0].getXYZ().z;
//            qDebug() << "First triangle returned";
//        }
//            else
//        {
//            closestTrianglePoint = triangleVertices[0+3].getXYZ().x, triangleVertices[1+3].getXYZ().y, triangleVertices[2+3].getXYZ().z;
//            qDebug() << "Second triangle returned";
//        }
////    }
//return closestTrianglePoint;
//}

