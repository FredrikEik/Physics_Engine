// Dag Nylund
// Lager et nytt namespace gsml
// Bruker dette etter 5/11/20

#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <cmath>
#include <iostream>

namespace gsml { // nytt navn 5/11/20
typedef float GLfloat;

struct Vector3d {
    float x;
    float y;
    float z;

    //! Standard and parametric constructor
    Vector3d(float a=0.0, float b=0.0, float c=0.0) : x(a), y(b), z(c) { }

    float& operator [] (int i) { if(i==0) return x; else if (i==1) return y; else if(i==2) return z; }

   /* const Vector3d& operator = (const Vector3d& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }*/

    Vector3d operator + (const Vector3d& v) const {
        Vector3d u;
        u.x = x + v.x;
        u.y = y + v.y;
        u.z = z + v.z;
        return u;
    }

    Vector3d operator - (const Vector3d& v) const {
        Vector3d u;
        u.x = x - v.x;
        u.y = y - v.y;
        u.z = z - v.z;
        return u;
    }

    //! Cross product
    Vector3d operator ^ (const Vector3d& v) const {
        Vector3d w;
        w.x =  y*v.z - z*v.y;
        w.y =  z*v.x - x*v.z;
        w.z =  x*v.y - y*v.x;
        return w;
    }

    GLfloat getX() const
    {
        return x;
    }
    GLfloat getY() const
    {
        return y;
    }
    GLfloat getZ() const
    {
        return z;
    }


    Vector3d& operator+=(const Vector3d &rhs)
    {
        x += rhs.getX();
        y += rhs.getY();
        z += rhs.getZ();

        return *this;
    }

    Vector3d& operator-=(const Vector3d &rhs)
    {
        x -= rhs.getX();
        y -= rhs.getY();
        z -= rhs.getZ();

        return *this;
    }


    bool operator==(const Vector3d &rhs)
    {
        return (x==rhs.x && y==rhs.y && z==rhs.z);
    }

    void setX(const GLfloat &value)
    {
        x = value;
    }
    void setY(const GLfloat &value)
    {
        y = value;
    }
    void setZ(const GLfloat &value)
    {
        z = value;
    }
    Vector3d cross (const Vector3d& v) const {
        Vector3d w;
        w.x =  y*v.z - z*v.y;
        w.y =  z*v.x - x*v.z;
        w.z =  x*v.y - y*v.x;
        return w;
    }
    GLfloat getX() const
    {
        return x;
    }
    GLfloat getY() const
    {
        return y;
    }
    GLfloat getZ() const
    {
        return z;
    }
    static Vector3d cross(const Vector3d &v1, const Vector3d &v2)
    {
        return {((v1.getY() * v2.getZ()) - (v1.getZ() * v2.getY())),
                    ((v1.getZ() * v2.getX()) - (v1.getX() * v2.getZ())),
                    ((v1.getX() * v2.getY()) - (v1.getY() * v2.getX()))};
    }

    Vector3d normalized()
    {
        Vector3d normalized;
        GLfloat l = length();

        if (l > 0.f)
        {
            normalized.setX(x / l);
            normalized.setY(y / l);
            normalized.setZ(z / l);
        }

        return normalized;
    }

    static GLfloat dot(const Vector3d &v1, const Vector3d &v2)
    {
        return ((v1.getX() * v2.getX()) + (v1.getY() * v2.getY()) + (v1.getZ() * v2.getZ()));
    }
    static Vector3d cross(const Vector3d &v1, const Vector3d &v2)
    {
        return {((v1.getY() * v2.getZ()) - (v1.getZ() * v2.getY())), ((v1.getZ() * v2.getX()) - (v1.getX() * v2.getZ())), ((v1.getX() * v2.getY()) - (v1.getY() * v2.getX()))};
    }
    bool operator!=(const Vector3d &rhs)
    {
        return (x!=rhs.x || y!=rhs.y || z!=rhs.z);
    }

    //! Length
    float length() {
        return sqrt(x*x+y*y+z*z);
    }

    //! Normalize to unit length
    void normalize() {
        float d = x*x+y*y+z*z;
        d = sqrt(d);
        if (d>0.0) {
            x=x/d;
            y=y/d;
            z=z/d;
        }
    }

    //! Dot product
    float operator * (const Vector3d& v) const{
        return x*v.x + y*v.y + z*v.z;
    }

    //! Scaling
    Vector3d operator * (float c) const {
        return Vector3d{x*c, y*c, z*c};
    }

    //! \brief indexForMinimum()
    //! \return the index for the minimum element i this
    //!
    int indexForMinimum() const
    {
        int i=2;
        if (x<y && x<z) i=0;
        else if (y<x && y<z) i=1;
        return i;
    }



    //!
    //! \brief barycentricCoordinates()
    //! \param p1 (x,y,0) coordinates for point 1 (z-value is neglected)
    //! \param p2 (x,y,0) coordinates for point 2 (z-value is neglected)
    //! \param p3 (x,y,0) coordinates for point 3 (z-value is neglected)
    //! \return The barycentric coordinates (u, v, w) with respect to triangle (p1, p2, p3)
    //!
    //! The points (p1, p2, p3) are the counterclockwise ordered vertices of the triangle.
    //! The function computes the barycentric coordinates of a point in the plane,
    //! the calling object, with respect to triangle (p1, p2, p3).
    //! u is the barycentric coordinate with respect to p1
    //! v is the barycentric coordinate with respect to p2
    //! w is the barycentric coordinate with respect to p3
    //!
    //! \todo Move this function to other class

    Vector3d barycentricCoordinates(const Vector3d& p1, const Vector3d& p2, const Vector3d& p3)
    {
        Vector3d p12 = p2-p1;
        Vector3d p13 = p3-p1;
        // Ikke bruk z-koordinatene. Vi jobber med trekanter i 2D her og projiserer
        p12.z = 0.0;
        p13.z = 0.0;
        Vector3d n = p12^p13;
        float areal_123 = n.length(); // dobbelt areal

        Vector3d baryc; // til retur
        // u
        Vector3d p = p2 - *this;
        Vector3d q = p3 - *this;
        n = p^q;
        baryc.x = n.z/areal_123;
        // v
        p = p3 - *this;
        q = p1 - *this;
        n = p^q;
        baryc.y = n.z/areal_123;
        // w
        p = p1 - *this;
        q = p2 - *this;
        n = p^q;
        baryc.z = n.z/areal_123;

        return baryc;
    }
    //Friend functions
    friend std::ostream& operator<<(std::ostream &output, const Vector3d &rhs )
    {
        output << "X = " << rhs.x << ", Y = " << rhs.y << ", Z = " << rhs.z;
        return output;
    }
};

typedef Vector3d Vec3;



/*
std::istream& operator>> (std::istream& is, Vector3d& v)
{
// Leser kun tre floats
   is >> v.x >> v.y >> v.z;
   return is;
}
*/
//!
//! \brief operator *   multiplication of scalar and vector
//!
//! Overloading of multiplication operator *
//! This operator is outside Vector3d class
//!
//! \param c            The first operand (scalar)
//! \param v            The second operand (vector)
//! \return             vector
//!
//!
//!
/*
Vector3d operator * (float c, const Vector3d& v)
{
    //float x = v.x*c, y=v.y*c, z=v.z*c;
    return Vector3d{v.x*c, v.y*c, v.z*c};
}
*/

//!
//! \brief barycentricCoordinates()
//! \param p1 (x,y,0) coordinates for point 1 (z-value is neglected)
//! \param p2 (x,y,0) coordinates for point 2 (z-value is neglected)
//! \param p3 (x,y,0) coordinates for point 3 (z-value is neglected)
//! \return The barycentric coordinates (u, v, w) with respect to triangle (p1, p2, p3)
//!
//! The points (p1, p2, p3) are the counterclockwise ordered vertices of the triangle.
//! The function computes the barycentric coordinates of a point in the plane,
//! the calling object, with respect to triangle (p1, p2, p3).
//! u is the barycentric coordinate with respect to p1
//! v is the barycentric coordinate with respect to p2
//! w is the barycentric coordinate with respect to p3
//!
//! \todo Move this function to other class
//typedef Vector3d Vector3d;


struct Vector2d
{
        GLfloat x;
        GLfloat y;

        float& operator[] (int i) { return (i==0) ? x : y; }

        //! Standard and parametric constructor
        Vector2d(GLfloat a=0.0, GLfloat b=0.0) : x(a), y(b) { }
        Vector2d(const Vector3d& v) { x=v.x; y=v.y; } // 8/4/19
        Vector2d operator = (const Vector3d& v) { x = v.x; y=v.y; return Vector2d{x,y};}
        Vector2d operator = (Vector3d& v) { x = v.x; y=v.y; return Vector2d{x,y};}

        //! Cross product
        Vector3d operator ^ (const Vector2d& v) const
        {
            Vector3d w;
            w.x =  0.0;
            w.y =  0.0;
            w.z =  x*v.y - y*v.x;
            return w;
        }
        Vector2d operator - (const Vector2d& v) const /// Subtraction
        {
            Vector2d u;
            u.x = x - v.x;
            u.y = y - v.y;
            //u.z = z - v.z;
            return u;
        }



        Vector3d barycentricCoordinates(const Vector2d& p1, const Vector2d& p2, const Vector2d& p3)
        {
            Vector2d p12 = p2-p1;
            Vector2d p13 = p3-p1;

    Vector3d n = p12^p13;
    float areal_123 = n.length(); // dobbelt areal

    Vector3d baryc; // til retur. Husk
    // u
    Vector2d p = p2 - *this;
    Vector2d q = p3 - *this;
    n = p^q;
    baryc.x = n.z/areal_123;
    // v
    p = p3 - *this;
    q = p1 - *this;
    n = p^q;
    baryc.y = n.z/areal_123;
    // w
    p = p1 - *this;
    q = p2 - *this;
    n = p^q;
    baryc.z = n.z/areal_123;

    return baryc;
}
    void normalize()
    {
        GLfloat d = x*x+y*y;
        d = sqrt(d);
        if (d>0.0)
        {
            x=x/d;
            y=y/d;
        }
    }
    float length() {
        return sqrt(x*x+y*y);
    }

};
typedef Vector2d Vec2;


class Vector4d
{
public:
    Vector4d() {v[0] = v[1] = v[2] = v[3] = 0.0f; }
    Vector4d(float a, float b, float c, float d) {v[0]=a; v[1]=b; v[2]=c; v[3]=d;}
    float& operator[] (int i) { return v[i]; }

private:
    float v[4];
};

}
// Noen (?) endringer 20/9/19

namespace gs2019 {
typedef float GLfloat;

struct Vector3d {
    float x;
    float y;
    float z;

    //! Standard and parametric constructor
    Vector3d(float a=0.0, float b=0.0, float c=0.0) : x(a), y(b), z(c) { }

    float& operator [] (int i) { if(i==0) return x; else if (i==1) return y; else if(i==2) return z; }

   /* const Vector3d& operator = (const Vector3d& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }*/

    Vector3d operator + (const Vector3d& v) const {
        Vector3d u;
        u.x = x + v.x;
        u.y = y + v.y;
        u.z = z + v.z;
        return u;
    }

    Vector3d operator - (const Vector3d& v) const {
        Vector3d u;
        u.x = x - v.x;
        u.y = y - v.y;
        u.z = z - v.z;
        return u;
    }

    //! Cross product
    Vector3d operator ^ (const Vector3d& v) const {
        Vector3d w;
        w.x =  y*v.z - z*v.y;
        w.y =  z*v.x - x*v.z;
        w.z =  x*v.y - y*v.x;
        return w;
    }
    Vector3d cross (const Vector3d& v) const {
        Vector3d w;
        w.x =  y*v.z - z*v.y;
        w.y =  z*v.x - x*v.z;
        w.z =  x*v.y - y*v.x;
        return w;
    }

    //! Length
    float length() {
        return sqrt(x*x+y*y+z*z);
    }

    //! Normalize to unit length
    void normalize() {
        float d = x*x+y*y+z*z;
        d = sqrt(d);
        if (d>0.0) {
            x=x/d;
            y=y/d;
            z=z/d;
        }
    }

    //! Dot product
    float operator * (const Vector3d& v) const{
        return x*v.x + y*v.y + z*v.z;
    }

    //! Scaling
    Vector3d operator * (float c) const {
        return Vector3d{x*c, y*c, z*c};
    }

    //! \brief indexForMinimum()
    //! \return the index for the minimum element i this
    //!
    int indexForMinimum() const
    {
        int i=2;
        if (x<y && x<z) i=0;
        else if (y<x && y<z) i=1;
        return i;
    }

    //!
    //! \brief barycentricCoordinates()
    //! \param p1 (x,y,0) coordinates for point 1 (z-value is neglected)
    //! \param p2 (x,y,0) coordinates for point 2 (z-value is neglected)
    //! \param p3 (x,y,0) coordinates for point 3 (z-value is neglected)
    //! \return The barycentric coordinates (u, v, w) with respect to triangle (p1, p2, p3)
    //!
    //! The points (p1, p2, p3) are the counterclockwise ordered vertices of the triangle.
    //! The function computes the barycentric coordinates of a point in the plane,
    //! the calling object, with respect to triangle (p1, p2, p3).
    //! u is the barycentric coordinate with respect to p1
    //! v is the barycentric coordinate with respect to p2
    //! w is the barycentric coordinate with respect to p3
    //!
    //! \todo Move this function to other class

    Vector3d barycentricCoordinates(const Vector3d& p1, const Vector3d& p2, const Vector3d& p3)
    {
        Vector3d p12 = p2-p1;
        Vector3d p13 = p3-p1;
        // Ikke bruk z-koordinatene. Vi jobber med trekanter i 2D her og projiserer
        p12.z = 0.0;
        p13.z = 0.0;
        Vector3d n = p12^p13;
        float areal_123 = n.length(); // dobbelt areal

        Vector3d baryc; // til retur
        // u
        Vector3d p = p2 - *this;
        Vector3d q = p3 - *this;
        n = p^q;
        baryc.x = n.length()/areal_123;
        // v
        p = p3 - *this;
        q = p1 - *this;
        n = p^q;
        baryc.y = n.length()/areal_123;
        // w
        p = p1 - *this;
        q = p2 - *this;
        n = p^q;
        baryc.z = n.length()/areal_123;

        return baryc;
    }
    //Friend functions
    friend std::ostream& operator<<(std::ostream &output, const Vector3d &rhs )
    {
        output << "X = " << rhs.x << ", Y = " << rhs.y << ", Z = " << rhs.z;
        return output;
    }

};

typedef Vector3d Vec3;

/*
std::istream& operator>> (std::istream& is, Vector3d& v)
{
// Leser kun tre floats
   is >> v.x >> v.y >> v.z;
   return is;
}
*/
//!
//! \brief operator *   multiplication of scalar and vector
//!
//! Overloading of multiplication operator *
//! This operator is outside Vector3d class
//!
//! \param c            The first operand (scalar)
//! \param v            The second operand (vector)
//! \return             vector
//!
//!
//!
/*
Vector3d operator * (float c, const Vector3d& v)
{
    //float x = v.x*c, y=v.y*c, z=v.z*c;
    return Vector3d{v.x*c, v.y*c, v.z*c};
}
*/

//!
//! \brief barycentricCoordinates()
//! \param p1 (x,y,0) coordinates for point 1 (z-value is neglected)
//! \param p2 (x,y,0) coordinates for point 2 (z-value is neglected)
//! \param p3 (x,y,0) coordinates for point 3 (z-value is neglected)
//! \return The barycentric coordinates (u, v, w) with respect to triangle (p1, p2, p3)
//!
//! The points (p1, p2, p3) are the counterclockwise ordered vertices of the triangle.
//! The function computes the barycentric coordinates of a point in the plane,
//! the calling object, with respect to triangle (p1, p2, p3).
//! u is the barycentric coordinate with respect to p1
//! v is the barycentric coordinate with respect to p2
//! w is the barycentric coordinate with respect to p3
//!
//! \todo Move this function to other class
//typedef Vector3d Vector3d;


struct Vector2d
{
        GLfloat x;
        GLfloat y;

        float& operator[] (int i) { return (i==0) ? x : y; }

        //! Standard and parametric constructor
        Vector2d(GLfloat a=0.0, GLfloat b=0.0) : x(a), y(b) { }
        Vector2d(const Vector3d& v) { x=v.x; y=v.y; } // 8/4/19
        Vector2d operator = (const Vector3d& v) { x = v.x; y=v.y; return Vector2d{x,y};}
        Vector2d operator = (Vector3d& v) { x = v.x; y=v.y; return Vector2d{x,y};}

        //! Cross product
        Vector3d operator ^ (const Vector2d& v) const
        {
            Vector3d w;
            w.x =  0.0;
            w.y =  0.0;
            w.z =  x*v.y - y*v.x;
            return w;
        }
        Vector2d operator - (const Vector2d& v) const /// Subtraction
        {
            Vector2d u;
            u.x = x - v.x;
            u.y = y - v.y;
            //u.z = z - v.z;
            return u;
        }


//Vector3d barycentricCoordinates(const Vector2d& p1, const Vector2d& p2, const Vector2d& p3)
//{
//    Vector2d p12 = p2-p1;
//    Vector2d p13 = p3-p1;

//    Vector3d n = p12^p13;
//    float areal_123 = n.length(); // dobbelt areal

//    Vector3d baryc; // til retur. Husk
//    // u
//    Vector2d p = p2 - *this;
//    Vector2d q = p3 - *this;
//    n = p^q;
//    baryc.x = n.z/areal_123;
//    // v
//    p = p3 - *this;
//    q = p1 - *this;
//    n = p^q;
//    baryc.y = n.z/areal_123;
//    // w
//    p = p1 - *this;
//    q = p2 - *this;
//    n = p^q;
//    baryc.z = n.z/areal_123;

//    return baryc;
//}
//    void normalize()
//    {
//        GLfloat d = x*x+y*y;
//        d = sqrt(d);
//        if (d>0.0)
//        {
//            x=x/d;
//            y=y/d;
//        }
//    }
//    float length() {
//        return sqrt(x*x+y*y);
//    }

};
typedef Vector2d Vec2;


class Vector4d
{
public:
    Vector4d() {v[0] = v[1] = v[2] = v[3] = 0.0f; }
    Vector4d(float a, float b, float c, float d) {v[0]=a; v[1]=b; v[2]=c; v[3]=d;}
    float& operator[] (int i) { return v[i]; }

private:
    float v[4];
};

}


#endif // VECTOR3D_H
