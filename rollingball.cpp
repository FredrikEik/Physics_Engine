#include "rollingball.h"


RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    mPosition.translate(-0.5, -0.5, 0);
    mScale.scale(0.25, 0.25, 0.25);
    force.z = -9.81f;
    mVelocity = gsml::Vector3d{0.f};
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();
    gsml::Vector3d ballCoords = mPosition.getPosition();
    gsml::Vector3d newPosition;
    gsml::Vector3d normal;

    for(int i = 0; i < (vertices.size() - 2); i += 3)
    {
        gsml::Vector3d p0 = gsml::Vector3d(vertices[i].getXYZ());
        gsml::Vector3d p1 = gsml::Vector3d(vertices[i + 1].getXYZ());
        gsml::Vector3d p2 = gsml::Vector3d(vertices[i + 2].getXYZ());

        gsml::Vector3d baryCoords = ballCoords.barycentricCoordinates(p0, p1, p2);

        if (baryCoords.x >= 0 && baryCoords.y >= 0 && baryCoords.z >= 0)
        {
            normal = (p1 - p0)^(p2 - p0);
            normal.normalize();
            acceleration = force * normal * normal.z;

            if(i == 0) //reverse the acceleration when it passes center
                acceleration = gsml::Vector3d(-acceleration.x, -acceleration.y, -acceleration.z);
            mVelocity = mVelocity + acceleration * dt;

            float zOffset = 0.25f;
            newPosition = mPosition.getPosition() + mVelocity;
            newPosition.z = p0.z * baryCoords.x + p1.z * baryCoords.y + p2.z * baryCoords.z;
            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z + zOffset);
            ballCoords = mPosition.getPosition();
        }
    }

    mMatrix = mPosition * mScale;
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
