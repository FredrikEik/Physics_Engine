#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(-1.5,0,0);
    mScale.scale(0.25,0.25,0.25);
    gKraft = gForce*mass;

}

RollingBall::RollingBall(int n, VisualObject *surface) : OctahedronBall(n)
{
    mPosition.translate(-1.5,0,0);
    mScale.scale(0.25,0.25,0.25);
    r = 1.f;
    m_rekursjoner = n;
    m_indeks = 0;
    mTriangleSurface = surface;
    mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    oktaederUnitBall();
    mMatrix.setToIdentity();
    switchVersion = true;
}

RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{

    mMatrix = mPosition * mScale;


    if(!switchVersion)
        barycentricCords(dt);
    else
        barycentricCords2(dt);




//mPosition.translate(0,0,0);

}

void RollingBall::barycentricCords2(float dt)
{
    //std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    if (mTime == 0)
    {mVelocity = {0.f, 0.f, 0.f};

    }
    mTime += dt;
    int index{0};

    qDebug() << "utenfor forloop";
    for (unsigned int i{0}; i < mTriangleSurface->mTriangles.size(); i++)
    {
        //searches for current triangle with barycentric coordinates
        Triangle t = mTriangleSurface->mTriangles[i];
        gsml::Vector3d bar = baryCoord(gsml::Vector2d{t.V1.getX(), t.V1.getZ()},
                                  gsml::Vector2d{t.V2.getX(), t.V2.getZ()},
                                  gsml::Vector2d{t.V3.getX(), t.V3.getZ()},
                                  gsml::Vector2d{mMatrix.getPosition().x, mMatrix.getPosition().z});


        if (bar.x >= 0 && bar.x <= 1 && bar.y >= 0 && bar.y <= 1 && bar.z >= 0 && bar.z <= 1)
        {
            qDebug() << "Barycentric coordinates: (" << bar.x << ", " << bar.y << ", " << bar.z << ")";

            //calculates normal and acceleration on current triangle
            mPrevIndex = i;

            gsml::Vector3d n = gsml::Vector3d::cross(t.V2.mXYZ - t.V1.mXYZ, t.V3.mXYZ - t.V1.mXYZ);
            n.normalize();

            //a = [x * y, y^2 -1. z * y] * g
            akselerasjon = gsml::Vector3d{n.x * n.y, (n.y * n.y) -1, n.z * n.y} * gravitation;
            akselerasjon.normalize();

            mMatrix.setPosition(mMatrix.getPosition().x,
                                (t.V1.getY() * bar.x) + (t.V2.getY() * bar.y) + (t.V3.getY() * bar.z) + r,
                                mMatrix.getPosition().z);
        }

        if (index != mPrevIndex && index != -1)
        {
            qDebug() << "index != mPrevIndex && index != -1";
            //updates movement for the next triangles
            Triangle t0 = mTriangleSurface->mTriangles[mPrevIndex];
            Triangle t1 = mTriangleSurface->mTriangles[index];
            mPrevIndex = index;

            gsml::Vector3d n0 = gsml::Vector3d::cross(t0.V2.mXYZ - t0.V1.mXYZ, t0.V3.mXYZ - t0.V1.mXYZ).normalized();
            gsml::Vector3d n1 = gsml::Vector3d::cross(t1.V2.mXYZ - t1.V1.mXYZ, t1.V3.mXYZ - t1.V1.mXYZ).normalized();

            gsml::Vector3d cn = gsml::Vector3d(n0 + n1).normalized();

            mVelocity -= (cn * gsml::Vector3d::dot(mVelocity, cn) * 2);

            akselerasjon = gsml::Vector3d{n1.x * n1.y, (n1.y * n1.y) -1, n1.z * n1.y} * gravitation;
            akselerasjon.normalize();
        }

        //calculates velocity and moves the ball
        mVelocity += akselerasjon * dt * mMass;
        mMatrix.translate(mVelocity);

        qDebug() << "inni forloop";
    }


}

void RollingBall::barycentricCords(float dt)
{

    gsml::Vector3d normalvektor = 0;
    gsml::Vector3d newPosition = 0;
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    for (unsigned int i=0; i < vertices.size(); i+=3){

        gsml::Vector3d pos1;
        gsml::Vector3d pos2;
        gsml::Vector3d pos3;
        pos1 = gsml::Vector3d(vertices[i].getXYZ());
        pos2 = gsml::Vector3d(vertices[i+1].getXYZ());
        pos3 = gsml::Vector3d(vertices[i+2].getXYZ());

        gsml::Vector3d BallPosition = mPosition.getPosition();
        gsml::Vector3d barCords = BallPosition.barycentricCoordinates(vertices[i].getXYZ(),
                                                  vertices[i+1].getXYZ(), vertices[i+2].getXYZ());

        if(barCords.x >= 0 && barCords.y >= 0 && barCords.z >= 0 &&
                barCords.x <= 1 && barCords.y <= 1 && barCords.z < 1){


            normalvektor = gsml::Vector3d::cross(pos3 - pos1,pos2 - pos1);
            normalvektor.normalize();


            akselerasjon = gKraft * normalvektor * normalvektor.z;

            if(i==3){
            hastighet = hastighet + akselerasjon * dt;
            newPosition = BallPosition + hastighet;
            newPosition.z = pos1.z*barCords.x+pos2.z*barCords.y+pos3.z*barCords.z;
            newPosition.x -= (pos1.x*barCords.x+pos2.x*barCords.x+pos3.x*barCords.x) * friction;
            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z+radius);

            qDebug() << hastighet.x << hastighet.y << hastighet.z;
            }
            else{
            hastighet = hastighet - akselerasjon * dt;
            newPosition = BallPosition + hastighet;
            newPosition.z = pos1.z*barCords.x+pos2.z*barCords.y+pos3.z*barCords.z;
            newPosition.x -= (pos1.x*barCords.x+pos2.x*barCords.x+pos3.x*barCords.x) * friction;
            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z+radius);

            qDebug() << hastighet.x << hastighet.y << hastighet.z;
            }


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
