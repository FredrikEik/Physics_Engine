#include "rollingball.h"

RollingBall::RollingBall(int dID) : VisualObject()
{
    std::string sID = std::to_string(dID);
    mTxt = mTxt + sID;
    mTxt = mTxt + ".txt";
    p = new Physics;
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    //mPosition.translate(1.5,1.5,3);
    mScale.scale(0.1,0.1,0.1);
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
    glGenVertexArrays( 1, &mMesh->mVAO );
    glBindVertexArray( mMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO );

    glBufferData( GL_ARRAY_BUFFER, mMesh->mVertices.size()*sizeof(gsml::Vertex), mMesh->mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mMesh->mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void RollingBall::draw()
{
    glBindVertexArray( mMesh->mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mMesh->mVertices.size());//mVertices.size());
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
    }
}

void RollingBall::setMesh(Mesh* uMesh)
{
    mMesh = uMesh;
}

void RollingBall::saveRoute(std::string filnavn)
{
    std::ofstream ut;
    ut.open(filnavn.c_str());

    if (ut.is_open())
    {
        auto n = mbsPoints.size();
        gsml::Vector3d temp;
        ut << n << std::endl;

        for (auto it=mbsPoints.begin(); it != mbsPoints.end(); it++)
        {
            temp = *it;
            ut << temp.x << std::endl;
            ut << temp.y << std::endl;
            ut << temp.z << std::endl;
        }
        ut.close();
    }
}

void RollingBall::constructBSpline(gsml::Vector3d dP)
{
    mbsPoints.push_back(dP);
    if(mbsPoints.size() == 9){
        saveRoute(mTxt);
        qDebug() << "Route saved";}
}

void RollingBall::move(float dx, float dy, float dz)
{
    mPosition.setPosition(dx, dy, dz);
    mMatrix = mPosition * mScale;
}

void RollingBall::setSurface(VisualObject* surface)
{
    triangle_surface = surface;
    surfVertices = triangle_surface->get_vertices();
    int mT = static_cast<int>(surfVertices.size());
    if(surfVertices.size()>100){ // only do this for big surface
        mT = rand()%mT;
        qDebug() << mT;
        gsml::Vector3d v1 =surfVertices.at(mT).getXYZ();
        gsml::Vector3d v2 =surfVertices.at(mT+1).getXYZ();
        gsml::Vector3d v3 =surfVertices.at(mT+2).getXYZ();
        gsml::Vector3d pos = (v1+v2+v3)*0.333;
        pos.z += 5;
        setPosition(pos);}
    else
        move(1,1,5);
}

void RollingBall::move(float dt)
{
    gsml::Vector3d bary;
    //std::vector<gsml::Vertex> vertices = triangle_surface->get_vertices();
    gsml::Vector2d ballPosXY(Get_position().x, Get_position().y);
    for (size_t i=0; i<surfVertices.size(); i++)
    {
        gsml::Vector3d p1 = surfVertices[i].getXYZ();
        gsml::Vector3d p2 = surfVertices[++i].getXYZ();
        gsml::Vector3d p3 = surfVertices[++i].getXYZ();

        m_index = static_cast<int>(i+1) /3;

        bary = barycentricCoords(gsml::Vector2d(p1.x, p1.y),
                                 gsml::Vector2d(p2.x, p2.y),
                                 gsml::Vector2d(p3.x, p3.y),
                                 ballPosXY);

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
            if(mHeight > p->radius+0.2)
                isFalling = true;
            else
                isFalling = false;


            if(isFalling)
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


            if(m_index != old_index)
            {
                if(!isFalling)
                    constructBSpline(Get_position());

                float speed = p->Velocity.length();
                gsml::Vector3d mVector;
                float dot = gsml::Vector3d::dot(p->Velocity, mN);
                mVector = p->Velocity - mN * (2.0f * dot);
                mVector.normalize();
                p->Velocity = mVector * speed;

            }

            p->Velocity = p->oldVelocity + p->Acceleration * dt;
            mPos = p->Velocity * dt;
            mPos = mPos + p->Acceleration * dt * dt * 0.5;

            mPosition.translate(mPos.x, mPos.y, mPos.z);
            mMatrix = mPosition * mScale;


            p->oldVelocity = p->Velocity;
            old_normal = m_normal;
            old_index = m_index;
            break;
        }
    }
}

