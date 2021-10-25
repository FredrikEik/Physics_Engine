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
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    mMatrix = mPosition * mScale;

    gsml::Vector3d closestTrianglePoint1, closestTrianglePoint2, closestTrianglePoint3 = getClosestTriangleToBall(vertices);
    qDebug() << "First triangle" << closestTrianglePoint1.x << closestTrianglePoint1.y << closestTrianglePoint1.z;
    qDebug() << "Second triangle" << closestTrianglePoint2.x << closestTrianglePoint2.y << closestTrianglePoint2.z;
    //qDebug() << "Third triangle" << closestTrianglePoint3.x << closestTrianglePoint3.y << closestTrianglePoint3.z;

    gsml::Vector3d baryCordinates;
//    baryCordinates = barycentricCoordinates(closestTriangleVector3, corner2, corner3);


    //h = v0t+1/2 gt^2 - formula for "hastighet" given freefall. Simplified in code.
    BallSpeed =+ -9.2*dt, 0*dt, 0*dt; //Accumulative ballspeed, framerate-independendt using tickrate. Dag set dt at a 60-ish hz rate at 0.017
//    std::cout << "ball is moving" << std::endl;

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

gsml::Vector3d RollingBall::getClosestTriangleToBall(std::vector<gsml::Vertex> vertices)
{
    gsml::Vector3d ballPosition = mMatrix.getPosition3D();
//    qDebug() << ballPosition.x << ballPosition.y << ballPosition.z;

    gsml::Vector3d closestTrianglePoint1, closestTrianglePoint2, closestTrianglePoint3;
    gsml::Vector3d distance[6]; //using vertices.size() instead of hardcoding 6 would be better. On to more difficult things.

    //Run through the vertices
    for (int i = 0; i < vertices.size(); i += 3)
    {
        //Find distance between the balls position and the vertices of the ground-triangles
        distance[ i ] = ballPosition - gsml::Vector3d (vertices[ i ].getXYZ());
        distance[i+1] = ballPosition - gsml::Vector3d (vertices[i+1].getXYZ());
        distance[i+2] = ballPosition - gsml::Vector3d (vertices[i+2].getXYZ());
//        qDebug() << i << "X-" << distance[i].x << i+1 << "Y-" << distance[i+1].y << i+2 << "Z-" << distance[i+2].z;
    }

    //Keep the three closest points
    if (distance[0].x + distance[1].y + distance[2].z > distance[3].x + distance[4].y + distance[5].z)
    {
        closestTrianglePoint1 = vertices[0].getXYZ().x, vertices[0].getXYZ().y, vertices[0].getXYZ().z;
        closestTrianglePoint2 = vertices[1].getXYZ().x, vertices[1].getXYZ().y, vertices[1].getXYZ().z;
        closestTrianglePoint3 = vertices[2].getXYZ().x, vertices[2].getXYZ().y, vertices[2].getXYZ().z;
        //qDebug() << "First triangle";
        return closestTrianglePoint1, closestTrianglePoint2, closestTrianglePoint3;
    }
        else
    {
        closestTrianglePoint1 = vertices[3].getXYZ().x, vertices[4].getXYZ().y, vertices[5].getXYZ().z;
        closestTrianglePoint2 = vertices[1].getXYZ().x, vertices[1].getXYZ().y, vertices[1].getXYZ().z;
        closestTrianglePoint3 = vertices[2].getXYZ().x, vertices[2].getXYZ().y, vertices[2].getXYZ().z;
        //qDebug() << "Second triangle";
        return closestTrianglePoint1, closestTrianglePoint2, closestTrianglePoint3;
    }
}

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
