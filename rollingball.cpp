#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};

    mScale.scale(0.25,0.25,0.25);
    mPosition.translate(-0.5,-0.5,0.0);
    gForce = gAcceleration * massInKg;
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    gsml::Vector3d barycCoords;
    gsml::Vector3d ballPosition = mPosition.getPosition();

    for(int i = 0; i < vertices.size() - 2; i+= 3) //0
    {
        gsml::Vector3d p1, p2, p3;
        p1 = gsml::Vector3d(vertices[i].getXYZ());
        p2 = gsml::Vector3d(vertices[i+1].getXYZ());
        p3 = gsml::Vector3d(vertices[i+2].getXYZ());
        //qDebug() << "p1:" << p1.x<<p1.y<<p1.z << " p2:" << p2.x<<p2.y<<p3.z << " p3:" << p3.x<<p3.y<<p3.z;

        barycCoords = ballPosition.barycentricCoordinates(p1, p2, p3);
        qDebug() << i << barycCoords.x << barycCoords.y << barycCoords.z;

        if(barycCoords.x >= 0 && barycCoords.y >= 0 && barycCoords.z >= 0)
        {

            qDebug() << "pos before:   " << ballPosition.x << ballPosition.y << ballPosition.z;
            qDebug() << i << barycCoords.x << barycCoords.y << barycCoords.z;

            gsml::Vector3d p12 = p2-p1;
                    //gsml::Vector3d(abs(p2.x), abs(p2.y), abs(p2.z)) - gsml::Vector3d(abs(p1.x), abs(p1.y), abs(p1.z));
            gsml::Vector3d p13 = p3-p1;
                    //gsml::Vector3d(abs(p3.x), abs(p3.y), abs(p3.z)) - gsml::Vector3d(abs(p1.x), abs(p1.y), abs(p1.z));
            gsml::Vector3d pNormal = p12^p13;
            //if(i == 3) {pNormal = gsml::Vector3d(-pNormal.x, -pNormal.y, -pNormal.z);}




            qDebug() << "pNormal: " << pNormal.x << pNormal.y << pNormal.z;
            pNormal.normalize();
            qDebug() << "pNormal normalized: " << pNormal.x << pNormal.y << pNormal.z;

            //gForce.z = abs(gForce.z);
            acceleration = gForce * pNormal * pNormal.z;
            //acceleration = gAcceleration * gsml::Vector3d(pNormal.x*pNormal.z, pNormal.y*pNormal.z, pNormal.z*pNormal.z-1);
            if(i == 0) {acceleration = gsml::Vector3d(-acceleration.x, -acceleration.y, -acceleration.z);}

            qDebug() << pNormal.z;


            velocity = velocity + acceleration * dt;

            float zOffset = 0.25f;
            gsml::Vector3d newPosition = mPosition.getPosition() + velocity * dt;
            newPosition.z = p1.z*barycCoords.x + p2.z*barycCoords.y + p3.z*barycCoords.z;
            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z + zOffset);

            //mPosition.translate(velocity.x, velocity.y, velocity.z);
            ballPosition = mPosition.getPosition();
            qDebug() << "pos after:    " << ballPosition.x << ballPosition.y << ballPosition.z;
            qDebug() << "acceleration: " << acceleration.x << acceleration.y << acceleration.z;
            qDebug() << "velocity:     " << velocity.x << velocity.y << velocity.z;
        }
        //qDebug() << "ballpos: " << ballPosition.x << ballPosition.y << ballPosition.z;
    }

    mMatrix = mPosition * mScale;

}

void RollingBall::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    mMatrix.setPosition(x,y,z);
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
