#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    mPosition.translate(1.5,1.5,3);
    mScale.scale(0.25,0.25,0.25);

}
RollingBall::~RollingBall()
{

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

    // +0.25 for å få ball på flaten
    Translation.z = (HeightVector.z +0.25);

    if(z != mMatrix.getColumn(3).z())
    {
        mMatrix.setPosition(Translation.x,Translation.y,Translation.z);
    }
}


void RollingBall::move(float dt)
{
    gsml::Vector3d bary;
    std::vector<gsml::Vertex> vertices = triangle_surface->get_vertices();
    gsml::Vector2d ballPosXY(Get_position().x, Get_position().y);
    for (size_t i=0; i<vertices.size(); i++)
    {
     // indekser til flaten
        if(i<=2)
            m_index = 1;
        else
            m_index = 2;

        // Finn trekantens vertices v0 , v1 , v2
        gsml::Vector3d p1 = vertices[i].getXYZ();
        gsml::Vector3d p2 = vertices[++i].getXYZ();
        gsml::Vector3d p3 = vertices[++i].getXYZ();

        //        // Finn ballens posisjon i xy=planet
        //        // Soek etter triangel som ballen er pa na
        //        // med barysentriske koordinater
        bary = barycentricCoords(gsml::Vector2d(p1.x, p1.y),
                                 gsml::Vector2d(p2.x, p2.y),
                                 gsml::Vector2d(p3.x, p3.y),
                                 ballPosXY);

      //if( /* barysentriske koordinater mellom 0 og 1 */ )
        if (bary.x >=0 && bary.y >=0 && bary.z >=0)
        {
            // beregne normal
            gsml::Vector3d mPos;
            gsml::Vector3d p12 = p2-p1;
            gsml::Vector3d p13 = p3-p1;
            m = p12^p13;
            m.normalize();


            // beregn akselerasjonsvektor = ligning(7)
            // Oppdaterer hastighet og posisjon
            mAcceleration = gsml::Vector3d(m.x * m.z, m.y * m.z, (m.z*m.z)-1) * lilleG;
            mVelocity = mVelocity + mAcceleration * dt;

            mPos = (oldVelocity + mVelocity) * (dt/2);
            mPosition.translate(mPos.x, mPos.y, mPos.z);
            mMatrix = mPosition * mScale;

            // Setter høyde, så ballen følger planet
            setHeight(barycentricHeight(Get_position(), p1,p2,p3));

            // if( /* ny indeks != forrige indeks */ )
            if(m_index != old_index)
            {
                // Ballen har rullet over pa nytt triangel
                // Beregner normalen til kollisjonsplanet,
                // se ligning(9)
                m_normal = m + old_normal;
                m_normal.normalize();

                // Korrigere posisjon oppover i normalens retning
                // Oppdater hastighetsvektoren, se ligning(8)
                // Oppdatere posisjon i retning den nye
                // hastighetsvektoren
                mVelocity = m_normal * gsml::Vector3d::dot(oldVelocity, m_normal);
                mVelocity = oldVelocity - mVelocity * 2;
                mVelocity = mVelocity * friction;
                qDebug() << "mVelocity: " << mVelocity.x << mVelocity.y << mVelocity.z;
            }
            // Oppdater gammel normal og indeks
            oldVelocity = mVelocity;
            old_index = m_index;
            old_normal = m_normal;

            break;
        }
    }

}
