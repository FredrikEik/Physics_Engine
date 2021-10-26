#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.setPosition(gsml::Vector3d{-2,-2,1.5});
    mVecPosition = {-2,-2,1.5};
    mScale.scale(0.25,0.25,0.25);
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    gsml::Vector3d ballPos = mPosition.getPosition();
    gsml::Vector2d rBallPos = {ballPos.x, ballPos.y};

    gsml::Vector3d baryC1 = barycentricCoords(vertices[0].getXY(), vertices[1].getXY(), vertices[2].getXY(), rBallPos);
    gsml::Vector3d baryC2 = barycentricCoords(vertices[5].getXY(), vertices[3].getXY(), vertices[4].getXY(), rBallPos);

    gsml::Vector3d norm1;
    gsml::Vector3d norm2;


    if(baryC1.x >= 0 && baryC1.y >= 0 && baryC1.z >= 0)
    {
        gsml::Vector3d vec1 = vertices[1].getXYZ() - vertices[0].getXYZ();
        gsml::Vector3d vec2 = vertices[2].getXYZ() - vertices[0].getXYZ();
        gsml::Vector3d norm1 = vec1.cross(vec2);
        norm1.normalize();
        gsml::Vector3d aVec = {norm1.x * norm1.z, norm1.y * norm1.z, (norm1.z * norm1.z) - 1};
        const float g = 9.81;
        aVec = {aVec.x * g, aVec.y * g, aVec.z * g};

        mVelocity = mVelocity + aVec * dt;
        mVecPosition = mVecPosition + mVelocity * dt;

        mPosition.setPosition(mVecPosition);

        mMatrix = mPosition * mScale;

        return;
    }
    else
    {
        gsml::Vector3d vec1 = vertices[3].getXYZ() - vertices[5].getXYZ();
        gsml::Vector3d vec2 = vertices[4].getXYZ() - vertices[5].getXYZ();
        gsml::Vector3d norm2 = vec1.cross(vec2);
        norm2.normalize();

        gsml::Vector3d x = (norm1 + norm2);
        x = {x.x / x.length(), x.y / x.length(), x.z / x.length()};
        QVector3D tempX = {x.x, x.y, x.z};
        QVector3D Vel = {mVelocity.x, mVelocity.y, mVelocity.z};
        QVector3D nextVelocity = Vel - (2 * (Vel * tempX) * tempX);
        mVelocity = {nextVelocity.x(), nextVelocity.y(), nextVelocity.z()};
    }

    if(baryC2.x >= 0 && baryC2.y >= 0 && baryC2.z >= 0)
    {
        gsml::Vector3d vec1 = vertices[3].getXYZ() - vertices[5].getXYZ();
        gsml::Vector3d vec2 = vertices[4].getXYZ() - vertices[5].getXYZ();
        gsml::Vector3d norm2 = vec1.cross(vec2);
        norm2.normalize();
        gsml::Vector3d aVec = {norm2.x * norm2.z, norm2.y * norm2.z, (norm2.z * norm2.z) - 1};
        const float g = 9.81;
        aVec = {aVec.x * g, aVec.y * g, aVec.z * g};

        mVelocity = mVelocity + aVec * dt;
        mVecPosition = mVecPosition + mVelocity * dt;

        mPosition.setPosition(mVecPosition);

        mMatrix = mPosition * mScale;

        return;
    }
    else
    {
        gsml::Vector3d x = (norm1 + norm2);
        x = {x.x / x.length(), x.y / x.length(), x.z / x.length()};
        QVector3D tempX = {x.x, x.y, x.z};
        QVector3D Vel = {mVelocity.x, mVelocity.y, mVelocity.z};
        QVector3D nextVelocity = Vel - (2 * (Vel * tempX) * tempX);
        mVelocity = {nextVelocity.x(), nextVelocity.y(), nextVelocity.z()};
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
