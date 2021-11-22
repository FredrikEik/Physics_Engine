#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(1.5,1.5,2);
    mScale.scale(0.25,0.25,0.25);
    gKraft = gAkselerasjon*masseIKG;        //regner ut gkraften til ballen
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{

    mMatrix = mPosition * mScale;
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();             //skaffer vertices
    for (int i=0; i < vertices.size()-2; i+=3){
        v0 = gsml::Vector3d(vertices[i].getXYZ());
        v1 = gsml::Vector3d(vertices[i+1].getXYZ());
        v2 = gsml::Vector3d(vertices[i+2].getXYZ());

        gsml::Vector3d playerPos = mPosition.getPosition();                 //skaffer posisjonen til ballen

        barycentricCord = playerPos.barycentricCoordinates(
                    vertices[i].getXYZ(),vertices[i+1].getXYZ(), vertices[i+2].getXYZ());  //kalkulerer barisentriske kordinater

        if(barycentricCord.x > 0 && barycentricCord.y > 0 && barycentricCord.z > 0 &&
                barycentricCord.x < 1 && barycentricCord.y < 1 && barycentricCord.z < 1){   //sjekker at ballen er innafor trianglene
            //qDebug() << "The ball is inside";
            //gsml::Vector3d avstand = 0;
            //gsml::Vector3d projeksjon=0;
            //gsml::Vector3d distanseFlyttet =0;
            //gsml::Vector3d distanseFlyttetNM =0;
            //gsml::Vector3d temp = (1,0,0.55);
            //qDebug() << i;

            normalvektor = (v1-v0)^(v2-v0);                                             //regner ut normalvektoren til planet
            normalvektor.normalize();                                                   //normaliserer normalvektoren
            akselerasjon = gKraft ^ normalvektor ^ gsml::Vector3d (0,0,normalvektor.z); //regner ut akselerasjon
            hastighet = hastighet + akselerasjon * dt;                                  //regner ut hastighet

            //if(i==3){
            nyPosisjon = playerPos + hastighet;                                         //oppdaterer posisjonen
            nyPosisjon.z = v0.z*barycentricCord.x+v1.z*barycentricCord.y+v2.z*barycentricCord.z;   //bruker barysentriske kordinatene til å bestemme nye z posisjonen
            mPosition.setPosition(nyPosisjon.x, nyPosisjon.y, nyPosisjon.z+radius);      //setter den nye posisjonen, plusser også på radiusen på z'en til ballen så den ligger oppå planet
           // }

//            else if (i==0){
//            newPosition = playerPos + hastighet* dt;
//            newPosition.z = v0.z*barycentricCord.x+v1.z*barycentricCord.y+v2.z*barycentricCord.z;
//            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z+radius);
//            }

                    //work in progress
            //avstand = (temp-vertices[0].getXYZ())^normalvektor;
            //projeksjon = normalvektor^(avstand^normalvektor);
            //distanseFlyttet = hastighet * dt;
            //distanseFlyttetNM = normalvektor^(distanseFlyttet ^normalvektor);
            //mPosition.setPosition(hastighet.x, hastighet.y, nyPosisjon.z+radius);

                    //Test greier
            //qDebug() << pow(normalvektor.z,2)-1;
            //qDebug() << nyPosisjon.z;
            //qDebug() << distanseFlyttet.x << distanseFlyttet.y << distanseFlyttet.z;
            //qDebug() << distanseFlyttetNM.x << distanseFlyttetNM.y << distanseFlyttetNM.z;
           // qDebug() << avstand.x << avstand.y << avstand.z;
           // qDebug() << projeksjon.x << projeksjon.y << projeksjon.z;

            //ting som trengs for akselerasjonsvektor og posisjon
            // *******************************************************
            //normalvektor n-> , avstanden til planet y og projeksjonen y->n->= n->*(y->*n->)
            //Avstanden ballen har flyttet seg langs normalvektoren =ds->n-> = n->*(ds->*n->)
            //Tyngdepunktet til ballen ligger enten over eller under planet (y->*n->)/||y->|| = +-1
            //negativt = under, må flytte tilbake posisjon med d=r-y langs normalvektor til planet,
            // må også flytte samme distansen d langs nye hastighetsvektoren
        }
        else if (barycentricCord.x < 0 && barycentricCord.y < 0 && barycentricCord.z < 0 &&     //sjekker at ballen ikke er innenfor trianglene
                 barycentricCord.x > 1 && barycentricCord.y > 1 && barycentricCord.z > 1) {
           qDebug() << "The ball is outside";
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
