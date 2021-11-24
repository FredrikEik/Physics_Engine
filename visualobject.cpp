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
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}
void VisualObject::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    //must call this to use OpenGL functions?
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( gsml::Vertex ),
                  mVertices.data(), GL_STATIC_DRAW );

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
gsml::Vector3d VisualObject::baryCoord(const gsml::Vector2d &p1, const gsml::Vector2d &p2, const gsml::Vector2d &p3, const gsml::Vector2d &pos)
{
    gsml::Vector2d p12 = p2 - p1;
    gsml::Vector2d p13 = p3 - p1;
    gsml::Vector3d n = {0.f, 0.f, (p13.x * p12.y) - (p13.y * p12.x)};
    float A = n.length();

    //u
    gsml::Vector2d p = p2 - pos;
    gsml::Vector2d q = p3 - pos;
    gsml::Vector3d n1 = {0.f, 0.f, (q.x * p.y) - (q.y * p.x)};

    //v
    p = p3 - pos;
    q = p1 - pos;
    gsml::Vector3d n2 = {0.f, 0.f, (q.x * p.y) - (q.y * p.x)};

    //w
    p = p1 - pos;
    q = p2 - pos;
    gsml::Vector3d n3 = {0.f, 0.f, (q.x * p.y) - (q.y * p.x)};

    return gsml::Vector3d{n1.z/A, n2.z/A, n3.z/A};
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
    return mVertices;
}

