#include "light.h"
#include <QOpenGLFunctions_4_1_Core>
#include "renderwindow.h"
#include "vertex.h"
//using namespace gsls21;

Light::Light()
{
    mVertices.push_back(gsml::Vertex{-0.25f, -0.25f,  0.25f, 0.8f, 0.8f, 0.3f, 0.f , 0.f});
    mVertices.push_back(gsml::Vertex{ 0.25f, -0.25f,  0.25f, 0.8f, 0.8f, 0.3f, 1.f , 0.f});   //Right low
    mVertices.push_back(gsml::Vertex{ 0.0f ,  0.25f,  0.0f , 0.8f, 0.8f, 0.3f, 0.5f, 0.5f});  //Top
    mVertices.push_back(gsml::Vertex{ 0.0f , -0.25f, -0.25f, 0.8f, 0.8f, 0.3f, 0.5f, 0.5f});   //Back low

    mIndices.insert( mIndices.end(),
                     { 0, 1, 2,
                       1, 3, 2,
                       3, 0, 2,
                       0, 3, 1
                     });

    mMatrix.setToIdentity();
}

Light::~Light()
{

}

void Light::init(GLint matrixUniform)
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(gsml::Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( gsml::Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Light::draw()
{
    //    glUseProgram(mMaterial.mShader->getProgram());
    glBindVertexArray( mVAO );
    //    mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}
