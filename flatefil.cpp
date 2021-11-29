#include "flatefil.h"
#include "string"


FlateFil::FlateFil(std::string filnavn) : VisualObject()
{
    mCL = new CountoureLines;
    mMatrix.setToIdentity();
    mShader = 1;
    mMesh = new Mesh;
    readPoints(filnavn);
    mMesh->mDrawType = GL_TRIANGLES;
    makePlain();
    calculateNormals();

}


void FlateFil::makePlain()
{

    float f = 0.5;
    for(float x = 0; x<static_cast<float>(X)-1; x+=1)
        for(float y =0; y<static_cast<float>(Y)-1; y+=1)
        {
            //get all height data :D
            float height1 = calcHeight(    x,    y);
            float height2 = calcHeight(  x+1,    y);
            float height3 = calcHeight(    x,  y+1);
            float height4 = calcHeight(    x,  y+1);
            float height5 = calcHeight(  x+1,    y);
            float height6 = calcHeight(  x+1,  y+1);

            mMesh->mVertices.push_back(gsml::Vertex{x, y, height1,   f, f, 0});
            mMesh->mVertices.push_back(gsml::Vertex{(x+1),y, height2,   f, 0, 0});
            mMesh->mVertices.push_back(gsml::Vertex{x, (y+1), height3,   f, f, 0});
            mMesh->mVertices.push_back(gsml::Vertex{x, (y+1), height4,   f, 0, 0});
            mMesh->mVertices.push_back(gsml::Vertex{(x+1),y, height5,   f,0, 0});
            mMesh->mVertices.push_back(gsml::Vertex{(x+1),(y+1),height6,   f, 0, 0});

            int test = static_cast<int>(((height1 + height2 + height3 + height4 + height5 + height6)/6)+mCL->ofsetZ); // GjennomsnittsHøyden
            if(test > mCL->mMax || test < mCL->mMin){
            }else{
                if(std::fmod(test,5)<0.1){
                    CountourPoints.push_back(gsml::Vertex{mCL->ofsetX +  x, mCL->ofsetY +   y ,test+0.5f,       1, 0, 1,0,1});
                }
            }
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
        mMesh->mVertices.reserve(m);
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
        z = 555;

    z = z-550;
    z = z*0.3;
    return z;

}

std::vector<gsml::Vertex> FlateFil::getCountourPoints()
{
    return CountourPoints;
}

void FlateFil::calculateNormals()
{
    for (int i = 0; i <static_cast<int>(mMesh->mVertices.size()); i+=3)
    {
        gsml::Vertex* v1 = &mMesh->mVertices[i];
        gsml::Vertex* v2 = &mMesh->mVertices[i+1];
        gsml::Vertex* v3 = &mMesh->mVertices[i+2];

        gsml::Vector3d p1 = v1->getXYZ();
        gsml::Vector3d p2 = v2->getXYZ();
        gsml::Vector3d p3 = v3->getXYZ();
        // p1, p2 and p3 are the points in the face (f)

        // calculate facet normal of the triangle using cross product;
        // both components are "normalized" against a common point chosen as the base
        gsml::Vector3d p12 = p2 - p1;
        gsml::Vector3d p13 = p3 - p1;
        gsml::Vector3d n = p12^p13;
        //float nF = n.length();    // p1 is the 'base' here

        // get the angle between the two other points for each point;
        // the starting point will be the 'base' and the two adjacent points will be normalized against it
        double a1 = (p2 - p1).Angle(p3 - p1);    // p1 is the 'base' here
        double a2 = (p3 - p2).Angle(p1 - p2);    // p2 is the 'base' here
        double a3 = (p1 - p3).Angle(p2 - p3);    // p3 is the 'base' here

//        // normalize the initial facet normals if you want to ignore surface area
//        if (!area_weighting)
//        {
//            n.normalize();
//        }

        // store the weighted normal in an structured array
        v1->wNormals.push_back(n * a1);
        v2->wNormals.push_back(n * a2);
        v3->wNormals.push_back(n * a3);
    }
    for (int i = 0; i < static_cast<int>(mMesh->mVertices.size()); i++)
    {
        gsml::Vector3d N;

        // run through the normals in each vertex's array and interpolate them
        // vertex(v) here fetches the data of the vertex at index 'v'
        for (int n = 0; n < static_cast<int>(mMesh->mVertices[i].wNormals.size()); n++)
        {
            N = N + mMesh->mVertices[i].wNormals.at(n);
        }

        // normalize the final normal
        N.normalize();
        //mVertices[0].at(i).set_normal(N.x, N.y, N.z);// = result;
        mMesh->mVertices[i].set_normal(N);
    }
}


void FlateFil::init(GLint matrixUniform)
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

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void FlateFil::draw()
{
    glBindVertexArray( mMesh->mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mMesh->mVertices.size());//mVertices.size());

}


