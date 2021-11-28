#include "rollingball.h"
#include "las.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(1.5,0,2);
    mScale.scale(0.2,0.2,0.2);
    gKraft = gForce*mass;
    //gForce = gAcceleration * massInKg;
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


    float rotspeed;
    gsml::Vector3d normalvektor = 0;
    gsml::Vector3d newPos = 0;
    std::vector<gsml::Vertex>& vertices = dynamic_cast<LAS*>(_las)->get_vertices();

    for (unsigned int i=0; i < vertices.size(); i+=3){

        gsml::Vector3d pos1;
        gsml::Vector3d pos2;
        gsml::Vector3d pos3;
        pos1 = gsml::Vector3d(vertices[i].getXYZ());
        pos2 = gsml::Vector3d(vertices[i+1].getXYZ());
        pos3 = gsml::Vector3d(vertices[i+2].getXYZ());

        gsml::Vector3d BallPos = mPosition.getPosition();
        gsml::Vector3d barCords = BallPos.barycentricCoordinates(vertices[i].getXYZ(),
                                                  vertices[i+1].getXYZ(), vertices[i+2].getXYZ());

        if(barCords.x >= 0 && barCords.y >= 0 && barCords.z >= 0){


            normalvektor = gsml::Vector3d::cross(pos3 - pos1,pos2 - pos1);
            normalvektor.normalize();

            old_normal = normalvektor;



            akselerasjon = gKraft * normalvektor * normalvektor.z;

            if(i==3){


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
