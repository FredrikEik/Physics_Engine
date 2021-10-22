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

//    if(mMatrix = vertices) //Trying to check if overlap.
//    {
//        std::cout << "contact" << std::endl;
//    }
//    else
//    {
      //h = v0t+1/2 gt^2 - formula for "hastighet" given freefall. Simplified in code.
      BallSpeed =+ -9.2*dt, 0*dt, 0*dt; //Accumulative ballspeed, framerate-independendt using tickrate. Dag set dt at a 60-ish hz rate at 0.017
//      std::cout << "ball is moving" << std::endl;
//    }
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
