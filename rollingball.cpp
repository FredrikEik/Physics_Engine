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
void RollingBall::baryMove(float x, float y, float z)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    if (z==0)
    {
        mMatrix.translate(x, 0, 0);
        mMatrix.translate(0, y, 0);

        gsml::Vector3d bary;
        qDebug() << "column x: " << mMatrix.getColumn(3).x();
        qDebug() << "column y: " << mMatrix.getColumn(3).y();
        for (size_t i=0; i<mVertices.size(); i++)
        {
            qDebug() << "ground size: " << vertices.size();
            gsml::Vector3d p1 = vertices[i].getXYZ();
            gsml::Vector3d p2 = vertices[++i].getXYZ();
            gsml::Vector3d p3 = vertices[++i].getXYZ();

            bary = barycentricCoords(gsml::Vector2d(p1.x, p1.y),
                                     gsml::Vector2d(p2.x, p2.y),
                                     gsml::Vector2d(p3.x, p3.y),
                                     gsml::Vector2d(mMatrix.getColumn(3).x(), mMatrix.getColumn(3).y()));
            //qDebug() << "bary value: " << bary;

            if (bary.x >=0 && bary.y >=0 && bary.z >=0)
            {
                gsml::Vector3d translation = {(0),(0), (p1.z * bary.x) + (p2.z * bary.y) + (p3.z * bary.z)};
                //qDebug() << "translating" << translation;
                mMatrix.translate(0,0, (p1.z * bary.x) + (p2.z * bary.y) + (p3.z * bary.z));
                break;
            }
        }
    }
    else
    {
        mMatrix.translate(0,0,z);
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

gsml::Vector3d RollingBall::Get_position()
{
    gsml::Vector3d temp;

    temp.x = (mMatrix.getColumn(3).x());
    temp.y = (mMatrix.getColumn(3).y());
    temp.z = (mMatrix.getColumn(3).z());

    return temp;
}

void RollingBall::setHeight(float z)
{
    QVector3D HeightVector{0,0,z};
    QVector3D Translation{0,0,0};

    Translation.setZ((HeightVector.z() - Get_position().z)+0.5);

    if(z != mMatrix.getColumn(3).z())
    {
        mMatrix.translate(Translation.x(),Translation.y(),Translation.z());
    }
}

void RollingBall::heightAt()
{
    gsml::Vector3d pos = Get_position();

    baryMove(pos.x,pos.y,pos.z);
}

void RollingBall::move(float dt)
{
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();
    mMatrix = mPosition * mScale;
}
