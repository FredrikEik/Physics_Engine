#include "renderwindow.h"
#include <QTimer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include "las.h"

#include "shader.h"
#include "mainwindow.h"

#include "rollingball.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)
{
    //creating light position
    help.x = 0; help.y = -5; help.z = 3;
    mLightPosition.x = 5.2f;
    mLightPosition.y = 5.2f;
    mLightPosition.z = 2.0f;
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);

    //Make the OpenGL context
    mContext = new QOpenGLContext(this);

    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
    gsml::Vector4d v{1,2,3,4};
    qDebug() << v[0] <<v[1] << v[3] << v[2];

    gsmMMatrix = new gsml::Matrix4x4;
    gsmMMatrix->setToIdentity();
    gsmVMatrix = new gsml::Matrix4x4;
    gsmVMatrix->setToIdentity();
    gsmPMatrix = new gsml::Matrix4x4;
    gsmPMatrix->setToIdentity();
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    //glDeleteVertexArrays( 1, &mVAO );
    //glDeleteBuffers( 1, &mVBO );
}

/// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Connect the gameloop timer to the render function:
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions

    initializeOpenGLFunctions();

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);     //draws only front side of models - usually what you want -
    glClearColor(0.4f, 0.4f, 0.4f,1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT
    glClearColor(1.0f, 1.0f, 1.0f,1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.

    mShaderProgram = new Shader("../VSIM101_H21_Rulleball_0/dagvertex.vert", "../VSIM101_H21_Rulleball_0/dagfragment.frag");

    //********************** Making the object to be drawn **********************

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //enable the matrixUniform
    // NB: enable in shader and in render() function also to use matrix
    // endret/nytt 23/1
    mMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "matrix" );
    mPMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "pmatrix" );
    mVMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "vmatrix" );
    mLightPositionUniform = glGetUniformLocation( mShaderProgram->getProgram(), "light_position" );
    glBindVertexArray( 0 );

    xyz.mMatrix.translate(1,1,1);
    xyz.init(mMatrixUniform);





}

void RenderWindow::SimulatePlane()
{
    //lager et plane fra txt fil og legger en simulasjon av baller
    plane = new LAS("../VSIM101_H21_Rulleball_0/datasett/test_las.txt");
    plane->init(mMatrixUniform);
    mGameObjects.push_back(plane);

    for(auto i{0}; i<10; i++)
    {
        ball = new RollingBall(2);
        ball->setSurface2(plane);
        SimulateBalls = true;

        ball->init(mMatrixUniform);
        planeSim.push_back(ball);
        mGameObjects.push_back(ball);
    }

}

void RenderWindow::LagTrekant()
{
    //lager en trekant som i oblig 3 og simulerer en balls bevegelse over den
    surf2 = new TriangleSurface("../VSIM101_H21_Rulleball_0/datasett/totrekanter.txt");
    surf2->mMatrix.rotate(90, 0, 0, 0);
    surf2->init(mMatrixUniform);
    ball = new RollingBall(3);
    dynamic_cast<RollingBall*>(ball)->setSurface(surf2);
    SimulateBalls = false;
    ball->init(mMatrixUniform);
    mBalls.push_back(ball);
    mGameObjects.push_back(surf2);
    mGameObjects.push_back(ball);


}

///Called each frame - doing the rendering
void RenderWindow::render()
{
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    // initializeOpenGLFunctions();    //must call this every frame it seems...

    // to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // what shader to use
    glUseProgram(mShaderProgram->getProgram() );
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5);
    // what object to draw

    // Since our shader uses a matrix and we rotate the triangle, we send the current matrix here
    // must be here to update each frame - if static object, it could be set only once

    gsmPMatrix->setToIdentity();
    gsmVMatrix->setToIdentity();
    gsmPMatrix->perspective(90, 4.0/3.0, 0.1, 100.0);

    gsml::Vector3d eye{help.x,help.y,help.z};
    gsml::Vector3d at{0 ,0 , 0};
    gsml::Vector3d up{0,0,1};
    gsmVMatrix->lookAt(eye, at, up);

    glUniformMatrix4fv( mPMatrixUniform, 1, GL_TRUE, gsmPMatrix->constData());
    glUniformMatrix4fv( mVMatrixUniform, 1, GL_TRUE, gsmVMatrix->constData());
    glUniform3f(mLightPositionUniform, mLightPosition.x, mLightPosition.y, mLightPosition.z);

    //sjekker om isplaying er true
    //hvis true, så sjekker den sim ball er false og så plane sim
    if(isPlaying)
    {
        if(!SimulateBalls)
        mBalls.back()->barycentricCords(0.017f);
        else
        {
            if(!planeSim.empty()){
                for(auto i{0}; i<static_cast<int>(planeSim.size()); i++)
                {
                    planeSim[i]->moveAlongLAs(0.017f);

                }}
        }

    }
    //tegner objectene
    for(unsigned int i{0}; i < mGameObjects.size(); i++)
    {
        mGameObjects[i]->draw();
    }

    xyz.draw();
    // checkForGLerrors() because that takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    // using our expanded OpenGL debugger to check if everything is OK.
    // checkForGLerrors();

    // Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);
}

void RenderWindow::toggleWireframe(bool buttonState)
{
    if (buttonState)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //turn on wireframe mode
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
        glEnable(GL_CULL_FACE);
    }
}

//This function is called from Qt when window is exposed (shown)
//and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact..)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//The way this is set up is that we start the clock before doing the draw call,
//and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30) //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7) +
                                                  "  |  Vertices drawn: :" + QString::number(mVerticesDrawn));
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}


/// Uses QOpenGLDebugLogger if this is present
/// Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

/// Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            qDebug() << "This system can not use QOpenGLDebugLogger, so we revert to glGetError()";

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        mMainWindow->close();
    }
    if (event->key() == Qt::Key_A)
        help.x -= 0.3;
    if (event->key() == Qt::Key_D)
        help.x += 0.3;
    if (event->key() == Qt::Key_W)
        help.y += 0.3;
    if (event->key() == Qt::Key_S)
        help.y -= 0.3;
    if (event->key() == Qt::Key_Q)
        help.z += 0.3;
    if (event->key() == Qt::Key_Z)
        help.z -= 0.3;
}
