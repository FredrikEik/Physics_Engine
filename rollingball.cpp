#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    mPosition.translate(1,0,2);
    mScale.scale(0.25,0.25,0.25);
    //force.z = -0.01f;
    force = gAccel * kiloMass;

}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    gsml::Vector3d baryCoords;
    gsml::Vector3d bPosition = mPosition.getPosition();

    for(int i = 0; i < vertices.size() - 2; i += 3)
        {
            gsml::Vector3d p1, p2, p3;
            p1 = gsml::Vector3d(vertices[i].getXYZ());
            p2 = gsml::Vector3d(vertices[i+1].getXYZ());
            p3 = gsml::Vector3d(vertices[i+2].getXYZ());
            //qDebug() << "p1:" << p1.x<<p1.y<<p1.z << " p2:" << p2.x<<p2.y<<p3.z << " p3:" << p3.x<<p3.y<<p3.z;

            baryCoords = bPosition.barycentricCoordinates(p1, p2, p3);
            qDebug() << i << baryCoords.x << baryCoords.y << baryCoords.z;

            if(baryCoords.x >= 0 && baryCoords.y >= 0 && baryCoords.z >= 0)
            {

                //qDebug() << "pos before:   " << bPosition.x << bPosition.y << bPosition.z;
                //qDebug() << "barycentric index: " i << baryCoords.x << baryCoords.y << baryCoords.z;

                gsml::Vector3d p12 = p2-p1;
                gsml::Vector3d p13 = p3-p1;
                gsml::Vector3d pNormal = p12^p13;

                //qDebug() << "pNormal not normalized: " << pNormal.x << pNormal.y << pNormal.z;
                pNormal.normalize();
                //qDebug() << "pNormal normalized: " << pNormal.x << pNormal.y << pNormal.z;

                //gForce.z = abs(gForce.z);
                accel = force * pNormal * pNormal.z;
                //accel = gAccel * gsml::Vector3d(pNormal.x*pNormal.z, pNormal.y*pNormal.z, pNormal.z*pNormal.z-1);
                if(i == 0)
                {
                    accel = gsml::Vector3d(-accel.x, -accel.y, -accel.z);
                }

                velocity = velocity + accel * dt;

                float zOffset = 0.25f;
                gsml::Vector3d newPosition = mPosition.getPosition() + velocity * dt;
                newPosition.z = p1.z*baryCoords.x + p2.z*baryCoords.y + p3.z*baryCoords.z;
                mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z + zOffset);

                bPosition = mPosition.getPosition();
            }
            //qDebug() << "ballpos: " << bPosition.x << bPosition.y << bPosition.z;
        }

        mMatrix = mPosition * mScale;

    /**gsml::Vector3d direction = gsml::Vector3d(0.f,0.f,-dt);
    mPosition.translate(direction.x, direction.y, direction.z);
    mMatrix = mPosition * mScale;*/

}

/**gsml::Vector3d RollingBall::calculateNewForce(gsml::Vector3d normal1, gsml::Vector3d normal2, float dt)
{
    gsml::Vector3d ballForce = gsml::Vector3d(0.f,0.f,-1.f);
    gsml::Vector3d normal = normal1 + normal2;
    normal.normalize();
    float degrees = 0;
    ballForce = ballForce + normal * std::cos(degrees);

}*/

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
