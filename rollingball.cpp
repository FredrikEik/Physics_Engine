#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(0,0,0.25);
    mScale.scale(0.25,0.25,0.25);
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    gsml::Vector3d BallSpeed;
    std::vector<gsml::Vertex>& triangleVertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();
//    qDebug() << vertices.size();

    mMatrix = mPosition * mScale;

    //Find the three vector3d of the closest triangle
    gsml::Vector3d ballPosition3d = mMatrix.getPosition3D();
//    qDebug() << "Ballposition3d:" << ballPosition3d.x << ballPosition3d.y << ballPosition3d.z;;

    gsml::Vector3d distanceBetweenBallAndVert[6]; //using vertices.size() instead of hardcoding 6 would be better.

    //Run through the vertices of the trianglesurface
    for (int i = 0; i < triangleVertices.size(); i++)
    {
        //Find distance between the balls position and the vertices of the ground-triangles
        distanceBetweenBallAndVert[i] = gsml::Vector3d (triangleVertices[i].getXYZ()) - ballPosition3d;

//        qDebug() << "Vert nr                 " << i+1 << "X" << triangleVertices[i].getXYZ().x << "Y" << triangleVertices[i].getXYZ().y << "z" << triangleVertices[i].getXYZ().z;
//        qDebug() << "Distance ball to vert nr" << i+1 << "X" << distanceBetweenBallAndVert[i].x << "Y" << distanceBetweenBallAndVert[i].y << "Z" << distanceBetweenBallAndVert[i].z;
    }

    gsml::Vector3d closestTrianglePoint[3]; //Used to store the three points of the closest triangle

//    //Keep the three closest points
        closestTrianglePoint[0] =     distanceBetweenBallAndVert[0] + distanceBetweenBallAndVert[1] + distanceBetweenBallAndVert[3];
        gsml::Vector3d comparePoint = distanceBetweenBallAndVert[3] + distanceBetweenBallAndVert[4] + distanceBetweenBallAndVert[5];

        if (closestTrianglePoint[0].x < comparePoint.x)
        {
            closestTrianglePoint[0] = (triangleVertices[0].getXYZ().x, triangleVertices[0].getXYZ().y, triangleVertices[0].getXYZ().z);
            closestTrianglePoint[1] = (triangleVertices[1].getXYZ().x, triangleVertices[1].getXYZ().y, triangleVertices[1].getXYZ().z);
            closestTrianglePoint[2] = (triangleVertices[2].getXYZ().x, triangleVertices[2].getXYZ().x, triangleVertices[2].getXYZ().z);
//            qDebug() << "First triangle closest and their 3 corners stored";
        }
            else
        {
            closestTrianglePoint[0] = (triangleVertices[3].getXYZ().x, triangleVertices[3].getXYZ().y, triangleVertices[3].getXYZ().z);
            closestTrianglePoint[1] = (triangleVertices[4].getXYZ().x, triangleVertices[4].getXYZ().y, triangleVertices[4].getXYZ().z);
            closestTrianglePoint[2] = (triangleVertices[5].getXYZ().x, triangleVertices[5].getXYZ().y, triangleVertices[5].getXYZ().z);
            qDebug() << "Second triangle closest and their 3 corners stored";
        }

      //get barycentric cordinates from ball to triangle
      gsml::Vector2d ballPosition2d = mMatrix.getPosition2D(); //Get position of ball in x,y space
//      qDebug() << ballPosition2d.x << ballPosition2d.y;

      gsml::Vector3d baryCordinates;
      baryCordinates = ballPosition2d.barycentricCoordinates(closestTrianglePoint[0], closestTrianglePoint[1], closestTrianglePoint[2]);
      qDebug() << "Barycentric cordinates to closest triangle" << baryCordinates.x << baryCordinates.y << baryCordinates.z;
//    if(baryCordinates >= (0.0f, 0.0f, 0.0f) && baryCordinates <= (1.0f, 1.0f, 1.0f))
//    {

//    }

    //h = v0t+1/2 gt^2 - formula for "hastighet" given freefall. Simplified in code.
    //BallSpeed =+ -9.2*dt, 0*dt, 0*dt; //Accumulative ballspeed, framerate-independendt using tickrate. Dag set dt at a 60-ish hz rate at 0.017
//    qDebug << "ball is moving";

mPosition.translate(BallSpeed.x, BallSpeed.y, BallSpeed.z); //Based on calculations in either collision or free-fall apply translation to ball.
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

//gsml::Vector3d RollingBall::getBarysentricCordinates(gsml::Vector3d closestTriangleToPoint)
//{
//    //https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
//    // Compute barycentric coordinates (u, v, w) for
//    // point p with respect to triangle (a, b, c)

//    gsml::Vector2d ballPosition; //Ignore height, position of ball
//    float a, b, c; //Position of points of the triangle
//    float u, v, w; //Returned barysentric cordinates

//    ballPosition = mMatrix.getPosition2D(); //Get the current position of the ball
//    //qDebug() << "x: " << ballPosition.x << "y: " << ballPosition.y;
//    a = closestTriangleToPoint.x;
//    b = closestTriangleToPoint.y;
//    c = closestTriangleToPoint.z;

//    //Calculate Barycentric cordiantes
//    gsml::Vector2d v0 = b - a, v1 = c - a, v2 = ballPosition - a;
//    float d00 = Dot(v0, v0);
//    float d01 = Dot(v0, v1);
//    float d11 = Dot(v1, v1);
//    float d20 = Dot(v2, v0);
//    float d21 = Dot(v2, v1);
//    float denom = d00 * d11 - d01 * d01;
//    v = (d11 * d20 - d01 * d21) / denom;
//    w = (d00 * d21 - d01 * d20) / denom;
//    u = 1.0f - v - w;

//    gsml::Vector3d BaricentricCordinates = {u, v, w};
//    return BaricentricCordinates;
//}
