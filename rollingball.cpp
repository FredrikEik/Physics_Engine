#include "rollingball.h"
#include "las.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(1.5,0,2);
    mScale.scale(0.25,0.25,0.25);
    gKraft = gForce*mass;
    gForce = gAcceleration * massInKg;
}

RollingBall::RollingBall(int n, VisualObject *surface) : OctahedronBall(n)
{
    mPosition.translate(1.5,0,0);
    mScale.scale(0.25,0.25,0.25);
    r = 1.f;
    m_rekursjoner = n;
    m_indeks = 0;
    mTriangleSurface = surface;
    mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    oktaederUnitBall();
    mMatrix.setToIdentity();
    //switchVersion = true;
}

RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{

    mMatrix = mPosition * mScale;


    if(!switchVersion)
       // barycentricCords(dt);
        moveAlongLAs(dt);
    else
        barycentricCords(dt);



}

void RollingBall::moveAlongLAs( float dt)
{

    /*
  //  std::vector<gsml::Vertex>& vertices = temp->get_vertices();

    std::vector<gsml::Vertex>& vertices = dynamic_cast<class LAS*>(_las)->mVertices;
    //mMatrix = mPosition * mScale;

     // MÅ FIKSES!

    gsml::Vector3d barycCoords;
    gsml::Vector3d ballPosition = mMatrix.getPosition();

    for(unsigned int i = 0; i < mLas->mTriangles.size() ; i++)
    {

        Triangle t = mLas->mTriangles[i];
        gsml::Vector3d bar = baryCoord(gsml::Vector2d{t.V1.getX(), t.V1.getZ()},
                                  gsml::Vector2d{t.V2.getX(), t.V2.getZ()},
                                  gsml::Vector2d{t.V3.getX(), t.V3.getZ()},
                                  gsml::Vector2d{mMatrix.getPosition().x, mMatrix.getPosition().z});


        gsml::Vector3d p1 = bar.getX();
        gsml::Vector3d p2 = bar.getY();
        gsml::Vector3d p3 = bar.getZ();
//        Triangle t = mLas->mTriangles[i];
//        gsml::Vector3d p1, p2, p3;
//        p1 = gsml::Vector3d(t.V1.getXYZ());
//        p2 = gsml::Vector3d(t.V2.getXYZ());
//        p3 = gsml::Vector3d(t.V3.getXYZ());
        //qDebug() << "p1:" << p1.x<<p1.y<<p1.z << " p2:" << p2.x<<p2.y<<p3.z << " p3:" << p3.x<<p3.y<<p3.z;

        barycCoords = ballPosition.barycentricCoordinates(p1, p2, p3);
        //qDebug() << i << barycCoords.x << barycCoords.y << barycCoords.z;

        if(barycCoords.x >= 0 && barycCoords.y >= 0 && barycCoords.z >= 0)
        {

            //qDebug() << "pos before:   " << ballPosition.x << ballPosition.y << ballPosition.z;
            //qDebug() << "barycentric index: " i << barycCoords.x << barycCoords.y << barycCoords.z;

            gsml::Vector3d p12 = p2-p1;
            gsml::Vector3d p13 = p3-p1;
            //qDebug() << "p12, p13" << p12.x << p12.y << p12.z << p13.x << p13.y << p13.z;
            gsml::Vector3d pNormal = p12^p13;

            //qDebug() << "pNormal not normalized: " << pNormal.x << pNormal.y << pNormal.z;
            pNormal.normalize();
            //qDebug() << "pNormal normalized: " << pNormal.x << pNormal.y << pNormal.z;

            gForce = gsml::Vector3d(abs(gForce.x), abs(gForce.y), abs(gForce.z));
            //gsml::Vector3d pz{pNormal.z, pNormal.z, pNormal.z};

            acceleration = gForce ^ pNormal ^ gsml::Vector3d(0, pNormal.y, 0);
            //acceleration = gAcceleration * gsml::Vector3d(pNormal.x*pNormal.z, pNormal.y*pNormal.z, pNormal.z*pNormal.z-1);
            //acceleration = acceleration + friction;
            velocity = velocity + acceleration * dt;

            float yOffset = 0.25f;
            gsml::Vector3d newPosition = mMatrix.getPosition() + velocity * dt;
            newPosition.y = p1.y*barycCoords.x + p2.y*barycCoords.y + p3.y*barycCoords.z;
            mMatrix.setPosition(newPosition.x, newPosition.y+ yOffset, newPosition.z);

            ballPosition = mMatrix.getPosition();

            //mPosition.translate(velocity.x, velocity.y, velocity.z);
            //qDebug() << "pos after:    " << ballPosition.x << ballPosition.y << ballPosition.z;
            //qDebug() << "acceleration: " << acceleration.x << acceleration.y << acceleration.z;
            //qDebug() << "velocity:     " << velocity.x << velocity.y << velocity.z;
        }
        //qDebug() << "ballpos: " << ballPosition.x << ballPosition.y << ballPosition.z;
    }
    */


    std::vector<gsml::Vertex>& vertices = dynamic_cast<LAS*>(_las)->get_vertices();

    for (unsigned int i=0; i < vertices.size() ; i+=3){

        gsml::Vector3d pos1;
        gsml::Vector3d pos2;
        gsml::Vector3d pos3;
        pos1 = gsml::Vector3d(vertices[i].getXYZ());
        pos2 = gsml::Vector3d(vertices[i+1].getXYZ());
        pos3 = gsml::Vector3d(vertices[i+2].getXYZ());

        gsml::Vector3d BallPos = mPosition.getPosition();
        gsml::Vector3d barCords = BallPos.barycentricCoordinates(vertices[i].getXYZ(),
                                                  vertices[i+1].getXYZ(), vertices[i+2].getXYZ());

        if(barCords.x >= 0 && barCords.y >= 0 && barCords.z >= 0)
        {

            gsml::Vector3d p12 = pos2-pos1;
            gsml::Vector3d p13 = pos3-pos1;
            //qDebug() << "p12, p13" << p12.x << p12.y << p12.z << p13.x << p13.y << p13.z;
            gsml::Vector3d pNormal = p12^p13;

            //qDebug() << "pNormal not normalized: " << pNormal.x << pNormal.y << pNormal.z;
            pNormal.normalize();

            old_normal = pNormal;
            //qDebug() << "pNormal normalized: " << pNormal.x << pNormal.y << pNormal.z;

            gKraft = gsml::Vector3d(abs(gKraft.x), abs(gKraft.y), abs(gKraft.z));
            //gsml::Vector3d pz{pNormal.z, pNormal.z, pNormal.z};

            akselerasjon = gKraft ^ pNormal ^ gsml::Vector3d(0, pNormal.y, 0);
            //acceleration = gAcceleration * gsml::Vector3d(pNormal.x*pNormal.z, pNormal.y*pNormal.z, pNormal.z*pNormal.z-1);
            //acceleration = acceleration + friction;
            hastighet = hastighet + acceleration * dt;

            float yOffset = 0.25f;
            gsml::Vector3d newPosition = mPosition.getPosition() + hastighet * dt;
            newPosition.y = pos1.y*barCords.x + pos2.y*barCords.y + pos3.y*barCords.z;
            mPosition.setPosition(newPosition.x, newPosition.y+ yOffset, newPosition.z);

            BallPos = mPosition.getPosition();





        }



    }

}

void RollingBall::barycentricCords2(float dt)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    if (mTime == 0)
    {mVelocity = {0.f, 0.f, 0.f};

    }
    mTime += dt;
    int index{0};

    qDebug() << "utenfor forloop";
    for (unsigned int i{0}; i < /*mTriangleSurface*/mLas->mTriangles.size(); i++)
    {
        //searches for current triangle with barycentric coordinates
        Triangle t = /*mTriangleSurface*/mLas->mTriangles[i];
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

void RollingBall::setSurface(VisualObject *surface)
{
    if(!switchVersion)
        _las = surface;
    else
        triangle_surface = surface;

}

void RollingBall::barycentricCords(float dt)
{

    float rotspeed;
    gsml::Vector3d normalvektor = 0;
    gsml::Vector3d newPos = 0;
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    // Finne trekant
    for (unsigned int i=0; i < vertices.size(); i+=3){

        /* Finn trekantvertices, v0,v1,v2
         * finn ballens posisjon
         * finne triangelet ballen er på med barysentriske koordinater

         */
        gsml::Vector3d pos1;
        gsml::Vector3d pos2;
        gsml::Vector3d pos3;
        pos1 = gsml::Vector3d(vertices[i].getXYZ());
        pos2 = gsml::Vector3d(vertices[i+1].getXYZ());
        pos3 = gsml::Vector3d(vertices[i+2].getXYZ());

        gsml::Vector3d BallPos = mPosition.getPosition();
        gsml::Vector3d barCords = BallPos.barycentricCoordinates(vertices[i].getXYZ(),
                                                  vertices[i+1].getXYZ(), vertices[i+2].getXYZ());

        if(barCords.x >= 0 && barCords.y >= 0 && barCords.z >= 0 &&
                barCords.x <= 1 && barCords.y <= 1 && barCords.z < 1){
            // beregne normal og akselerasjonsvektor
            // oppdatere hastighet og posisjon

            /*
            Formula for kryssprodukt:
            x = (Ay * Bz) - (By * Az)
            y = (Az * Bx) - (Bz * Ax)
            z = (Ax * By) - (Bx * Ay)
            */

            normalvektor = gsml::Vector3d::cross(pos3 - pos1,pos2 - pos1);
            normalvektor.normalize();

            old_normal = normalvektor;



            akselerasjon = gKraft * normalvektor * normalvektor.z;

            if(i==3){

                //korriger posisjon oppover i normalens retning
                //oppdaterer hastighet
                //lagt til rotasjon
            hastighet = hastighet + akselerasjon * dt;
            newPos = BallPos + hastighet;
            newPos.z = pos1.z*barCords.x+pos2.z*barCords.y+pos3.z*barCords.z;
            newPos.x -= (pos1.x*barCords.x+pos2.x*barCords.x+pos3.x*barCords.x) * friction;
            rotspeed += newPos.x * gravitation;
            mPosition.setPosition(newPos.x, newPos.y, newPos.z+radius);
            mPosition.rotate(rotspeed, 1, 0, 0);


            qDebug() << hastighet.x << hastighet.y << hastighet.z;
            }
            if(i<3){
                //ballen er over på ny trekant
                //kalkulere ny hastighet og posisjon
            hastighet = hastighet - akselerasjon * dt;
            newPos = BallPos + hastighet;
            newPos.z = pos1.z*barCords.x+pos2.z*barCords.y+pos3.z*barCords.z;
            newPos.x -= (pos1.x*barCords.x+pos2.x*barCords.x+pos3.x*barCords.x) * friction;
            rotspeed -= newPos.x * gravitation;
            mPosition.setPosition(newPos.x, newPos.y, newPos.z+radius);
            //mPosition.rotate(-rotspeed,0,1,0);
            mPosition.rotate(-rotspeed,1,0,0);


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
