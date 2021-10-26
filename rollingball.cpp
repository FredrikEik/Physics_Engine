#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};

    mPosition.translate(0.5,0.5,0.5);
    mScale.scale(0.25,0.25,0.25);
    gForce = gAcceleration * massKg;
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();


    gsml::Vector3d barycentricCoords;
    gsml::Vector3d ballPosition = mPosition.get_position();

    for (int i = 0; i < vertices.size() - 2; i+= 3)
    {
        gsml::Vector3d p1, p2, p3;
        p1 = gsml::Vector3d(vertices[i].getXYZ());
        p2 = gsml::Vector3d(vertices[i+1].getXYZ());
        p3 = gsml::Vector3d(vertices[i+2].getXYZ());
//        qDebug() << "p1= " << p1.x << p1.y << p1.z << " p2= " << p2.x << p2.y << p2.z << " p3= " << p3.x << p3.y << p3.z;

        barycentricCoords = ballPosition.barycentricCoordinates(p1, p2, p3);
        qDebug() << i << barycentricCoords.x << " " << barycentricCoords.y << " " << barycentricCoords.z;

        if (barycentricCoords.x >= 0 && barycentricCoords.y >= 0 && barycentricCoords.z >= 0)
        {
            gsml::Vector3d p12 = p2 - p1;
            gsml::Vector3d p13 = p3 - p1;
            gsml::Vector3d pNormal = p12^p13;

//            qDebug() << "pNormal not normalized: " << pNormal.x << pNormal.y << pNormal.z;
            pNormal.normalize();
//            qDebug() << "pNormal normalized: " << pNormal.x << pNormal.y << pNormal.z;

//            acceleration = gForce * pNormal * pNormal.z;
            acceleration = gAcceleration * gsml::Vector3d(pNormal.x*pNormal.z, pNormal.y*pNormal.z, pNormal.z*pNormal.z-1);
            if (i == 0) {acceleration = gsml::Vector3d(-acceleration.x, -acceleration.y, -acceleration.z);}

            velocity = velocity - acceleration * dt;

            float zOffset = 0.25f;
            gsml::Vector3d newPos = mPosition.get_position() + velocity * dt;
            newPos.z = p1.z * barycentricCoords.x + p2.z * barycentricCoords.y + p3.z * barycentricCoords.z;
            mPosition.setPosition(newPos.x, newPos.y, newPos.z + zOffset);

            ballPosition = mPosition.get_position();


        }

    }

    mMatrix = mPosition * mScale;

}

void RollingBall::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    mMatrix.setPosition(x, y, z);
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
