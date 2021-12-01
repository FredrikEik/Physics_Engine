#include "marchingsquare.h"

MarchingSquare::MarchingSquare(VisualObject* mSurf)
{
    surface = mSurf;
    mMatrix.setToIdentity();
    mShader = 1;
    mMesh = new Mesh;

}

MarchingSquare::~MarchingSquare()
{

}

void MarchingSquare::init(GLint matrixUniform)
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

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mMesh->mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);


    glBindVertexArray(0);

}

void MarchingSquare::draw()
{
    glBindVertexArray( mMesh->mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_LINE_STRIP, 0, mMesh->mVertices.size());
}

void MarchingSquare::setSquares()
{
//    VisualObject* mSurf = static_cast<FlateFil*>(surface);
//    for(float i = 0; i<mSurf->getX(); i++)
//        for(float j=0;j<mSurf->getY(); j++)
//        {
//            int rez = 1;
//            float x = i * rez;
//            float y = j * rez;

//            gsml::Vector2d a(x + rez * .5,	y           );
//            gsml::Vector2d b(x + rez     ,	y + rez * .5);
//            gsml::Vector2d c(x + rez * .5,	y + rez     );
//            gsml::Vector2d d(x           ,	y + rez * .5);

//            int state = getState(fields[i][j], fields[i + 1][j], fields[i + 1][j + 1], fields[i][j + 1]);
//            switch (state)
//                        {
//                        case 1:
//                            drawLines(c, d);
//                            break;
//                        case 2:
//                            drawLines(b, c);
//                            break;
//                        case 3:
//                            drawLines(b, d);
//                            break;
//                        case 4:
//                            drawLines(a, b);
//                            break;
//                        case 5:
//                            drawLines(a, d);
//                            drawLines(b, c);
//                            break;
//                        case 6:
//                            drawLines(a, c);
//                            break;
//                        case 7:
//                            drawLines(a, d);
//                            break;
//                        case 8:
//                            drawLines(a, d);
//                            break;
//                        case 9:
//                            drawLines(a, c);
//                            break;
//                        case 10:
//                            drawLines(a, b);
//                            drawLines(c, d);
//                            break;
//                        case 11:
//                            drawLines(a, b);
//                            break;
//                        case 12:
//                            drawLines(b, d);
//                            break;
//                        case 13:
//                            drawLines(b, c);
//                            break;
//                        case 14:
//                            drawLines(c, d);
//                            break;
//                        }
       //}
}

int MarchingSquare::getState(int a, int b, int c, int d)
{
    return a * 8 + b * 4 + c * 2 + d * 1;
}

void MarchingSquare::drawLines(gsml::Vector2d v1, gsml::Vector2d v2)
{

}
