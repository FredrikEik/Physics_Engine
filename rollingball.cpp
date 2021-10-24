#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(-1,0.5,1);
    mScale.scale(0.25,0.25,0.25);
    gForce = gAcceleration * mass;
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

    //qDebug() <<ballPos.x << ballPos.y << ballPos.z;
    //qDebug() << i << baryCoords.x << baryCoords.y << baryCoords.z;

    if(baryCoords.x >= 0 && baryCoords.y >= 0 && baryCoords.z >= 0)
        {
        //qDebug() << ballPos.x << ballPos.y << ballPos.z;
        qDebug() << velocity.x << velocity.y << velocity.z;
        //qDebug()<< i;
        /*
        p1.x = abs(p1.x); p1.y = abs(p1.y); p1.z = abs(p1.z);
        p2.x = abs(p2.x); p2.y = abs(p2.y); p2.z = abs(p2.z);
        p3.x = abs(p3.x); p3.y = abs(p3.y); p3.z = abs(p3.z);
        */

        gsml::Vec3 p12 = p2-p1;
        gsml::Vec3 p13 = p3-p1;
        gsml::Vec3 planeNormal = p12^p13;
        planeNormal.normalize();

        acceleration = gForce * planeNormal * planeNormal.z;
        if(i==0){velocity = velocity - acceleration * 0.00017;}
        else{velocity = velocity + acceleration * 0.00017;}

        gsml::Vec3 newPos = ballPos + velocity;
        newPos.z = (baryCoords.x * p1.z + baryCoords.y * p2.z + baryCoords.z * p3.z)+0.25;
        mPosition.setPosition(newPos.x,newPos.y,newPos.z);

        //qDebug()<< velocity.x << velocity.y << velocity.z;
        //mPosition.translate(velocity.x,velocity.y,velocity.z);
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
