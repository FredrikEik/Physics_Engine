#include "bsplinecurve.h"

BSplineCurve::BSplineCurve(int dID)
{
    std::string sID = std::to_string(dID);
    mTxt = mTxt + sID;
    mTxt = mTxt + ".txt";
    mMesh = new Mesh;
    mMatrix.setToIdentity();
    mMesh->mDrawType = GL_LINES;
    initialize();
}

void BSplineCurve::initialize()
{
    addC(mTxt);
    for(double x = t[0]; x<t[n]; x+=0.05)
    {
        gsml::Vector3d punkt = deBoor(x);
        gsml::Vertex v{};
        v.set_xyz(punkt.x, punkt.y, punkt.z); v.set_rgb(1,0,0);
        mMesh-> mVertices.push_back(v);
    }
}


gsml::Vector3d BSplineCurve::deBoor(double x)
{
    int my = findKnotInterval(x);
    std::vector<gsml::Vector3d> a;
    a.reserve(d+1);
    for(int j = 0; j<=d; j++){
        a[d-j] = c[my-j];
    }
    for(int k = d; k>0; k--){
        int j = my-k;
        for(int i=0; i<k; i++){
            j++;
            float w =(x-t[j])/(t[j+k]-t[j]);
            a[i] = a[i] * (1-w) + a[i+1]*w;
        }
    }
    return a[0];
}



int BSplineCurve::findKnotInterval(float x)
{
    int my = n-1; //Indeks til siste kontrollpunkt
    while (x<t[my] && my>d) {
        my--;
    }return my;
}

void BSplineCurve::init(GLint matrixUniform)
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

void BSplineCurve::draw()
{
    glBindVertexArray( mMesh->mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_LINES, 0, mMesh->mVertices.size());//mVertices.size());
}

void BSplineCurve::addC(std::string filnavn)
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

            c.push_back(temp);
        }
        inn.close();
    }
    n = static_cast<int>(c.size());
}
