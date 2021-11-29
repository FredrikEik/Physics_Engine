// VisualObject.cpp
#include "visualobject.h"

VisualObject::VisualObject()
{

    mMatrix.setToIdentity();
    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
}

VisualObject::~VisualObject()
{
    glDeleteVertexArrays( 1, &mMesh->mVAO );
    glDeleteBuffers( 1, &mMesh->mVBO );
}

void VisualObject::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    //must call this to use OpenGL functions?
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mMesh->mVAO );
    glBindVertexArray( mMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mMesh->mVertices.size()*sizeof( gsml::Vertex ),
                  mMesh->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)0  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void VisualObject::move(float dx, float dy, float dz)
{
   mPosition.setPosition(dx, dy, dz);
}

gsml::Vector3d VisualObject::barycentricCoords(const gsml::Vector2d &p1, const gsml::Vector2d &p2, const gsml::Vector2d &p3, const gsml::Vector2d &position)
{
    gsml::Vector2d p12 = p2-p1;
    gsml::Vector2d p13 = p3-p1;

    //qDebug() << "P12" << p12.x << p12.y << "P13" << p13.x << p13.y;

    gsml::Vector3d n = p12.operator^(p13);

    //qDebug() << "n" << n.x << n.y << n.z;
    float areal_123 = n.length();

    //qDebug() << "areal_123" << areal_123;
    gsml::Vector3d baryc;
    gsml::Vector2d p = p2 - position;
    gsml::Vector2d q = p3 - position;
    n = p.operator^(q);// {0,0,(q.x*p.y)-(q.y*p.x)};
    baryc.x = (n.z / areal_123);

    p = p3 - position;
    q = p1 - position;
    n = p.operator^(q);//n = {0,0,(q.x*p.y)-(q.y*p.x)};
    baryc.y = (n.z / areal_123);

    p = p1 - position;
    q = p2 - position;
    n = p.operator^(q);//n = {0,0,(q.x*p.y)-(q.y*p.x)};
    baryc.z = (n.z / areal_123);

    return baryc;
}

float VisualObject::barycentricHeight(const gsml::Vector3d &point, const gsml::Vector3d &corner1, const gsml::Vector3d &corner2, const gsml::Vector3d &corner3)
{
    // This calculation assumes that height is in the Y axis.
    // We also assume that the point is projected down the Y axis onto the triangle
    // Because of this we flatten the problem down to 2D
    gsml::Vector2d p{point.x, point.y};
    gsml::Vector2d a{corner1.x, corner1.y};
    gsml::Vector2d b{corner2.x, corner2.y};
    gsml::Vector2d c{corner3.x, corner3.y};

    gsml::Vector2d v0 = b - a;
    gsml::Vector2d v1 = c - a;
    gsml::Vector2d v2 = p - a;


    float d00 = (v0.x * v0.x +  v0.y * v0.y);
    float d01 = (v0.x * v1.x +  v0.y * v1.y);
    float d11 = (v1.x * v1.x +  v1.y * v1.y);
    float d20 = (v2.x * v0.x +  v2.y * v0.y);
    float d21 = (v2.x * v1.x +  v2.y * v1.y);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);

    // u, v, w are the barycentric coordinates
    float v = (d11 * d20 - d01 * d21) * invDenom;
    float w = (d00 * d21 - d01 * d20) * invDenom;
    float u = 1.0f - v - w;

    //        qDebug() << "BaryCords:" << u << v << w;

    float heightOut;
    if(u >= 0.f && v >= 0.f && w >= 0.f )
        heightOut = corner1.z * u + corner2.z * v + corner3.z * w;
    else
        heightOut = -10000.f;

    return heightOut;
}

std::vector<gsml::Vertex> VisualObject::get_vertices()
{
    return mMesh->mVertices;
}
