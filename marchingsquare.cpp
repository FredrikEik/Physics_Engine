#include "marchingsquare.h"

MarchingSquare::MarchingSquare(VisualObject* mSurf)
{
    surface = mSurf;
    mMatrix.setToIdentity();
    mShader = 0;
    mMesh = new Mesh;
    mMesh->mDrawType = GL_LINES;
    for(int i =0; i<nrOfFloors; i++)
        setSquares();




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


void MarchingSquare::setSquares()
{
    FlateFil* mSurf = static_cast<FlateFil*>(surface);
    for(int i{0}; i<mSurf->getX(); i++){
        for(int j{0}; j<mSurf->getY(); j++)
        {
            int rez = 1;
            float x = i;
            float y = j;
            //float z = (mSurf->mHPoints[i][j] + mSurf->mHPoints[i+1][j])/2;
            gsml::Vector2d a {x + rez*0.5f, y           };
            gsml::Vector2d b {x + rez     , y + rez*0.5f};
            gsml::Vector2d c {x + rez*0.5f, y + rez     };
            gsml::Vector2d d {x           , y + rez*0.5f};
            float aH, bH, cH, dH;
            if(mSurf->mHPoints[i][j] < iso * floor)
                aH = 0;
            else
                aH = 1;
            if(mSurf->mHPoints[i+1][j] < iso* floor)
                bH = 0;
            else
                bH = 1;
            if(mSurf->mHPoints[i+1][j+1] < iso* floor)
                cH = 0;
            else
                cH = 1;
            if(mSurf->mHPoints[i][j+1] < iso* floor)
                dH = 0;
            else
                dH = 1;


            int state = getState(aH, bH, cH, dH);

            switch (state)
            {
            case 1:
                drawLine(c, d);
                break;
            case 2:
                drawLine(b, c);
                break;
            case 3:
                drawLine(b, d);
                break;
            case 4:
                drawLine(a, b);
                break;
            case 5:
                drawLine(a, d);
                drawLine(b, c);
                break;
            case 6:
                drawLine(a, c);
                break;
            case 7:
                drawLine(a, d);
                break;
            case 8:
                drawLine(a, d);
                break;
            case 9:
                drawLine(a, c);
                break;
            case 10:
                drawLine(a, b);
                drawLine(c, d);
                break;
            case 11:
                drawLine(a, b);
                break;
            case 12:
                drawLine(b, d);
                break;
            case 13:
                drawLine(b, c);
                break;
            case 14:
                drawLine(c, d);
                break;
            }
        }}
    floor++;
}
int MarchingSquare::getState(int a, int b, int c, int d)
{
  return a*8 + b*4 + c*2 + d*1;
}

void MarchingSquare::drawLine(gsml::Vector2d a, gsml::Vector2d b)
{
    mMesh->mVertices.push_back(gsml::Vertex{a.x, a.y,static_cast<float>(iso*floor) + 0.1f, static_cast<float>(1-1/floor*iso),0,static_cast<float>(0.05*floor*iso), 0,0});
    mMesh->mVertices.push_back(gsml::Vertex{b.x, b.y,static_cast<float>(iso*floor) + 0.1f, static_cast<float>(1-1/floor*iso),0,static_cast<float>(0.05*floor*iso), 0,0});
}

void MarchingSquare::draw()
{

}
