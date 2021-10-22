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
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    mMatrix = mPosition * mScale;

}

void RollingBall::calculateBarycentricCoordinates(VisualObject* surface)
{
    bool isInTriangle = false;
    gsml::Vector3d normal{0};
    gsml::Vector3d playerTempPos{0};
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    //find normal vector
    for (unsigned int i=0; i<surface->mVertices.size(); i+=3)
    {
        gsml::Vector3d pos1;
        gsml::Vector3d pos2;
        gsml::Vector3d pos3;
        pos1 = surface->mVertices[surface->mIndices[i+0]].mXYZ;
        pos2 = surface->mVertices[surface->mIndices[i+1]].mXYZ;
        pos3 = surface->mVertices[surface->mIndices[i+2]].mXYZ;

        gsml::Vector2d temp = gsml::Vector2d(mMatrix.getPosition().x, mMatrix.getPosition().z);
        gsml::Vector3d bar = temp.barycentricCoordinates(gsml::Vector2d(pos1.x,pos1.z),gsml::Vector2d(pos2.x, pos2.z), gsml::Vector2d(pos3.x,pos3.z));

        if(bar.x>=0 && bar.x<=1 && bar.y>=0 && bar.y<=1 && bar.z>=0 && bar.z <=1)
        {
            isInTriangle = true;
            playerTempPos = (pos1*bar.x + pos2*bar.y + pos3*bar.z);
            normal = gsml::Vector3d::cross(pos3 - pos1,pos2 - pos1);
            normal.normalize();
        }
    }

    //the formula is actually N = |G| * n * cos a
    //by taking dot(-G*n), we save time and get |G|* cos a in one swoop.
    //As gravity will always be the exact opposite direction of the xz-plane's normal vector,
    //this shouldnt be a problem
    gsml::Vector3d N;
    N = normal* gsml::Vector3d::dot(gravity * -1, normal);

    gsml::Vector3d vectorToBall =  (mMatrix.getPosition()-playerTempPos);
    float distanceToBall = gsml::Vector3d::dot(vectorToBall,normal);

    if(distanceToBall>radius)
    {
        normal = gsml::Vector3d(0);
        N = gsml::Vector3d(0);
    }
    else
    {
        float distance = radius - distanceToBall;
        if(distance >0.5f)
        {

            mMatrix.translate(normal*distance);
        }
        //qDebug() << distance;

    }
    if(normal != prevTriangleNormal)
    {
        //qDebug() << "Same Normals!";
        if(normal == gsml::Vector3d(0)) //går til lufta
        {
            qDebug() << "Leaving Triangls!";
            //N = gsl::Vector3D(0);
        }
        else if(prevTriangleNormal== gsml::Vector3d(0))//kommer fra lufta
        {
            velocity = (gravity+N).normalized() * gsml::Vector3d::dot(velocity, (gravity+N).normalized());
        }
        else    //bytter trekant
        {
            //qDebug() << "Swapping Triangle!";
            gsml::Vector3d tempNormal = normal + prevTriangleNormal;
            tempNormal.normalize();
            gsml::Vector3d tempVel = tempNormal*gsml::Vector3d::dot(velocity,tempNormal);
            tempVel= velocity - tempVel*2;
            velocity = tempVel;
        }
    }

    prevTriangleNormal = normal;

        //LastLocation = gsl::Vector3D(mMatrix.getPosition().x,playerTempPos,mMatrix.getPosition().z);
    //qDebug() << prevTriangleNormal << normal <<  radius << distanceToBall;
        //qDebug() << acceleration << velocity.normalized() << (gravity+N).normalized() << currentTriangleNormal << gsl::Vector3D::dot(gravity, currentTriangleNormal);

    //(1/m)* (N+G);
    acceleration = (N+gravity);
    velocity+=acceleration*speed;
    //mMatrix.setPosition(LastLocation);
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
