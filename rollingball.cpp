#include "rollingball.h"
#include "QDebug"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.setPosition(1,0,1.25);
    mScale.scale(0.25,0.25,0.25);
    gForce = gAcceleration * massKG;
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();



    gsml::Vector3d BaryCord;
    gsml::Vector3d BallPosition = mPosition.getPosition();

    for(unsigned long long i = 0; i < vertices.size() - 2; i+=3)
    {
        gsml::Vector3d p1, p2, p3;
        p1 = gsml::Vector3d(vertices[i].getXYZ());
        p2 = gsml::Vector3d(vertices[i+1].getXYZ());
        p3 = gsml::Vector3d(vertices[i+2].getXYZ());

        BaryCord = BallPosition.barycentricCoordinates(p1, p2, p3);

        if(BaryCord.x >= 0 && BaryCord.y >= 0 && BaryCord.z >= 0)
        {
            gsml::Vector3d p12     = p2-p1;
            gsml::Vector3d p13     = p3-p1;
            gsml::Vector3d pNormal = p12^p13;
            pNormal.normalize();

            /*gForce.x = abs(gForce.x);
            gForce.y = abs(gForce.y);
            gForce.z = abs(gForce.z);*/

            acceleration = gForce * pNormal * pNormal.z;

            if(i!=0)
            {
                velocity = velocity + acceleration * dt;
            }


            if(i == 0)
            {
                velocity = velocity - acceleration * dt;
            }

            gsml::Vector3d newPosition = mPosition.getPosition() + velocity*dt;
            newPosition.z = p1.z*BaryCord.x + p2.z*BaryCord.y + p3.z*BaryCord.z;
            mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z+0.25);

            BallPosition = mPosition.getPosition();
            qDebug() << "BallPosition: " << BallPosition.x << BallPosition.y << BallPosition.z;
        }
    }


    mMatrix = mPosition * mScale;


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

void RollingBall::barycentricCoordinates()
{
  /*  // Barysentriske koordinater ble gjort av Eivind Hobrad Naasen
    // barycentricCoordinates() will calculate the next y-coordinate out from current playerPos.
    // So the player will always be one step ahead in the y-direction.
    QVector2D a, b, c;
    QVector3D barycCoords;
    float area{};
    QVector3D collidingQuad[6] {};
    //const Heightmap* surfaceRef = static_cast<Heightmap*>(mMap["heightmap"]);
    const QVector2D playerPos =
    int indexOffset{};

    // 0.35f is 'h' | '20' is the amount of quads in negative x- and y-direction respectively (abs(-7.0 / 0.35)) |
    // '240' is the amount of vertices per change in x-direction (6 * (abs(-7.0) + 7.0) / 0.35f) | '6' is the amount of vertices per change in z-direction
    indexOffset = ((floorf(playerPos.x() / 0.35f) + 20) * 240) + ((floorf(playerPos.y() / 0.35f) + 20) * 6);

    // Fill up 'collidingQuad[]'
    int j{0};
    for (int i{indexOffset}; i < indexOffset + 6; i++){
        collidingQuad[j] = QVector3D(surfaceRef->mVertices[i].m_xyz[0], surfaceRef->mVertices[i].m_xyz[1], surfaceRef->mVertices[i].m_xyz[2]);
        j += 1;
    }

    // ------------------------------------- BARYCENTRIC COORDINATES FOR TRIANGLE (1/2) -------------------------------------
    a = QVector2D(collidingQuad[0].x(), collidingQuad[0].z());
    b = QVector2D(collidingQuad[1].x(), collidingQuad[1].z());
    c = QVector2D(collidingQuad[2].x(), collidingQuad[2].z());

    QVector2D ab = b - a;
    QVector2D ac = c - a;
    QVector3D n = QVector3D::crossProduct(ab, ac);
    area = n.length();

    QVector2D p = b - playerPos;
    QVector2D q = c - playerPos;
    n = QVector3D::crossProduct(p, q);
    barycCoords.setX(n.z() / area);

    p = c - playerPos;
    q = a - playerPos;
    n = QVector3D::crossProduct(p, q);
    barycCoords.setY(n.z() / area);

    p = a - playerPos;
    q = b - playerPos;
    n = QVector3D::crossProduct(p, q);
    barycCoords.setZ(n.z() / area);
    // ---------------------------------------------------------------------------------------------------------------------

    // If player is within the bounds of triangle (1/2), else the player is within the bounds of triangle (2/2)
    if (barycCoords.x() > 0 && barycCoords.y() > 0 && barycCoords.z() > 0){
        // Calculate player's height at current position
        // Then turn it into an additive value.
        // The movement function adds previous position to new one, but I want the height to stay exact (so I "reverse" it here).
        nextYPos = collidingQuad[2].y() * barycCoords.z() + collidingQuad[1].y() * barycCoords.y() + collidingQuad[0].y() * barycCoords.x();
        nextYPos -= static_cast<Cube*>(mMap["cube"])->mPosition.column(3).y();
    } else {

        // ------------------------------------- BARYCENTRIC COORDINATES FOR TRIANGLE (2/2) -------------------------------------
        a = QVector2D(collidingQuad[3].x(), collidingQuad[3].z());
        b = QVector2D(collidingQuad[4].x(), collidingQuad[4].z());
        c = QVector2D(collidingQuad[5].x(), collidingQuad[5].z());

        QVector2D ab = b - a;
        QVector2D ac = c - a;
        QVector3D n = QVector3D::crossProduct(ab, ac);
        area = n.length();

        QVector2D p = b - playerPos;
        QVector2D q = c - playerPos;
        n = QVector3D::crossProduct(p, q);
        barycCoords.setX(n.z() / area);

        p = c - playerPos;
        q = a - playerPos;
        n = QVector3D::crossProduct(p, q);
        barycCoords.setY(n.z() / area);

        p = a - playerPos;
        q = b - playerPos;
        n = QVector3D::crossProduct(p, q);
        barycCoords.setZ(n.z() / area);
        // ---------------------------------------------------------------------------------------------------------------------

        // Calculate player's height at current position
        // Then turn it into an additive value.
        // The movement function adds previous position to new one, but I want the height to stay exact (so I "reverse" it here).
        nextYPos = collidingQuad[5].y() * barycCoords.z() + collidingQuad[4].y() * barycCoords.y() + collidingQuad[3].y() * barycCoords.x();
        nextYPos -= static_cast<Cube*>(mMap["cube"])->mPosition.column(3).y();
    }*/

}

void RollingBall::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
}
