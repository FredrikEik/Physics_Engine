#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "xyz.h"
#include "trianglesurface.h"
#include "matrix4x4.h"
#include "rollingball.h"
#include "flatefil.h"
#include "camera.h"
#include "bspinecurve.h"

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

private slots:
    void render();

private:
    void init();
    XYZ xyz;
    TriangleSurface surf;
    TriangleSurface* surf2;
    QOpenGLContext *mContext;
    bool mInitialized;
    RollingBall* ball;
    VisualObject *Flate;
    Camera* mCamera;
    std::vector<RollingBall*> Rain;
    void makeRain();
    BSpineCurve *mBSpline;




    Shader *mShaderProgram;
    GLint  mMatrixUniform;
    GLint mPMatrixUniform;  // nytt 23/1
    GLint mVMatrixUniform;  // nytt 23/1
    GLint mLightPositionUniform;   // nytt 3/10/19
    GLuint mVAO;
    GLuint mVBO;

    gsml::Matrix4x4 * gsmMMatrix;  // nytt 23/1
    gsml::Matrix4x4 * gsmVMatrix;  // nytt 23/1
    gsml::Matrix4x4 * gsmPMatrix;  // nytt 23/1
    gsml::Vector3d mLightPosition;  // nytt 3/10/19
    QTimer *mRenderTimer;     //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the actual FPS


    void checkCamInp();
    float mCameraSpeed{0.5f};
    float mCameraRotateSpeed{0.05f};
    float mPlayerSpeed{0.05f};
    int mMouseXlast{0};
    int mMouseYlast{0};

    bool inpW{false};
    bool inpA{false};
    bool inpS{false};
    bool inpD{false};
    bool inpQ{false};
    bool inpE{false};
    bool inpC{false};
    bool inpX{false};
    bool inpRMB{false};
    bool inpLMB{false};





    MainWindow *mMainWindow;    //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

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
