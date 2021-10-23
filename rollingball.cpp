#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(1,0,0.55);
    mScale.scale(0.25,0.25,0.25);
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{

    mMatrix = mPosition * mScale;
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();




    for (int i=0; i < vertices.size()-3; i+=3){


        auto playerPos = mPosition.getPosition();

        barycentricCord = playerPos.barycentricCoordinates(vertices[i].getXYZ(),vertices[i+1].getXYZ(), vertices[i+2].getXYZ());


        if(barycentricCord.x > 0 && barycentricCord.y > 0){
            qDebug() << "you are inside";
        }
    }

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
//        // ------------------------------------- BARYCENTRIC COORDINATES FOR TRIANGLE (2/2) -------------------------------------
//        a = QVector2D(collidingQuad[3].x(), collidingQuad[3].z());
//        b = QVector2D(collidingQuad[4].x(), collidingQuad[4].z());
//        c = QVector2D(collidingQuad[5].x(), collidingQuad[5].z());

//        QVector2D ab = b - a;
//        QVector2D ac = c - a;
//        QVector3D n = QVector3D::crossProduct(ab.toVector3D(), ac.toVector3D());
//        area = n.length();

//        QVector2D p = b - playerPos;
//        QVector2D q = c - playerPos;
//        n = QVector3D::crossProduct(p.toVector3D(), q.toVector3D());
//        barycCoords.setX(n.z() / area);

//        p = c - playerPos;
//        q = a - playerPos;
//        n = QVector3D::crossProduct(p.toVector3D(), q.toVector3D());
//        barycCoords.setY(n.z() / area);

//        p = a - playerPos;
//        q = b - playerPos;
//        n = QVector3D::crossProduct(p.toVector3D(), q.toVector3D());
//        barycCoords.setZ(n.z() / area);
        // ---------------------------------------------------------------------------------------------------------------------

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
