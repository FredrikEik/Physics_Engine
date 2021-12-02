#include "rollingball.h"
#include "las.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    physics = new Physics;
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(1.5,0,2);
    //_las->mMatrix.rotate(90,1,0,0);
    mScale.scale(0.4,0.4,0.4);
    //mScale.rotate(90,1,0,0);
    //mMatrix.rotate(90,1,0,0);


    //gForce = gAcceleration * massInKg;
}


RollingBall::~RollingBall()
{

}

gsml::Vector3d RollingBall::Get_position()
{
    gsml::Vector3d temp;

    temp.x = (mMatrix.getColumn(3).x());
    temp.y = (mMatrix.getColumn(3).y());
    temp.z = (mMatrix.getColumn(3).z());

    return temp;
}

void RollingBall::setPosition(gsml::Vector3d v)
{
    mPosition.setPosition(v.x, v.y, v.z);
    mMatrix = mPosition * mScale;
}

void RollingBall::setHeight(float z)
{
    gsml::Vector3d HeightVector{0,0,z};
    gsml::Vector3d Translation{0,0,0};
    Translation = Get_position();

    Translation.z = (HeightVector.z + physics->radius);

    if(z != mMatrix.getColumn(3).z())
    {
        setPosition(Translation);
        //mMatrix.setPosition(Translation.x,Translation.y,Translation.z);
    }
}
void RollingBall::move(float dx, float dy, float dz)
{
    mPosition.setPosition(dx, dy, dz);
    //mMatrix = mPosition * mScale;
}
void RollingBall::setSurface2(VisualObject* surface)
{
    _las = surface;
    surfVertices = _las->get_vertices();
    int mT = static_cast<int>(surfVertices.size());
    if(surfVertices.size()>100){
        mT = rand()%mT;
        qDebug() << mT;
        gsml::Vector3d v1 =surfVertices.at(mT).getXYZ();
        gsml::Vector3d v2 =surfVertices.at(mT+1).getXYZ();
        gsml::Vector3d v3 =surfVertices.at(mT+2).getXYZ();
        gsml::Vector3d pos = (v1+v2+v3)*0.333;
        pos.z += 50;
        setPosition(pos);}
    else
        move(1,1,5);
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

    gsml::Vector3d bary;
    //std::vector<gsml::Vertex> vertices = triangle_surface->get_vertices();
    gsml::Vector2d ballPosXY(Get_position().x, Get_position().y);
    for (size_t i=0; i<surfVertices.size(); i++)
    {
        //qDebug() << "ground size: " << vertices.size();
        gsml::Vector3d p1 = surfVertices[i].getXYZ();
        gsml::Vector3d p2 = surfVertices[++i].getXYZ();
        gsml::Vector3d p3 = surfVertices[++i].getXYZ();
        //qDebug() << "p1: " << p1.x << p1.y << p1.z << "p2: " << p2.x << p2.y << p2.z << "p3: " << p3.x << p3.y << p3.z;

        m_index = static_cast<int>(i+1) /3;

        bary = baryCoord(gsml::Vector2d(p1.x, p1.y),
                                 gsml::Vector2d(p2.x, p2.y),
                                 gsml::Vector2d(p3.x, p3.y),
                                 ballPosXY);

        //qDebug() << "bary value: " << bary.x << " " << bary.y << " " << bary.z;

        if (bary.x >=0 && bary.y >=0 && bary.z >=0)
        {
            gsml::Vector3d mPos;
            gsml::Vector3d p12 = p2-p1;
            gsml::Vector3d p13 = p3-p1;
            m_normal = p12^p13;
            m_normal.normalize();

            float mHeight = Get_position().z - barycentricHeight(Get_position(), p1,p2,p3);
            mHeight = sqrt(mHeight * mHeight);
            bool isFalling{false};
            if(mHeight > physics->radius+0.2)
                isFalling = true;
            else
                isFalling = false;


            if(isFalling)
            {
                physics->Falling();
                //qDebug() << "Fritt Fall";
                mN = m_normal;
                mN.normalize();
                m_index = -1;
                qDebug() << "mVelocity: " << physics->Velocity.x << physics->Velocity.y << physics->Velocity.z;
            }
            else
            {
                physics->onGround(m_normal);
                setHeight(barycentricHeight(Get_position(), p1,p2,p3));
                mN = m_normal + old_normal;
                mN.normalize();
                qDebug() << "mVelocity: " << physics->Velocity.x << physics->Velocity.y << physics->Velocity.z;
            }
            physics->Velocity = physics->OldVelocity + physics->Acceleration * dt;
            mPos = (physics->OldVelocity + physics->Velocity) * (dt/2);

            mPosition.translate(mPos.x, mPos.y, mPos.z);
            mMatrix = mPosition * mScale;

            if(m_index != old_index)
            {
                physics->Velocity = mN * gsml::Vector3d::dot(physics->OldVelocity, mN);
                physics->Velocity = physics->OldVelocity - physics->Velocity * 2;
                physics->Velocity = physics->Velocity * physics->friction;
                //qDebug() << "mVelocity: " << p->Velocity.x << p->Velocity.y << p->Velocity.z;
            }
            physics->OldVelocity = physics->Velocity;
            old_normal = m_normal;
            old_index = m_index;
            break;
        }
    }


}




void RollingBall::setSurface(VisualObject *surface)
{

        triangle_surface = surface;


}

void RollingBall::barycentricCords(float dt)
{
    gsml::Vector3d gKraft;
    physics->mass = 0.01;
    gKraft = physics->Acceleration*physics->mass;
    mMatrix = mPosition * mScale;

    float rotspeed = 2;
    gsml::Vector3d normalvektor = 0;
    gsml::Vector3d newPos = 0;
    gsml::Vector3d akselerasjon{0.0f,0.0f,0.0f};
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
            physics->Velocity = physics->Velocity + akselerasjon * dt;
            newPos = BallPos + physics->Velocity;
            newPos.z = pos1.z*barCords.x+pos2.z*barCords.y+pos3.z*barCords.z;
            newPos.x -= (pos1.x*barCords.x+pos2.x*barCords.x+pos3.x*barCords.x) * physics->friction;
            //rotspeed += newPos.x;// * physics->mass;
            mPosition.setPosition(newPos.x, newPos.y, newPos.z+radius);
            mPosition.rotate(rotspeed, 1, 0, 0);


            qDebug() << physics->Velocity.x << physics->Velocity.y << physics->Velocity.z;
            }
            if(i<3){
                //ballen er over på ny trekant
                //kalkulere ny hastighet og posisjon
            physics->Velocity = physics->Velocity - akselerasjon * dt;
            newPos = BallPos + physics->Velocity;
            newPos.z = pos1.z*barCords.x+pos2.z*barCords.y+pos3.z*barCords.z;
            newPos.x -= (pos1.x*barCords.x+pos2.x*barCords.x+pos3.x*barCords.x) * physics->friction;
           // rotspeed -= newPos.x;// * physics->mass;
            mPosition.setPosition(newPos.x, newPos.y, newPos.z+radius);
            //mPosition.rotate(-rotspeed,0,1,0);
            mPosition.rotate(-rotspeed,1,0,0);


            qDebug() << physics->Velocity.x << physics->Velocity.y << physics->Velocity.z;
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
