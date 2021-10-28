#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(0, 0, 0.25);
    mScale.scale(0.25, 0.25, 0.25);
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& triangleVertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

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

    //qDebug() << "Triangle normal is:" << triangleNormal.x << triangleNormal.y << triangleNormal.z;
    triangleNormal.normalize();
    //qDebug() << "Normalized Triangle normal is:" << triangleNormal.x << triangleNormal.y << triangleNormal.z;


qDebug() << "BaryCordinates before if" << baryCordinates.x << baryCordinates.y << baryCordinates.z;

//Update ball speed across triangle
    if(baryCordinates.x >= 0.0f && baryCordinates.y >= 0.0f && baryCordinates.z >= 0.0f)
    {
        gsml::Vector3d acceleration = (gravity * 0.001f) ^ triangleNormal ^ gsml::Vector3d(0, 0, triangleNormal.z);

        //ballSpeed = triangleNormal * dt; //Ballspeed, framerate-dependent beacuse DT is set at 0.017 (in theory 16 1/3ms = 60hz)

        velocity = velocity + (acceleration * 0.17);

        gsml::Vector3d newBallPosition = mMatrix.getPosition3D() + velocity;

//        newBallPosition.z = closestTrianglePoint[0].z * baryCordinates.x +
//                            closestTrianglePoint[0].z * baryCordinates.y +
//                            closestTrianglePoint[0].z * baryCordinates.z;

        newBallPosition.z = triangleNormal.z * baryCordinates.x +
                            triangleNormal.z * baryCordinates.y +
                            triangleNormal.z * baryCordinates.z;

        mPosition.setPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z); //Based on calculations in either collision or free-fall apply translation to ball.
        qDebug() << "ball is moving";
    }
}


//gsml::Vec3 acceleration = (gravityForce * 0.001f) ^ planeNormal ^ gsml::Vec3(0,0,planeNormal.z);

//velocity = velocity + (acceleration * 0.17);

//gsml::Vec3 newPos = ballPos + velocity;
//newPos.z = (baryCoords.x * p1.z + baryCoords.y * p2.z + baryCoords.z * p3.z) + offset;
//mPosition.setPosition(newPos.x,newPos.y,newPos.z);

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

