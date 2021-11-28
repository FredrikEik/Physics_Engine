#include "light.h"
#include "renderwindow.h"

Light::Light()
{
    mMesh = new Mesh;
    mMesh->mVertices.push_back(gsml::Vertex{-0.25f, -0.25f,  0.25f, 0.8f, 0.8f, 0.3f, 0.f , 0.f});
    mMesh->mVertices.push_back(gsml::Vertex{ 0.25f, -0.25f,  0.25f, 0.8f, 0.8f, 0.3f, 1.f , 0.f});   //Right low
    mMesh->mVertices.push_back(gsml::Vertex{ 0.0f ,  0.25f,  0.0f , 0.8f, 0.8f, 0.3f, 0.5f, 0.5f});  //Top
    mMesh->mVertices.push_back(gsml::Vertex{ 0.0f , -0.25f, -0.25f, 0.8f, 0.8f, 0.3f, 0.5f, 0.5f});   //Back low

    mMesh->mIndices.insert( mMesh->mIndices.end(),
    { 0, 1, 2,
      1, 3, 2,
      3, 0, 2,
      0, 3, 1
    });

    mMatrix.setToIdentity();
}

void Light::init(GLint matrixUniform)
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mMesh->mVAO );
    glBindVertexArray( mMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO );

    glBufferData( GL_ARRAY_BUFFER, mMesh->mVertices.size()*sizeof(gsml::Vertex), mMesh->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mMesh->mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( gsml::Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mMesh->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mMesh->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->mIndices.size() * sizeof(GLuint), mMesh->mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Light::draw()
{
    glBindVertexArray( mMesh->mVAO );
    glDrawElements(GL_TRIANGLES, mMesh->mIndices.size(), GL_UNSIGNED_INT, nullptr);
}
