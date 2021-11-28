// xyz.cpp
#include "xyz.h"

XYZ::XYZ() : VisualObject()
{
    mMesh = new Mesh;
    mDrawType = GL_LINES;
    mMesh->mVertices.push_back(gsml::Vertex{0,0,0,1,0,0});
    mMesh->mVertices.push_back(gsml::Vertex{1,0,0,1,0,0});
    mMesh->mVertices.push_back(gsml::Vertex{0,0,0,0,1,0});
    mMesh->mVertices.push_back(gsml::Vertex{0,1,0,0,1,0});
    mMesh->mVertices.push_back(gsml::Vertex{0,0,0,0,0,1});
    mMesh->mVertices.push_back(gsml::Vertex{0,0,1,0,0,1});

    //mMatrix.setToIdentity();
    //mMatrix.frustum(-1,1,-1,1,-1,1);
}

XYZ::~XYZ()
{
    //
}

void XYZ::init(GLint matrixUniform)
{
    //mMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "matrix" );
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mMesh->mVAO );
    glBindVertexArray( mMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO );

    glBufferData( GL_ARRAY_BUFFER, mMesh->mVertices.size()*sizeof( gsml::Vertex ), mMesh->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mMesh->mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( gsml::Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void XYZ::draw()
{
    glBindVertexArray( mMesh->mVAO );
    //glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    //glDrawArrays(GL_LINES, 0, mVertices.size());//mVertices.size());
    //mMatrix.rotate(-1.5f, 1.f, 1.0, 0.f);
    glBindVertexArray(0);
}

