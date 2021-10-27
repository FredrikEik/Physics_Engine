#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};

    mPosition.translate(-0.5,-0.5,1.0);
    mScale.scale(0.25,0.25,0.25);
    //baseForce = baseAcceleration * mass;
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    //std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    gsml::Vector3d pos = mPosition.getPosition(); //use for function call for bary.
    float height = getHeight(pos) + radius;

    if(current_index != old_index)
        doCollition();

    velocity = velocity + acceleration * dt;
    gsml::Vector3d newPos = pos + velocity * dt;
    newPos.z = height;

    mPosition.setPosition(newPos);
    mMatrix = mPosition * mScale;

    old_index = current_index;
    old_normal = current_normal;
}

bool RollingBall::findTriangle(unsigned int index, gsml::Vector3d &position, gsml::Vector3d &outBaryCords, gsml::Vector3d &outP, gsml::Vector3d &outQ, gsml::Vector3d &outR) const
{
    gsml::Vector3d p, q, r;
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    p = vertices[index].getXYZ();
    q = vertices[index+1].getXYZ();
    r = vertices[index+2].getXYZ();

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
            current_index = i;
            RollingBall::calculateMovement(p, q, r);
            break;
        }
    }

    return BaryCords.x * p.z + BaryCords.y * q.z + BaryCords.z * r.z;

}

void RollingBall::calculateMovement(const gsml::Vector3d& p, const gsml::Vector3d& q, const gsml::Vector3d& r)
{
    //Normal
    gsml::Vector3d pq = q - p;
    gsml::Vector3d pr = r - p;
    gsml::Vector3d normal = pq ^ pr;
    normal.normalize();
    current_normal = normal;

    //qDebug() << normal.x << " " << normal.y << " " << normal.z;

    acceleration = gsml::Vector3d((normal.x * normal.z) * -gravity, (normal.y * normal.z) * -gravity, (normal.z * normal.z - 1) * -gravity);
}

void RollingBall::doCollition()
{
    gsml::Vector3d collisonNormal = (old_normal + current_normal)/(old_normal.length() + current_normal.length());
    velocity = velocity - collisonNormal * 2.f * (velocity * collisonNormal);
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


