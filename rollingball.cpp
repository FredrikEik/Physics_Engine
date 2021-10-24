#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(1.5,0,0);
    mScale.scale(0.25,0.25,0.25);
    gKraft = gAkselerasjon*masseIKG;
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{

    mMatrix = mPosition * mScale;
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();




    for (int i=0; i < vertices.size()-2; i+=3){

        gsml::Vector3d v0,v1,v2;
        v0 = gsml::Vector3d(vertices[i].getXYZ());
        v1 = gsml::Vector3d(vertices[i+1].getXYZ());
        v2 = gsml::Vector3d(vertices[i+2].getXYZ());


        gsml::Vector3d playerPos = mPosition.getPosition();

        gsml::Vector3d temp = (1,0,0.55);

        barycentricCord = playerPos.barycentricCoordinates(vertices[i].getXYZ(),vertices[i+1].getXYZ(), vertices[i+2].getXYZ());
       // qDebug() << barycentricCord.x << barycentricCord.y << barycentricCord.z;

        if(barycentricCord.x > 0 && barycentricCord.y > 0 && barycentricCord.z > 0 &&
                barycentricCord.x < 1 && barycentricCord.y < 1 && barycentricCord.z < 1){
            //qDebug() << "you are inside";

            gsml::Vector3d normalvektor =0;
            gsml::Vector3d playerNorm = mPosition.getPosition();
            gsml::Vector3d avstand = 0;
            gsml::Vector3d projeksjon=0;
            gsml::Vector3d newPosition;
            normalvektor = (v1-v0)^(v2-v0);
            normalvektor.normalize();

            akselerasjon = gKraft * normalvektor * normalvektor.z;



            if(i==3){
            hastighet = hastighet + akselerasjon * dt;
            newPosition = playerPos + hastighet;
            newPosition.z = v0.z*barycentricCord.x+v1.z*barycentricCord.y+v2.z*barycentricCord.z;
            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z+radius);
            qDebug() << akselerasjon.x << akselerasjon.y << akselerasjon.z;
            qDebug() << hastighet.x << hastighet.y << hastighet.z;
            }
            else{
            hastighet = hastighet - akselerasjon * dt;
            newPosition = playerPos + hastighet;
            newPosition.z = v0.z*barycentricCord.x+v1.z*barycentricCord.y+v2.z*barycentricCord.z;
            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z+radius);

            }
            //for (int i=0; i < vertices.size()-3; i+=3){
            //playerNorm.normalize();

            //avstand = (temp-vertices[0].getXYZ())*normalvektor;
            //qDebug() << avstand.x << avstand.y << avstand.z;
            //projeksjon = normalvektor*(avstand*normalvektor);
            //qDebug() << projeksjon.x << projeksjon.y << projeksjon.z;


            if (newPosition.z > playerPos.z){
                hastighet = hastighet - akselerasjon * dt;
            }


            //ting som trengs for akselerasjonsvektor og posisjon
            // *******************************************************
            //normalvektor n-> , avstanden til planet y og projeksjonen y->n->= n->*(y->*n->)
            //Avstanden ballen har flyttet seg langs normalvektoren =ds->n-> = n->*(ds->*n->)
            //Tyngdepunktet til ballen ligger enten over eller under planet (y->*n->)/||y->|| = +-1
            //negativt = under, må flytte tilbake posisjon med d=r-y langs normalvektor til planet,
            // må også flytte samme distansen d langs nye hastighetsvektoren


            //}

        }
        else {
           //qDebug() << "You are outside";
        }
    }


//mPosition.translate(0,0,0);

}

void RollingBall::barycentricCoordinates()
{













    // If player is within the bounds of triangle (1/2), else the player is within the bounds of triangle (2/2)
//    if (barycCoords.x() > 0 && barycCoords.y() > 0 && barycCoords.z() > 0){
//        // Calculate player's height at current position
//        // Then turn it into an additive value.
//        // The movement function adds previous position to new one, but I want the height to stay exact (so I "reverse" it here).
//        nextYPos = collidingQuad[2].y() * barycCoords.z() + collidingQuad[1].y() * barycCoords.y() + collidingQuad[0].y() * barycCoords.x();
//        nextYPos -= mtransform->position.y;
//    }

//    else {
//

        // Calculate player's height at current position
        // Then turn it into an additive value.
        // The movement function adds previous position to new one, but I want the height to stay exact (so I "reverse" it here).
        //nextYPos = collidingQuad[5].y() * barycCoords.z() + collidingQuad[4].y() * barycCoords.y() + collidingQuad[3].y() * barycCoords.x();
        //nextYPos -= mtransform->position.y;
//    }

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
