#include "flatefil.h"
#include "string"


FlateFil::FlateFil(std::string filnavn) : VisualObject()
{
    readFile(filnavn);
    qDebug()<<xMax << xMin;
    qDebug() <<yMax << yMin;
    makePlain();

}

void FlateFil::makePlain()
{
    xMax = 615578 ;
    xMin = 614581 ;
    yMax = 6758759;
    yMin = 6757299;
    float z = 0;
    for(auto x = MapMin.x; x<MapMax.x; x+=n)
        for(auto y = MapMin.y; y<MapMax.y; y+=n)
        {
            mVertices.push_back(gsml::Vertex{x, y, z, 1, 0, 1});
            mVertices.push_back(gsml::Vertex{x+n, y, z, 1, 0, 1});
            mVertices.push_back(gsml::Vertex{x, y+n, z, 1, 0, 1});
            mVertices.push_back(gsml::Vertex{x, y+n, z, 1, 0, 1});
            mVertices.push_back(gsml::Vertex{x+n, y, z, 1, 0, 1});
            mVertices.push_back(gsml::Vertex{x+n, y+n, z, 1, 0, 1});
        }
//    gsml::Vertex v{};
//    v.set_xyz(xMin,yMax,0); v.set_rgb(1,0,0); mVertices.push_back(v);
//    v.set_xyz(xMax,yMax,0); v.set_rgb(0,1,0); mVertices.push_back(v);
//    v.set_xyz(xMin,yMin,0); v.set_rgb(0,0,1); mVertices.push_back(v);
//    v.set_xyz(xMax,yMin,0); v.set_rgb(0,1,0); mVertices.push_back(v);
}


FlateFil::~FlateFil()
{

}



void FlateFil::readFile(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open())
    {
        int n;
        gsml::Vector3d temp;
        inn >> n;
        for (int i=0; i<n; i++)
        {
             inn >> temp.x;
             inn >> temp.y;
             inn >> temp.z;
             points.push_back(temp);
        }
        inn.close();
    }

    for(int i{0}; i<(int)points.size(); i++){
        if(points[i].x < xMin)
            xMin = points[i].x;
        if(points[i].x > xMax)
            xMax = points[i].x;
        if(points[i].y < yMin)
            yMin = points[i].y;
        if(points[i].y > yMax)
            yMax = points[i].y;
        if(points[i].z < zMin)
            zMin = points[i].z;
        if(points[i].z > yMax)
            zMax = points[i].z;
    }

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


