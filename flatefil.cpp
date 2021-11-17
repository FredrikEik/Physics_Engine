#include "flatefil.h"
#include "string"


FlateFil::FlateFil(std::string filnavn) : VisualObject()
{
    //readFile(filnavn);
    mMatrix.setToIdentity();
    //mMatrix.translate(0,0,5);

    readPoints(filnavn);
    // qDebug() << xMin << xMax << yMin << yMax;

    makePlain();

//    float tempX = MapMin.x + MapMax.x;
//    tempX = tempX/2;
//    float tempY = MapMin.y + MapMax.y;
//    tempY = tempY/2;
//    mMatrix.setPosition(-tempX, -tempY, 0);
//    //move(-tempX, -tempY, 0);

}

void FlateFil::makePlain()
{
    float f = 1;
    for(auto x = MapMin.x; x<static_cast<int>(X); x+=n)
        for(auto y = MapMin.y; y<static_cast<int>(Y); y+=n)
        {
            float z = calcHeight(x, y);
            mVertices.push_back(gsml::Vertex{  x,   y, calcHeight(x,y),   f, 0, 0});
            mVertices.push_back(gsml::Vertex{x+n,   y, calcHeight(x+1,y),   0, f, 0});
            mVertices.push_back(gsml::Vertex{  x, y+n, calcHeight(x,y+1),   0, 0, f});
            mVertices.push_back(gsml::Vertex{  x, y+n, calcHeight(x,y+1),   0, 0, f});
            mVertices.push_back(gsml::Vertex{x+n,   y, calcHeight(x+1,y),   0, f, 0});
            mVertices.push_back(gsml::Vertex{x+n, y+n, calcHeight(x+1,y+1),   f, 0, 0});
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

        for (int i=0; i<m; i++)
        {
            inn >> temp.x;
            inn >> temp.y;
            inn >> temp.z;


            int fX= (temp.x - MapMin.x);
            fX = fX/n;
            fX = std::floor(fX);
            int fY = (temp.y - MapMin.y)/n;
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
    MapMin.y = std::floor(yMin); MapMax.y = std::ceil(yMax);//6757290  6758760
}


float FlateFil::calcHeight(float x, float y)
{
    std::vector<float> mH;
    float z = 0;
    for(auto i{0}; i<(int)points.size(); i++)
    {
        if(points[i].x >= x && points[i].x < (x+n))
            if(points[i].y >= y && points[i].y < (y+n))
                mH.push_back(points[i].z);
    }
    int hSize = (int)mH.size();
    if(hSize>0){
        for(auto i{0}; i<hSize; i++)
            z += mH[i];
        z = z/hSize;}

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


