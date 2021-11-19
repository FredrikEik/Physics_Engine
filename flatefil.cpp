#include "flatefil.h"
#include "string"


FlateFil::FlateFil(std::string filnavn) : VisualObject()
{
    //readFile(filnavn);
    mMatrix.setToIdentity();
    //mMatrix.translate(0,0,5);

    readPoints(filnavn);
    //qDebug() << xMin << xMax << yMin << yMax;


    makePlain();
  // mMatrix.setPosition(-X/2, -Y/2, 0);
    //move(-tempX, -tempY, 0);

}

void FlateFil::makePlain()
{
    float f = 1;
    for(float x = 0; x<static_cast<float>(X)-1; x+=1)
        for(float y =0; y<static_cast<float>(Y)-1; y+=1)
        {
            mVertices.push_back(gsml::Vertex{  x,   y, calcHeight(  x,   y),   f, f, 0});
            mVertices.push_back(gsml::Vertex{x+1,   y, calcHeight(x+1,   y),   0, f, f});
            mVertices.push_back(gsml::Vertex{  x, y+1, calcHeight(  x, y+1),   0, 0, f});
            mVertices.push_back(gsml::Vertex{  x, y+1, calcHeight(  x, y+1),   0, 0, f});
            mVertices.push_back(gsml::Vertex{x+1,   y, calcHeight(x+1,   y),   0, f, 0});
            mVertices.push_back(gsml::Vertex{x+1, y+1, calcHeight(  x+1, y+1),   f, f, 0});

            //              mVertices.push_back(gsml::Vertex{  x,   calcHeight(  x,   y), y,   f, 0, 0});
            //              mVertices.push_back(gsml::Vertex{x+1,   calcHeight(x+1,   y), y,   0, f, 0});
            //              mVertices.push_back(gsml::Vertex{  x, calcHeight(  x, y+1), y+1,   0, 0, f});
            //              mVertices.push_back(gsml::Vertex{  x, calcHeight(  x, y+1), y+1,   0, 0, f});
            //              mVertices.push_back(gsml::Vertex{x+1,   calcHeight(x+1,   y), y,   0, f, 0});
            //              mVertices.push_back(gsml::Vertex{x+1, calcHeight(  x+1, y+1), y+1,   f, 0, 0});
        }


}


FlateFil::~FlateFil()
{

}


void FlateFil::readPoints(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open())
    {
        int m;
        gsml::Vector3d temp;
        inn >> m;
        mVertices.reserve(m);
        for (int i=0; i<m; i++){
            inn >> temp.x;
            inn >> temp.y;
            inn >> temp.z;

            float fX = temp.x - 614580.f;
            fX = fX/n;
            fX = std::floor(fX);
            float fY = (temp.y - 6757290.f);
            fY = fY/n;
            fY = std::floor(fY);
            int tempX = static_cast<int>(fX);
            int tempY = static_cast<int>(fY);
            map[tempX][tempY].push_back(temp);

            if(temp.x < xMin)
                xMin = temp.x;
            if(temp.x > xMax)
                xMax = temp.x;
            if(temp.y < yMin)
                yMin = temp.y;
            if(temp.y > yMax)
                yMax = temp.y;
        }
        inn.close();
    }
    MapMin.x = std::floor(xMin); MapMax.x = std::ceil(xMax);//614580  615580
    MapMin.y = std::floor(yMin); MapMax.y = std::ceil(yMax);//6757290  6758760;
}



float FlateFil::calcHeight(float x, float y)
{
    int xInt = static_cast<int>(x);
    int yInt = static_cast<int>(y);
    float z = 0;
    if(!map[xInt][yInt].empty())
    {

        for(auto it = map[xInt][yInt].begin(); it != map[xInt][yInt].end(); it++)
        {
            z += (*it).z;
        }
        z = z/map[xInt][yInt].size();
    }
    else
        z = 560;
    z = z-550;
    z = z*0.35;
    return z;


}

void FlateFil::init(GLint matrixUniform)
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

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void FlateFil::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());

}


