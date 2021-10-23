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
    TriangleSurface* surface = dynamic_cast<TriangleSurface*>(triangle_surface);
    std::vector<gsml::Vertex>& vertices = surface->get_vertices();

//    float terrainHeight = surface->getTerrainHeight(getPosition());

    mMatrix = mPosition * mScale;

}

void RollingBall::move(float dx, float dy, float dz)
{
    if(bIsSimulating)
        return;

    // TODO: Make barycentric coordinates work
    qDebug()<< "Should move, height: "<<dynamic_cast<TriangleSurface*>(triangle_surface)->getTerrainHeight(getPosition());
    // TODO: Position is transposed from what i'm used to, take that into account or return transposed version
    qDebug() << mPosition.getRow(3).toVector3D();
    mPosition.translate(dx, dy, dz);
    mPosition(2, 3) = dynamic_cast<TriangleSurface*>(triangle_surface)->getTerrainHeight(getPosition());

    mMatrix = mPosition * mScale;
}

void RollingBall::startSimulation()
{
    bIsSimulating = true;
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
