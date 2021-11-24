#include "surf.h"

surf::surf(std::string filnavn) : VisualObject()
{
    mMatrix.setToIdentity();

    mShader = 1;
    read(filnavn);
    make();
    calculateNormals();
}
void surf::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

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
void surf::draw()
{
    glBindVertexArray( mVAO );

}
void surf::calculateNormals()
{
    for (int i = 0; i <static_cast<int>(mVertices.size()); i+=3)
    {
        gsml::Vertex* v1 = &mVertices[i];
        gsml::Vertex* v2 = &mVertices[i+1];
        gsml::Vertex* v3 = &mVertices[i+2];

        gsml::Vector3d p1 = v1->getXYZ();
        gsml::Vector3d p2 = v2->getXYZ();
        gsml::Vector3d p3 = v3->getXYZ();

        gsml::Vector3d p12 = p2 - p1;
        gsml::Vector3d p13 = p3 - p1;
        gsml::Vector3d n = p12^p13;

        double a1 = (p2 - p1).Angle(p3 - p1);    // p1 is the 'base' here
        double a2 = (p3 - p2).Angle(p1 - p2);    // p2 is the 'base' here
        double a3 = (p1 - p3).Angle(p2 - p3);    // p3 is the 'base' here


        v1->wNormals.push_back(n * a1);
        v2->wNormals.push_back(n * a2);
        v3->wNormals.push_back(n * a3);
    }
    for (int i = 0; i < static_cast<int>(mVertices.size()); i++)
    {
        gsml::Vector3d N;


        for (int n = 0; n < static_cast<int>(mVertices[i].wNormals.size()); n++)
        {
            N = N + mVertices[i].wNormals.at(n);
        }


        N.normalize();
        mVertices[i].set_normal(N);
    }
}

void surf::read(std::string filnavn)
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
    Min.x = std::floor(xMin); Max.x = std::ceil(xMax);
    Min.y = std::floor(yMin); Max.y = std::ceil(yMax);
}
void surf::make()
{
    float f = 0.5;
    for(float x = 0; x<static_cast<float>(X-1); x+=1)
        for(float y =0; y<static_cast<float>(Y-1); y+=1)
        {
            mVertices.push_back(gsml::Vertex{f*x, f*y, calcHeight(  x,   y),   f, f, 0});
            mVertices.push_back(gsml::Vertex{f*(x+1),f*y, calcHeight(x+1,   y),   0, f, 0});
            mVertices.push_back(gsml::Vertex{f*x, f*(y+1), calcHeight(  x, y+1),   f+f, f, 0});
            mVertices.push_back(gsml::Vertex{f*x, f*(y+1), calcHeight(  x, y+1),   f+f, f, 0});
            mVertices.push_back(gsml::Vertex{f*(x+1),f*y, calcHeight(x+1,   y),   0, f, 0});
            mVertices.push_back(gsml::Vertex{f*(x+1),f*(y+1), calcHeight(x+1, y+1),   f, f, 0});
        }
}
float surf::calcHeight(float x, float y)
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
        z = 555;

    z = z-550;
    z = z*0.3;
    return z;
}
