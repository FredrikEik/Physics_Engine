#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(1,0,0.25);
    mScale.scale(0.25,0.25,0.25);
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

       mMatrix = mPosition * mScale;


       gsml::Vec3 ballPos = mPosition.getPosition();

       for(int i = 0; i < vertices.size(); i+=3)
       {
       gsml::Vec3 p1, p2, p3;
       p1 = gsml::Vec3(vertices[i].getXYZ());
       p2 = gsml::Vec3(vertices[i+1].getXYZ());
       p3 = gsml::Vec3(vertices[i+2].getXYZ());

       gsml::Vec3 baryCoords = ballPos.barycentricCoordinates(p1,p2,p3);


       if(baryCoords.x >= 0 && baryCoords.y >= 0 && baryCoords.z >= 0)
           {

           gsml::Vec3 p12 = p2-p1;
           gsml::Vec3 p13 = p3-p1;
           gsml::Vec3 planeNormal = p12^p13;
           planeNormal.normalize();

           gsml::Vec3 acceleration = (gravityForce * 0.001f) ^ planeNormal ^ gsml::Vec3(0,0,planeNormal.z);

           velocity = velocity + (acceleration * 0.17);

           gsml::Vec3 newPos = ballPos + velocity;
           newPos.z = (baryCoords.x * p1.z + baryCoords.y * p2.z + baryCoords.z * p3.z) + offset;
           mPosition.setPosition(newPos.x,newPos.y,newPos.z);

           }
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
