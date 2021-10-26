#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};

    mPosition.translate(-0.5,-0.5,0.0);
    mScale.scale(0.25,0.25,0.25);
    baseForce = baseAcceleration * mass;
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    //std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    gsml::Vector3d pos = mPosition.getPosition(); //use for function call for bary.
    float height = getHeight(pos);

    velocity = velocity + acceleration * dt;
    gsml::Vector3d newPos = pos + velocity * dt;
    newPos.y = height;
    mPosition.setPosition(newPos);

    mMatrix = mPosition * mScale;
}

bool RollingBall::findTriangle(unsigned int index, gsml::Vector3d &position, gsml::Vector3d &outBaryCords, gsml::Vector3d &outP, gsml::Vector3d &outQ, gsml::Vector3d &outR) const
{
    gsml::Vector3d p, q, r;
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    p = gsml::Vector3d(vertices[index].getXYZ().x, vertices[index].getXYZ().y, 1.f);
    q = gsml::Vector3d(vertices[index+1].getXYZ().x, vertices[index+1].getXYZ().y, 1.f);
    r = gsml::Vector3d(vertices[index+2].getXYZ().x, vertices[index+2].getXYZ().y, 1.f);

    outBaryCords = position.barycentricCoordinates(p, q, r);

    if (outBaryCords.x >= 0 && outBaryCords.y >= 0 && outBaryCords.z >= 0)
    {
        outP = vertices[index].getXYZ();
        outQ = vertices[index+1].getXYZ();
        outR = vertices[index+2].getXYZ();
        return true;
    }

    return false;
}

float RollingBall::getHeight(gsml::Vector3d& positon)
{
    gsml::Vector3d BaryCords, p, q, r;
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    for (unsigned int i{}; i < vertices.size(); i += 3)
    {
        if (findTriangle(i, positon, BaryCords, p, q, r))
        {
            RollingBall::calculateMovement(p, q, r);
            break;
        }
    }

    return BaryCords.x * p.y + BaryCords.y * q.y + BaryCords.z * r.y;

}

void RollingBall::calculateMovement(const gsml::Vector3d& p, const gsml::Vector3d& q, const gsml::Vector3d& r)
{
    //Normal
    gsml::Vector3d pq = q - p;
    gsml::Vector3d pr = r - p;
    gsml::Vector3d normal = pq ^ pr;
    normal.normalize();

    acceleration = baseForce * normal * normal.y;
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


