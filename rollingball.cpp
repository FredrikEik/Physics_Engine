#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(1.5,0,0);
    mScale.scale(0.25,0.25,0.25);

    gForce = constGForce*mass;
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    mMatrix = mPosition * mScale;

    for(int i = 0; i < vertices.size() - 2; i += 3)
    {
        point1 = vertices[i].getXYZ();
        point2 = vertices[i+1].getXYZ();
        point3 = vertices[i+2].getXYZ();

        setBallPosition( mPosition.getPosition());

        BarycentricCoordinates = getBallPosition().barycentricCoordinates(point1, point2, point3);


        if(   BarycentricCoordinates.x < 1 && BarycentricCoordinates.x > 0
           && BarycentricCoordinates.y < 1 && BarycentricCoordinates.y > 0
           && BarycentricCoordinates.z < 1 && BarycentricCoordinates.z > 0 )
        {
//            qDebug() << BarycentricCoordinates.x << " " << BarycentricCoordinates.y << " " << BarycentricCoordinates.z << " ";

        /** Beregne Normal */
            gsml::Vector3d Normal = (point2-point1).cross(point3-point2); //Cross product of points: 2-1 and 3-2.     == (2-1)^(3-2)
            Normal.normalize();

        /** Beregne akselerasjonsvektor */
            acceleration = gForce * (Normal * Normal.z);

        /** Oppdatere hastighet */
            if(i==0)
                speed = speed - (acceleration * dt);
            else
                speed = speed + (acceleration * dt);

        /** Oppdatere posisjon */
            nextPos = BallPosition + speed;
            nextPos.z = point1.z*BarycentricCoordinates.x+point2.z*BarycentricCoordinates.y+point3.z*BarycentricCoordinates.z;
            mPosition.setPosition(nextPos.x, nextPos.y, nextPos.z+radius);

        }
    }

}

void RollingBall::setBallPosition(gsml::Vector3d pos)
{
    BallPosition = pos;
}

gsml::Vector3d RollingBall::getBallPosition()
{
    return BallPosition;
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
