#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(1.5,0,0);
    mScale.scale(0.25,0.25,0.25);
    gKraft = gAkselerasjon*mass;
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{

    mMatrix = mPosition * mScale;

    barycentricCords(dt );





//mPosition.translate(0,0,0);

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


            normalvektor = (pos2-pos1)^(pos3-pos1);
            normalvektor.normalize();
          //  normalvektor = gsml::Vector3d::cross(pos3 - pos1,pos2 - pos1);

            akselerasjon = gKraft * normalvektor * normalvektor.z;



            if(i==3){
            hastighet = hastighet + akselerasjon * dt;
            newPosition = BallPosition + hastighet;
            newPosition.z = pos1.z*barCords.x+pos2.z*barCords.y+pos3.z*barCords.z;
            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z+radius);
            qDebug() << akselerasjon.x << akselerasjon.y << akselerasjon.z;
            qDebug() << hastighet.x << hastighet.y << hastighet.z;
            }
            else{
            hastighet = hastighet - akselerasjon * dt;
            newPosition = BallPosition + hastighet;
            newPosition.z = pos1.z*barCords.x+pos2.z*barCords.y+pos3.z*barCords.z;
            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z+radius);

            }

            if (newPosition.z > BallPosition.z){
                hastighet = hastighet - akselerasjon * dt;
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
