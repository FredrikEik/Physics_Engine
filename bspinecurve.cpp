#include "bspinecurve.h"

BSpineCurve::BSpineCurve()
{
    initialize();
}

void BSpineCurve::initialize()
{
    for(auto x = t[0]; x<t[n]; x+=0.05)
    {
        gsml::Vector3d  punkt = deBoor(x);
        gsml::Vertex v{};
        v.set_xyz(punkt.x, punkt.y, punkt.z); v.set_rgb(1,0,0);
        mVertices.push_back(v);
    }
}


gsml::Vector3d BSpineCurve::deBoor(double x)
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



int BSpineCurve::findKnotInterval(float x)
{
    int my = n-1; //Indeks til siste kontrollpunkt
    while (x<t[my] && my>d) {
        my--;
    }return my;
}

void BSpineCurve::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_LINES, 0, mVertices.size());//mVertices.size());
}
