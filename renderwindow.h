#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include <vector>
#include "camera.h"
#include "light.h"
#include "vertex.h"
#include "xyz.h"
#include "trianglesurface.h"
#include "filesurface.h"
#include "matrix4x4.h"
#include "octahedronball.h"
#include "rollingball.h"
#include "bsplinecurve.h"

class QOpenGLContext;
class Shader;
class MainWindow;

/// This inherits from QWindow to get access to the Qt functionality and
/// OpenGL surface.
/// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
/// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;

    //    void error(const QString &msg);
    void makeRain();
    void spawnRollingBall();
private slots:
    void render();

private:
    void init();
    XYZ* xyz;
    OctahedronBall* oball{nullptr};
    TriangleSurface* surf2;
    VisualObject* surf;
    QOpenGLContext *mContext;
    bool mInitialized;
    gsml::Vector3d help;
    Camera* mCamera;
    std::vector<RollingBall*> Rain;
    std::vector<VisualObject*> mVisualObjects;
    BSplineCurve* mBSpline;
    Light* mLight{nullptr};
    int currentScene{1};

    Shader *mShaderProgram[2];
    GLuint mVAO;
    GLuint mVBO;

    gsml::Matrix4x4 * gsmMMatrix;  // nytt 23/1
    gsml::Matrix4x4 * gsmVMatrix;  // nytt 23/1
    gsml::Matrix4x4 * gsmPMatrix;  // nytt 23/1
    gsml::Vector3d mLightPosition;  // nytt 3/10/19
    QTimer *mRenderTimer;     //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the actual FPS

    MainWindow *mMainWindow;    //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void setupPlainShader(int shaderIndex);
    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};

    void setupPhongShader(int shaderIndex);
    GLint mMatrixUniform1{-1};
    GLint vMatrixUniform1{-1};
    GLint pMatrixUniform1{-1};
    GLint mTextureUniformPhong{-1};
    // GLint mUsingTextureUniform{-1};
    //light shader variables
    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};

    void makeObjects();
    void drawObjects();

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

    void itsRainingBalls(float ElapsedSeconds);

    float mCameraSpeed{0.5f};
    float mCameraRotateSpeed{0.1f};
    float mPlayerSpeed{0.05f};
    int mMouseXlast{0};
    int mMouseYlast{0};
    void checkCamInp();


    bool inpW{false};
    bool inpA{false};
    bool inpS{false};
    bool inpD{false};
    bool inpQ{false};
    bool inpE{false};
    bool inpRMB{false};

protected:
    //The QWindow that we inherit from has these functions to capture
    // mouse and keyboard. Uncomment to use
    //
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    //    void wheelEvent(QWheelEvent *event) override{}
};

#endif // RENDERWINDOW_H
