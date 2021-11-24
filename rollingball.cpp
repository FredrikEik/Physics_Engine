#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    p = new Physics;
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    //mPosition.translate(1.5,1.5,3);
    //ph = new Physics;
    mScale.scale(0.25,0.25,0.25);
    //mMatrix = mPosition * mScale;
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

    Translation.z = (HeightVector.z + p->radius);

    if(z != mMatrix.getColumn(3).z())
    {
        setPosition(Translation);
        //mMatrix.setPosition(Translation.x,Translation.y,Translation.z);
    }
}

void RollingBall::move(float dx, float dy, float dz)
{
    mPosition.setPosition(dx, dy, dz);
    mMatrix = mPosition * mScale;
}

void RollingBall::setSurface(VisualObject* surface)
{
    triangle_surface = surface;
    vertices = triangle_surface->get_vertices();
    int mT = static_cast<int>(vertices.size());
    if(vertices.size()>100){
        mT = rand()%mT;
        qDebug() << mT;
        gsml::Vector3d v1 =vertices.at(mT).getXYZ();
        gsml::Vector3d v2 =vertices.at(mT+1).getXYZ();
        gsml::Vector3d v3 =vertices.at(mT+2).getXYZ();
        gsml::Vector3d pos = (v1+v2+v3)*0.333;
        pos.z += 50;
        setPosition(pos);}
    else
        move(1,1,5);
}

void RollingBall::move(float dt)
{
    gsml::Vector3d bary;
    //std::vector<gsml::Vertex> vertices = triangle_surface->get_vertices();
    gsml::Vector2d ballPosXY(Get_position().x, Get_position().y);
    for (size_t i=0; i<vertices.size(); i++)
    {
        //qDebug() << "ground size: " << vertices.size();
        gsml::Vector3d p1 = vertices[i].getXYZ();
        gsml::Vector3d p2 = vertices[++i].getXYZ();
        gsml::Vector3d p3 = vertices[++i].getXYZ();
        //qDebug() << "p1: " << p1.x << p1.y << p1.z << "p2: " << p2.x << p2.y << p2.z << "p3: " << p3.x << p3.y << p3.z;

        m_index = static_cast<int>(i+1) /3;

        bary = barycentricCoords(gsml::Vector2d(p1.x, p1.y),
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

            if(mHeight > p->radius+0.2)
            {
                p->freeFall();
                //qDebug() << "Fritt Fall";
                mN = m_normal;
                mN.normalize();
                m_index = -1;
            }
            else
            {
                p->onGround(m_normal);
                setHeight(barycentricHeight(Get_position(), p1,p2,p3));
                mN = m_normal + old_normal;
                mN.normalize();
            }
            p->Velocity = p->oldVelocity + p->Acceleration * dt;
            mPos = (p->oldVelocity + p->Velocity) * (dt/2);

            mPosition.translate(mPos.x, mPos.y, mPos.z);
            mMatrix = mPosition * mScale;

            if(m_index != old_index)
            {
                p->Velocity = mN * gsml::Vector3d::dot(p->oldVelocity, mN);
                p->Velocity = p->oldVelocity - p->Velocity * 2;
                //p->Velocity = p->Velocity * p->friction;
                //qDebug() << "mVelocity: " << mVelocity.x << mVelocity.y << mVelocity.z;
            }
            p->oldVelocity = p->Velocity;
            old_normal = m_normal;
            old_index = m_index;
            break;
        }
    }
}
