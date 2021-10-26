// Dag Nylund 31/1/18 - 1/2/18 - 21/1/19 - 5/11/20
// Lager et nytt namespace gsml
// Bruker dette etter 5/11/20
#ifndef MATRIX4x4_H
#define MATRIX4x4_H

#include <iostream>
#include <fstream>
#include <cmath>
#include "vector3d.h"

namespace gsml { // nytt navn 5/11/20

typedef std::pair<int, int> Dimension;

/*class Vec4
{
public:
    Vec4() {v[0] = v[1] = v[2] = v[3] = 0.0f; }
    Vec4(float a, float b, float c, float d) {v[0]=a; v[1]=b; v[2]=c; v[3]=d;}
    float& operator[] (int i) { return v[i]; }

private:
    float v[4];
};*/

class Matrix4x4
{
public:
    Matrix4x4();
    void setToIdentity();
    void read(std::string filnavn);
    void print() const;
    void set(int i, int j, float x);
    void set_dim(int rader, int kolonner);
    Dimension get_dim() const;
    void operator =(const Matrix4x4& M);
    Matrix4x4 operator * (const Matrix4x4& M) const;
    float& operator () (int i, int j) {return A[i][j]; }

    Matrix4x4 inverse() const;
    Matrix4x4 transpose() const;
    void LU();
    float determinant() const;
    // Disse tre endres/flyttes
    Vector4d solve(Vector4d& b) const;
    void set(int j, Vector4d &v);
    Vector4d operator * (Vector4d &v) const;

    void translate(float tx, float ty, float tz);
    void rotate(float degrees, float rx, float ry, float rz);
    void scale(float sx, float sy, float sz);
    void frustum(float left, float right, float bottom, float top, float near, float far);

    void perspective(float fovy, float aspectRatio, float nearPlane, float farPlane);

    void lookAt(const Vector3d& eye, const Vector3d& at, const Vector3d& up);
    const float * constData() const;

    Vec3 getPosition() const;
    void setPosition(float x,float y, float z);


protected:
    static const int M=4;
    static const int N=4;
    static const float EPS;
    float A[M][N];
    int m; // foreløpig ingen sjekk
    int n; //
    bool lu_faktorisert;
    int permutasjon[M];

    void mult(const Matrix4x4 &M);
    void pivot(int k);
};

}

namespace gs2019 {

typedef std::pair<int, int> Dimension;
typedef Vector4d Vec4;
/*class Vec4
{
public:
    Vec4() {v[0] = v[1] = v[2] = v[3] = 0.0f; }
    Vec4(float a, float b, float c, float d) {v[0]=a; v[1]=b; v[2]=c; v[3]=d;}
    float& operator[] (int i) { return v[i]; }

private:
    float v[4];
};*/

class Matrix4x4
{
public:
    Matrix4x4();
    void setToIdentity();
    void read(std::string filnavn);
    void print() const;
    void set(int i, int j, float x);
    void set_dim(int rader, int kolonner);
    Dimension get_dim() const;
    void operator =(const Matrix4x4& M);
    Matrix4x4 operator * (const Matrix4x4& M) const;
    float& operator () (int i, int j) {return A[i][j]; }

    Matrix4x4 inverse() const;
    Matrix4x4 transpose() const;
    void LU();
    float determinant() const;
    // Disse tre endres/flyttes
    Vec4 solve(Vec4& b) const;
    void set(int j, Vec4 &v);
    Vec4 operator * (Vec4 &v) const;

    void translate(float tx, float ty, float tz);
    void rotate(float degrees, float rx, float ry, float rz);
    void scale(float sx, float sy, float sz);
    void frustum(float left, float right, float bottom, float top, float near, float far);

    void perspective(float fovy, float aspectRatio, float nearPlane, float farPlane);

    void lookAt(const Vector3d& eye, const Vector3d& at, const Vector3d& up);
    const float * constData() const;

protected:
    static const int M=4;
    static const int N=4;
    static const float EPS;
    float A[M][N];
    int m; // foreløpig ingen sjekk
    int n; //
    bool lu_faktorisert;
    int permutasjon[M];

    void mult(const Matrix4x4 &M);
    void pivot(int k);
};

}

#endif // Matrix4x4_H
