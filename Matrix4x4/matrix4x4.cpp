// Dag Nylund 31/1/18 - 1/2/18
// gsml fra 5/11/20
// gs2019 ligger nederst
#include "../Matrix4x4/Matrix4x4.h"

#include <iomanip>
#include <cmath>

namespace gsml {

using std::fixed;
using std::right;
using std::setw;
using std::setprecision;
#define debug 0


const float Matrix4x4::EPS = 0.000001;

Matrix4x4::Matrix4x4() : m(4), n(4), lu_faktorisert(false)
{
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            A[i][j] = 0.0f;
}

void Matrix4x4::setToIdentity()
{
    if (m == n) {
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            A[i][j] = 0.0f;
    for (int i=0; i<m; i++)
        A[i][i] = 1.0f;
    }
}
void Matrix4x4::read(std::string filnavn)
{
    std::ifstream inn(filnavn.c_str()); // Åpne eksisterende fil
    if (inn.is_open()) // Lese punkter - her: linje for linje
    {
        inn >> m >> n;
        // Må kreve m=n=4
        for (int i=0; i<m; i++)
           for (int j=0; j<n; j++)
               inn >> A[i][j];
   }
   inn.close(); // Lukke fil
}

void Matrix4x4::print() const
{
    for (int i=0; i<m; i++)
    {
        //std::cout << setw(8) << setprecision(2)  << fixed << right;
        for (int j=0; j<n; j++)
            std::cout << setw(8) << setprecision(2)  << fixed << right << A[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Matrix4x4::set(int i, int j, float x)
{
    A[i][j] = x;
}

void Matrix4x4::set(int j, Vector4d &v)
{
    for (int i=0; i<m; i++)
        A[i][j] = v[i];
}

void Matrix4x4::set_dim(int rader, int kolonner)
{
    m = rader; n = kolonner;
}

void Matrix4x4::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    A[0][3] = x;
    A[1][3] = y;
    A[2][3] = z;
}

Vector3d Matrix4x4::getPosition()
{
    Vec3 temp;
    temp.x = A[0][3];
    temp.y = A[1][3];
    temp.z = A[2][3];
    return temp;
}

Dimension Matrix4x4::get_dim() const
{
    return Dimension(m, n);
}

// Det er ikke veldig elegant å skrive M.A[i][j]
// gjentatte ganger. Et alternativ er å overlaste
// en () operator for klassen
void Matrix4x4::operator =(const Matrix4x4 &M)
{
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            set(i, j, M.A[i][j]);
            //A[i][j] = M(i,j);
}

void Matrix4x4::mult(const Matrix4x4& M) // Hjelpefunksjon
{
    Matrix4x4 AA;
    //M.print();
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
           for(int k=0; k<4; k++)
                // multipliserer i-te rad i A
                // med j-te kolonne i B
                AA.A[i][j] += A[i][k]*M.A[k][j];
                //AA(i,j) += A[i][k]*M(k,j);
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            //set(i, j, AA.A[i][j]);
            A[i][j] = AA.A[i][j];
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& M) const
{
    Matrix4x4 AA;
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            for (int k=0; k<4; k++)
                // endre for andre enn 4x4 matriser
                // altså kontroller dimensjonene
                AA.A[i][j] += A[i][k]*M.A[k][j];
                //AA(i,j) += A[i][k]*M(k,j);
    return AA;
}

// Pivotering, jukser litt og bruker en Vector4d til å lagre permutasjoner
// til retur og bruk på b-vektor
void Matrix4x4::pivot(int k)
{
    if (-EPS <= A[k][k] && A[k][k] < EPS) // zero diagonalelement
    {
        // pivotering/swap - finner rad med største element i kolonne
        if (debug) std::cout << "må pivotere.\n";

        int pivot = k+1; // initierer kandidat for rad-pivotering
        for (int i=k+2; i<m; i++)
        {
            if (A[i][k] > A[pivot][k])
                pivot = i;
        }
        if (-EPS <= A[k][k] && A[k][k] < EPS)
            // std::cout << "Kan ikke invertere\n";
            ; // exception

        // swapper rader
        for (int j=k; j<n; j++)
           std::swap(A[k][j], A[pivot][j]);
        std::swap(permutasjon[k], permutasjon[pivot]);
    }
}

void Matrix4x4::LU()
{
    // Nå løses matriseligning med permutasjon, og vi må huske
    // på å initiere en vektor med indekser
    for (int i=0; i<m; i++)
        permutasjon[i] = i;

    for (int k=0; k<m-1; k++) // tar ikke med nederste rad
    {
        pivot(k); // permutasjon er nå member

        // Ved radoperasjoner skal vi oppnå 0-ere under diagonalelementet
        // i alle rader nedenfor (altså kolonne k).
        // Vi subtraherer et multiplum av k-te
        // rad fra radene nedenfor, nuller ut kolonner fra venstre
        for (int i=k+1; i<m; i++)
        {
            // Skal mult med denne og deretter trekke fra rad i
            // denne skal bli null og vi kan lagre faktoren her
            A[i][k] = A[i][k]/A[k][k];

            for (int j=k+1; j<m; j++) // m==n
            {
                // kolonnene til høyre for den som blir nullet ut
                A[i][j] = A[i][j] - A[i][k]*A[k][j];
            }
            // Den følgende linjen gjøres på permutert vektor
            // i solve(), bedre ved løsning av flere x=A^(-1)*b
            // siden LU-faktorisering kan brukes opp igjen
            // b[i] = b[i] - A[i][k]*b[k];
        }
    }
    lu_faktorisert = true;
}
// Determinanten til en diagonalmatrise er produktet av diagonalelementene
// Returnerer determinanten hvis LU-faktorisert, 0 ellers (hvilket ikke nødvendigvis er riktig
float Matrix4x4::determinant() const
{
    float diagonalProdukt = 1.0f;
    if (lu_faktorisert)
    {
        for (int i=0; i<m; i++)
            diagonalProdukt *= A[i][i];
        return diagonalProdukt;
    }
    return 0.0f;
}
/*
 *                      b0  b1  b2  b3       e1 e2 e3 e4
 *
 * |A00 A01 A02 A03|   |B00 B01 B02 B03|   | 1  0  0  0 |
 * |A10 A11 A12 A13| * |B10 B11 B12 B13| = | 0  1  0  0 |
 * |A20 A21 A22 A23|   |B20 B21 B22 B23|   | 0  0  1  0 |
 * |A30 A31 A32 A33|   |B30 B31 B32 B33|   | 0  0  0  1 |
 *
 * Fire ligninger:
 *  A b0 = e1
 *  A b1 = e2
 *  A b2 = e3
 *  A b3 = e4
 *
 * Hver av disse bestemmer en kolonne i B.
 * Siden AB=I er B=A sin inverse
 */
Matrix4x4 Matrix4x4::inverse() const
{
    Matrix4x4 H = *this;            // inverse() er const, så vi trenger en
    Vector4d perm(0,1,2,3);             // kopi å LU-faktorisere

    H.LU();
    Vector4d e1(1.0f, 0.0f, 0.0f, 0.0f);
    Vector4d e2(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4d e3(0.0f, 0.0f, 1.0f, 0.0f);
    Vector4d e4(0.0f, 0.0f, 0.0f, 1.0f);

    // Løser A*b0 = e1
    Vector4d e1_perm(e1[(int)perm[0]], e1[(int)perm[1]], e1[(int)perm[2]], e1[(int)perm[3]]);
    Vector4d x = H.solve(e1);//H.solve(e1_perm);
    Matrix4x4 M;
    M.set(0, x);

    // Løser A*b1 = e2
    Vector4d e2_perm(e2[(int)perm[0]], e2[(int)perm[1]], e2[(int)perm[2]], e2[(int)perm[3]]);
    x = H.solve(e2);//(e2_perm);
    M.set(1, x);

    // Løser A*b2 = e3
    Vector4d e3_perm(e3[(int)perm[0]], e3[(int)perm[1]], e3[(int)perm[2]], e3[(int)perm[3]]);
    x = H.solve(e3);//(e3_perm);
    M.set(2, x);

    // Løser A*b3 = e4
    Vector4d e4_perm(e4[(int)perm[0]], e4[(int)perm[1]], e4[(int)perm[2]], e4[(int)perm[3]]);
    x = H.solve(e4);//(e4_perm);
    M.set(3, x);

    return M;
}

// Sjekke m<M, n<N etc og sette dim
Matrix4x4 Matrix4x4::transpose() const
{
    Matrix4x4 T;
    for (auto i=0; i<m; i++)
        for (auto j=0; j<n; j++)
            T(i,j) = A[j][i];
    return T;
}

// Løser Ax=b, syntaks x = A.solve(b)
// Forutsetter at LU er gjort på A
Vector4d Matrix4x4::solve(Vector4d &b) const
{
    Vector4d x;
    Vector4d b_perm(b[permutasjon[0]], b[permutasjon[1]], b[permutasjon[2]], b[permutasjon[3]]);

    for (int k=0; k<n-1; k++) // m==n
        for (int i=k+1; i<n; i++)
            b_perm[i] = b_perm[i]-A[i][k]*b_perm[k];

    for (int i=n-1; i>=0; i--)
    {
        x[i] = b_perm[i];
        for (int j=i+1; j<n; j++)
            x[i] = x[i] - A[i][j]*x[j];
        x[i] = x[i]/A[i][i];
    }
    return x;
}

Vector4d Matrix4x4::operator * ( Vector4d& v) const
{
    Vector4d x;
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            x[i] += A[i][j]*v[j];
    return x;
}


void Matrix4x4::translate(float tx, float ty, float tz)
{
    // Lager translasjonssmatrise
    Matrix4x4 T;
    T.setToIdentity();
    //T.A[0][3] = tx;
    //T.A[1][3] = ty;
    //T.A[2][3] = tz;
    T(0,3) = tx;
    T(1,3) = ty;
    T(2,3) = tz;

    // Translerer - multipliserer med translasjonsmatrisen
    mult(T);
}

// Rotasjon om (1,0,0), (0,1,0) eller (0,0,1)
// Rotasjon om vilkårlig akse er beskrevet i Angel 4.10.4
void Matrix4x4::rotate(float degrees, float rx, float ry, float rz)
{
    // Lager rotasjonsmatrise
    Matrix4x4 R;
    R.setToIdentity();

    float pi = 4*std::atan(1.0);
    float rad = degrees*pi/180;
    float a = std::cos(rad);
    float b = std::sin(rad);
// Her kan vi ha både 1 og -1 som rx ry rz
    if (rx==0.0f && ry==0.0f && rz==1.0f)
    {
        //R.A[0][0] = a;
        //R.A[1][0] = b;
        //R.A[0][1] = -b;
        //R.A[1][1] = a;
        R(0,0) = a;
        R(1,0) = b;
        R(0,1) = -b;
        R(1,1) = a;
    }
    else if (rx==0.0f && ry==1.0f && rz==0.0f)
    {
        //R.A[0][0] = a;
        //R.A[2][0] = b;
        //R.A[0][2] = -b;
        //R.A[2][2] = a;
        R(0,0) = a;
        R(2,0) = b;
        R(0,2) = -b;
        R(2,2) = a;
    }
    if (rx==1.0f && ry==0.0f && rz==0.0f)
    {
        //R.A[1][1] = a;
        //R.A[2][1] = b;
        //R.A[1][2] = -b;
        //R.A[2][2] = a;
        R(1,1) = a;
        R(2,1) = b;
        R(1,2) = -b;
        R(2,2) = a;
    }
    // Roterer - multipliserer med rotasjonematrisen
    mult(R);
}

void Matrix4x4::scale(float sx, float sy, float sz)
{
    // Lager skaleringsmatrisen
    Matrix4x4 S;
    S.setToIdentity();
    S(0,0) = sx;
    S(1,1) = sy;
    S(2,2) = sz;
    // Skalerer - multipliserer med skaleringsmatrisen
    mult(S);
}
// Se Angel kapittel 5.7.2
void Matrix4x4::frustum(float left, float right, float bottom, float top, float near, float far)
{
    // Forutsetter a_ij initielt 0
    A[0][0] = 2*near/(right-left);
    A[1][1] = 2*near/(top-bottom);
    A[0][2] = (right+left)/(right-left);
    A[1][2] = (top+bottom)/(top-bottom);
    A[2][2] = -(far+near)/(far-near);
    A[2][3] = -2*far*near/(far-near);
    A[3][2] = -1.0f;
    A[3][3] = 0.0f;
    /*
    */
}
// Se Angel kapittel 5.7.2
void Matrix4x4::perspective(float fovy, float aspectRatio, float near, float far)
{
    float top = near*std::tan(fovy);
    float right = top/aspectRatio;
    A[0][0] = near/right;
    A[1][1] = near/top;
    A[2][2] = -(far+near)/(far-near);
    A[2][3] = -2*far*near/(far-near);
    A[3][2] = -1;
}
// Transformasjon fra xyz til uvn, se Angel(7th ed) 5.3.2
// up - orienteringen av kamera, trenger ikke ligge i viewplan
// u = v x n gir riktig retning i høyrehåndssystem.
// n er normal på viewplan, retning mot kamera (som z-aksen er default)
void Matrix4x4::lookAt(const Vector3d &eye, const Vector3d &at, const Vector3d &up)
{
    Matrix4x4 M;
    M.setToIdentity();
    Vector3d v = up;
    v.normalize();          // y-akse i kameraframe
    Vector3d n = eye-at;
    n.normalize();          // z-akse i kameraframe
    Vector3d u = v.cross(n);
    u.normalize();          // x-akse i kameraframe
    v = n.cross(u);
    v.normalize();
    M(0,0) = u.x;  M(0,1) = u.y;  M(0,2) = u.z;
    M(1,0) = v.x;  M(1,1) = v.y;  M(1,2) = v.z;
    M(2,0) = n.x;  M(2,1) = n.y;  M(2,2) = n.z;
    M.translate(-eye.x, -eye.y, -eye.z);
    //M.print();
    mult(M);
}
// internt er A M*N, så return &A[0][0];
// går ikke. Returner kolonnevis 4x4
const float *Matrix4x4::constData() const
{
    return &A[0][0];
}


}
namespace gs2019 {

using std::fixed;
using std::right;
using std::setw;
using std::setprecision;
#define debug 0


const float Matrix4x4::EPS = 0.000001;

Matrix4x4::Matrix4x4() : m(4), n(4), lu_faktorisert(false)
{
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            A[i][j] = 0.0f;
}

void Matrix4x4::setToIdentity()
{
    if (m == n) {
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            A[i][j] = 0.0f;
    for (int i=0; i<m; i++)
        A[i][i] = 1.0f;
    }
}
void Matrix4x4::read(std::string filnavn)
{
    std::ifstream inn(filnavn.c_str()); // Åpne eksisterende fil
    if (inn.is_open()) // Lese punkter - her: linje for linje
    {
        inn >> m >> n;
        // Må kreve m=n=4
        for (int i=0; i<m; i++)
           for (int j=0; j<n; j++)
               inn >> A[i][j];
   }
   inn.close(); // Lukke fil
}

void Matrix4x4::print() const
{
    for (int i=0; i<m; i++)
    {
        //std::cout << setw(8) << setprecision(2)  << fixed << right;
        for (int j=0; j<n; j++)
            std::cout << setw(8) << setprecision(2)  << fixed << right << A[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Matrix4x4::set(int i, int j, float x)
{
    A[i][j] = x;
}

void Matrix4x4::set(int j, Vector4d &v)
{
    for (int i=0; i<m; i++)
        A[i][j] = v[i];
}

void Matrix4x4::set_dim(int rader, int kolonner)
{
    m = rader; n = kolonner;
}

Dimension Matrix4x4::get_dim() const
{
    return Dimension(m, n);
}

// Det er ikke veldig elegant å skrive M.A[i][j]
// gjentatte ganger. Et alternativ er å overlaste
// en () operator for klassen
void Matrix4x4::operator =(const Matrix4x4 &M)
{
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            set(i, j, M.A[i][j]);
            //A[i][j] = M(i,j);
}

void Matrix4x4::mult(const Matrix4x4& M) // Hjelpefunksjon
{
    Matrix4x4 AA;
    //M.print();
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            for(int k=0; k<4; k++)
                // multipliserer i-te rad i A
                // med j-te kolonne i B
                AA.A[i][j] += A[i][k]*M.A[k][j];
                //AA(i,j) += A[i][k]*M(k,j);
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            //set(i, j, AA.A[i][j]);
            A[i][j] = AA.A[i][j];
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& M) const
{
    Matrix4x4 AA;
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            for (int k=0; k<4; k++)
                // endre for andre enn 4x4 matriser
                // altså kontroller dimensjonene
                AA.A[i][j] += A[i][k]*M.A[k][j];
                //AA(i,j) += A[i][k]*M(k,j);
    return AA;
}

// Pivotering, jukser litt og bruker en Vector4d til å lagre permutasjoner
// til retur og bruk på b-vektor
void Matrix4x4::pivot(int k)
{
    if (-EPS <= A[k][k] && A[k][k] < EPS) // zero diagonalelement
    {
        // pivotering/swap - finner rad med største element i kolonne
        if (debug) std::cout << "må pivotere.\n";

        int pivot = k+1; // initierer kandidat for rad-pivotering
        for (int i=k+2; i<m; i++)
        {
            if (A[i][k] > A[pivot][k])
                pivot = i;
        }
        if (-EPS <= A[k][k] && A[k][k] < EPS)
            // std::cout << "Kan ikke invertere\n";
            ; // exception

        // swapper rader
        for (int j=k; j<n; j++)
           std::swap(A[k][j], A[pivot][j]);
        std::swap(permutasjon[k], permutasjon[pivot]);
    }
}

void Matrix4x4::LU()
{
    // Nå løses matriseligning med permutasjon, og vi må huske
    // på å initiere en vektor med indekser
    for (int i=0; i<m; i++)
        permutasjon[i] = i;

    for (int k=0; k<m-1; k++) // tar ikke med nederste rad
    {
        pivot(k); // permutasjon er nå member

        // Ved radoperasjoner skal vi oppnå 0-ere under diagonalelementet
        // i alle rader nedenfor (altså kolonne k).
        // Vi subtraherer et multiplum av k-te
        // rad fra radene nedenfor, nuller ut kolonner fra venstre
        for (int i=k+1; i<m; i++)
        {
            // Skal mult med denne og deretter trekke fra rad i
            // denne skal bli null og vi kan lagre faktoren her
            A[i][k] = A[i][k]/A[k][k];

            for (int j=k+1; j<m; j++) // m==n
            {
                // kolonnene til høyre for den som blir nullet ut
                A[i][j] = A[i][j] - A[i][k]*A[k][j];
            }
            // Den følgende linjen gjøres på permutert vektor
            // i solve(), bedre ved løsning av flere x=A^(-1)*b
            // siden LU-faktorisering kan brukes opp igjen
            // b[i] = b[i] - A[i][k]*b[k];
        }
    }
    lu_faktorisert = true;
}
// Determinanten til en diagonalmatrise er produktet av diagonalelementene
// Returnerer determinanten hvis LU-faktorisert, 0 ellers (hvilket ikke nødvendigvis er riktig
float Matrix4x4::determinant() const
{
    float diagonalProdukt = 1.0f;
    if (lu_faktorisert)
    {
        for (int i=0; i<m; i++)
            diagonalProdukt *= A[i][i];
        return diagonalProdukt;
    }
    return 0.0f;
}
/*
 *                      b0  b1  b2  b3       e1 e2 e3 e4
 *
 * |A00 A01 A02 A03|   |B00 B01 B02 B03|   | 1  0  0  0 |
 * |A10 A11 A12 A13| * |B10 B11 B12 B13| = | 0  1  0  0 |
 * |A20 A21 A22 A23|   |B20 B21 B22 B23|   | 0  0  1  0 |
 * |A30 A31 A32 A33|   |B30 B31 B32 B33|   | 0  0  0  1 |
 *
 * Fire ligninger:
 *  A b0 = e1
 *  A b1 = e2
 *  A b2 = e3
 *  A b3 = e4
 *
 * Hver av disse bestemmer en kolonne i B.
 * Siden AB=I er B=A sin inverse
 */
Matrix4x4 Matrix4x4::inverse() const
{
    Matrix4x4 H = *this;            // inverse() er const, så vi trenger en
    Vector4d perm(0,1,2,3);             // kopi å LU-faktorisere

    H.LU();
    Vector4d e1(1.0f, 0.0f, 0.0f, 0.0f);
    Vector4d e2(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4d e3(0.0f, 0.0f, 1.0f, 0.0f);
    Vector4d e4(0.0f, 0.0f, 0.0f, 1.0f);

    // Løser A*b0 = e1
    Vector4d e1_perm(e1[(int)perm[0]], e1[(int)perm[1]], e1[(int)perm[2]], e1[(int)perm[3]]);
    Vector4d x = H.solve(e1);//H.solve(e1_perm);
    Matrix4x4 M;
    M.set(0, x);

    // Løser A*b1 = e2
    Vector4d e2_perm(e2[(int)perm[0]], e2[(int)perm[1]], e2[(int)perm[2]], e2[(int)perm[3]]);
    x = H.solve(e2);//(e2_perm);
    M.set(1, x);

    // Løser A*b2 = e3
    Vector4d e3_perm(e3[(int)perm[0]], e3[(int)perm[1]], e3[(int)perm[2]], e3[(int)perm[3]]);
    x = H.solve(e3);//(e3_perm);
    M.set(2, x);

    // Løser A*b3 = e4
    Vector4d e4_perm(e4[(int)perm[0]], e4[(int)perm[1]], e4[(int)perm[2]], e4[(int)perm[3]]);
    x = H.solve(e4);//(e4_perm);
    M.set(3, x);

    return M;
}

// Sjekke m<M, n<N etc og sette dim
Matrix4x4 Matrix4x4::transpose() const
{
    Matrix4x4 T;
    for (auto i=0; i<m; i++)
        for (auto j=0; j<n; j++)
            T(i,j) = A[j][i];
    return T;
}

// Løser Ax=b, syntaks x = A.solve(b)
// Forutsetter at LU er gjort på A
Vector4d Matrix4x4::solve(Vector4d &b) const
{
    Vector4d x;
    Vector4d b_perm(b[permutasjon[0]], b[permutasjon[1]], b[permutasjon[2]], b[permutasjon[3]]);

    for (int k=0; k<n-1; k++) // m==n
        for (int i=k+1; i<n; i++)
            b_perm[i] = b_perm[i]-A[i][k]*b_perm[k];

    for (int i=n-1; i>=0; i--)
    {
        x[i] = b_perm[i];
        for (int j=i+1; j<n; j++)
            x[i] = x[i] - A[i][j]*x[j];
        x[i] = x[i]/A[i][i];
    }
    return x;
}

Vector4d Matrix4x4::operator * ( Vector4d& v) const
{
    Vector4d x;
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            x[i] += A[i][j]*v[j];
    return x;
}


void Matrix4x4::translate(float tx, float ty, float tz)
{
    // Lager translasjonssmatrise
    Matrix4x4 T;
    T.setToIdentity();
    //T.A[0][3] = tx;
    //T.A[1][3] = ty;
    //T.A[2][3] = tz;
    T(0,3) = tx;
    T(1,3) = ty;
    T(2,3) = tz;

    // Translerer - multipliserer med translasjonsmatrisen
    mult(T);
}

// Rotasjon om (1,0,0), (0,1,0) eller (0,0,1)
// Rotasjon om vilkårlig akse er beskrevet i Angel 4.10.4
void Matrix4x4::rotate(float degrees, float rx, float ry, float rz)
{
    // Lager rotasjonsmatrise
    Matrix4x4 R;
    R.setToIdentity();

    float pi = 4*std::atan(1.0);
    float rad = degrees*pi/180;
    float a = std::cos(rad);
    float b = std::sin(rad);
// Her kan vi ha både 1 og -1 som rx ry rz
    if (rx==0.0f && ry==0.0f && rz==1.0f)
    {
        //R.A[0][0] = a;
        //R.A[1][0] = b;
        //R.A[0][1] = -b;
        //R.A[1][1] = a;
        R(0,0) = a;
        R(1,0) = b;
        R(0,1) = -b;
        R(1,1) = a;
    }
    else if (rx==0.0f && ry==1.0f && rz==0.0f)
    {
        //R.A[0][0] = a;
        //R.A[2][0] = b;
        //R.A[0][2] = -b;
        //R.A[2][2] = a;
        R(0,0) = a;
        R(2,0) = b;
        R(0,2) = -b;
        R(2,2) = a;
    }
    if (rx==1.0f && ry==0.0f && rz==0.0f)
    {
        //R.A[1][1] = a;
        //R.A[2][1] = b;
        //R.A[1][2] = -b;
        //R.A[2][2] = a;
        R(1,1) = a;
        R(2,1) = b;
        R(1,2) = -b;
        R(2,2) = a;
    }
    // Roterer - multipliserer med rotasjonematrisen
    mult(R);
}

void Matrix4x4::scale(float sx, float sy, float sz)
{
    // Lager skaleringsmatrisen
    Matrix4x4 S;
    S.setToIdentity();
    S(0,0) = sx;
    S(1,1) = sy;
    S(2,2) = sz;
    // Skalerer - multipliserer med skaleringsmatrisen
    mult(S);
}
// Se Angel kapittel 5.7.2
void Matrix4x4::frustum(float left, float right, float bottom, float top, float near, float far)
{
    // Forutsetter a_ij initielt 0
    A[0][0] = 2*near/(right-left);
    A[1][1] = 2*near/(top-bottom);
    A[0][2] = (right+left)/(right-left);
    A[1][2] = (top+bottom)/(top-bottom);
    A[2][2] = -(far+near)/(far-near);
    A[2][3] = -2*far*near/(far-near);
    A[3][2] = -1.0f;
    A[3][3] = 0.0f;
    /*
    */
}
// Se Angel kapittel 5.7.2
void Matrix4x4::perspective(float fovy, float aspectRatio, float near, float far)
{
    float top = near*std::tan(fovy);
    float right = top/aspectRatio;
    A[0][0] = near/right;
    A[1][1] = near/top;
    A[2][2] = -(far+near)/(far-near);
    A[2][3] = -2*far*near/(far-near);
    A[3][2] = -1;
}
// Transformasjon fra xyz til uvn, se Angel(7th ed) 5.3.2
// up - orienteringen av kamera, trenger ikke ligge i viewplan
// u = v x n gir riktig retning i høyrehåndssystem.
// n er normal på viewplan, retning mot kamera (som z-aksen er default)
void Matrix4x4::lookAt(const Vector3d &eye, const Vector3d &at, const Vector3d &up)
{
    Matrix4x4 M;
    M.setToIdentity();
    Vector3d v = up;
    v.normalize();          // y-akse i kameraframe
    Vector3d n = eye-at;
    n.normalize();          // z-akse i kameraframe
    Vector3d u = v.cross(n);
    u.normalize();          // x-akse i kameraframe
    v = n.cross(u);
    v.normalize();
    M(0,0) = u.x;  M(0,1) = u.y;  M(0,2) = u.z;
    M(1,0) = v.x;  M(1,1) = v.y;  M(1,2) = v.z;
    M(2,0) = n.x;  M(2,1) = n.y;  M(2,2) = n.z;
    M.translate(-eye.x, -eye.y, -eye.z);
    //M.print();
    mult(M);
}
// internt er A M*N, så return &A[0][0];
// går ikke. Returner kolonnevis 4x4
const float *Matrix4x4::constData() const
{
    return &A[0][0];
}


}
