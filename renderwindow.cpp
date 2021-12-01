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

#include "shader.h"
#include "mainwindow.h"



RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)
{
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


}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
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
    //glClearColor(1.0f, 1.0f, 1.0f,1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.

    mLight = new Light();
    mCamera = new Camera;
    mShaderProgram[0] = new Shader("../VSIM101_H21_Rulleball_0/dagvertex.vert", "../VSIM101_H21_Rulleball_0/dagfragment.frag");
    qDebug() << "Plain shader program id: " << mShaderProgram[0]->getProgram();
    mShaderProgram[1] = new Shader("../VSIM101_H21_Rulleball_0/phongvertex.vert", "../VSIM101_H21_Rulleball_0/phongfragment.frag");
    qDebug() << "Phong shader program id: " << mShaderProgram[1]->getProgram();
    setupPlainShader(0);
    setupPhongShader(1);

    //********************** Making the object to be drawn **********************

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );


    glBindVertexArray( 0 );


    mCamera->pitch(70);
    mCamera->setPosition(gsml::Vector3d(0.f, -100.f, -30.f));
    mCamera->yaw(180);

    makeObjects();



}

void RenderWindow::makeObjects()
{


    mLight->init(mMatrixUniform);
    mLight->move(10,10,10);
    mVisualObjects.push_back(mLight);

    xyz = new XYZ;
    xyz->init(mMatrixUniform);
    mVisualObjects.push_back(xyz);

    surf = new FlateFil("../VSIM101_H21_Rulleball_0/test_las.txt");
    surf->init(mMatrixUniform1);
    mVisualObjects.push_back(surf);

    mContour = new MarchingSquare(surf);
    mContour->init(mMatrixUniform);
    mVisualObjects.push_back(mContour);

    oball = new OctahedronBall(1);
    RollingBall* ball{nullptr};

    for(auto i{0}; i<2; i++)
    {
        mBSpline = new BSplineCurve(i);
        mBSpline->init(mMatrixUniform);
        mVisualObjects.push_back(mBSpline);

        ball = new RollingBall(i);
        ball->setMesh(oball->getMesh());
        ball->setSurface(surf);
        ball->init(mMatrixUniform);
        Rain.push_back(ball);
        mVisualObjects.push_back(ball);

    }

    surf2 = new TriangleSurface("../VSIM101_H21_Rulleball_0/totrekanter.txt");
    surf2->mScene = 1;
    surf2->init(mMatrixUniform);
    mVisualObjects.push_back(surf2);


    ball = new RollingBall(3);
    ball->setMesh(oball->getMesh());
    ball->setSurface(surf2);
    ball->mScene = 1;
    ball->init(mMatrixUniform);
    mVisualObjects.push_back(ball);
}

void RenderWindow::drawObjects()
{
    //This block sets up the uniforms for the shader used in the material
    //Also sets up texture if needed.
    int viewMatrix{-1};
    int projectionMatrix{-1};
    int modelMatrix{-1};
    //Draws the objects
    for(int i{0}; i <static_cast<int>(mVisualObjects.size()); i++)
    {
        glUseProgram(mShaderProgram[mVisualObjects[i]->getShaderID()]->getProgram());

        if (mVisualObjects[i]->getShaderID() == 0) //PlainShader
        {
            viewMatrix = vMatrixUniform;
            projectionMatrix = pMatrixUniform;
            modelMatrix = mMatrixUniform;
        }
        else if (mVisualObjects[i]->getShaderID() == 1)//TextureShader
        {
            viewMatrix = vMatrixUniform1;
            projectionMatrix = pMatrixUniform1;
            modelMatrix = mMatrixUniform1;

            glUniform3f(mLightPositionUniform, mLight->getMatrix().getColumn(3).x(), mLight->getMatrix().getColumn(3).y(), mLight->getMatrix().getColumn(3).z());
            glUniform3f(mCameraPositionUniform, mCamera->position().x, mCamera->position().y, mCamera->position().z);
            glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
        }


        if(mVisualObjects[i]->mScene == currentScene)
        {
            //send data to shader
            glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, mCamera->mViewMatrix.constData());
            glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, mCamera->mProjectionMatrix.constData());
            glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mVisualObjects[i]->getMatrix().constData());

            glBindVertexArray(mVisualObjects[i]->mMesh->mVAO );
            glDrawArrays(mVisualObjects[i]->mMesh->mDrawType, 0, mVisualObjects[i]->get_vertices().size());
            glBindVertexArray(0);
        }
    }

    glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, mLight->getMatrix().constData());
    glBindVertexArray(mLight->mMesh->mVAO );
    glDrawArrays(mLight->mMesh->mDrawType, 0, mLight->get_vertices().size());
    glBindVertexArray(0);

    if(currentScene == 0){
        if(!Rain.empty()){
            for(auto i{0}; i<static_cast<int>(Rain.size()); i++)
            {
                Rain[i]->move(0.017f);
            }}}
    else
        mVisualObjects.back()->move(0.017f);
}


///Called each frame - doing the rendering
void RenderWindow::render()
{
    checkCamInp();
    mCamera->update();

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    // initializeOpenGLFunctions();    //must call this every frame it seems...
    // to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawObjects();

    // checkForGLerrors() because that takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    // using our expanded OpenGL debugger to check if everything is OK.
    // checkForGLerrors();

    // Qt require us to call this swapBuffers() -function.a
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);
}

void RenderWindow::makeRain()
{
    currentScene = 0;
    mCamera->setPosition(gsml::Vector3d(0.f, -100.f, -50.f));

}

void RenderWindow::spawnRollingBall()
{
    currentScene = 1;
    static_cast<RollingBall*>(mVisualObjects.back())->p = new Physics;
    mVisualObjects.back()->move(1,1,5);
    mCamera->setPosition(gsml::Vector3d(0.f, -15.f, -10.f));
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


    mCamera->calculateProjectionMatrix();
}


void RenderWindow::setupPhongShader(int shaderIndex)
{
    mMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );

    mLightColorUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightColor" );
    mObjectColorUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "objectColor" );
    mAmbientLightStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "ambientStrengt" );
    mLightPositionUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularStrength" );
    mSpecularExponentUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularExponent" );
    mLightPowerUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightStrengt" );
    mCameraPositionUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "cameraPosition" );
    mTextureUniformPhong = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}


void RenderWindow::setupPlainShader(int shaderIndex)
{
    mMatrixUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
    mLightPositionUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "light_position");
}


//The way this is set up is that we start the clock before doing the draw call,
//and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30) //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
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

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (inpRMB)
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;

        if (mMouseXlast != 0)
            mCamera->yaw(mCameraRotateSpeed * mMouseXlast);
        if (mMouseYlast != 0)
            mCamera->pitch(mCameraRotateSpeed * mMouseYlast);
    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();
}

void RenderWindow::checkCamInp()
{
    mCamera->setSpeed(0.f);

    if(inpRMB == true)
    {
        if (inpA)
            mCamera->moveRight(mCameraSpeed);
        if (inpD)
            mCamera->moveRight(-mCameraSpeed);
        if (inpW)
            mCamera->setSpeed(mCameraSpeed);
        if (inpS)
            mCamera->setSpeed(-mCameraSpeed);
        if (inpQ)
            mCamera->updateHeigth(mCameraSpeed);
        if (inpE)
            mCamera->updateHeigth(-mCameraSpeed);
        if (inpC==true)
            mCamera->pitch(2);
        if (inpX==true)
            mCamera->yaw(2);

    }
    if(inpLMB == true)
    {
        if (inpC==true)
            mCamera->pitch(-2);
        if (inpX==true)
            mCamera->yaw(-2);
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    mCamera->setSpeed(0.f);
    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        mMainWindow->close();
    }
    if (event->key() == Qt::Key_A)
        inpA = true;
    if (event->key() == Qt::Key_D)
        inpD = true;
    if (event->key() == Qt::Key_W)
        inpW = true;
    if (event->key() == Qt::Key_S)
        inpS = true;
    if (event->key() == Qt::Key_Q)
        inpQ = true;
    if (event->key() == Qt::Key_E)
        inpE = true;
    if (event->key() == Qt::Key_C)
        inpC = true;
    if (event->key() == Qt::Key_X)
        inpX = true;


}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A)
        inpA = false;
    if (event->key() == Qt::Key_D)
        inpD = false;
    if (event->key() == Qt::Key_W)
        inpW = false;
    if (event->key() == Qt::Key_S)
        inpS = false;
    if (event->key() == Qt::Key_Q)
        inpQ = false;
    if (event->key() == Qt::Key_E)
        inpE = false;
    if (event->key() == Qt::Key_C)
        inpC = false;
    if (event->key() == Qt::Key_X)
        inpX = false;

}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        inpRMB = true;
    if (event->button() == Qt::LeftButton)
        inpLMB = true;
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        inpRMB = false;
    if (event->button() == Qt::LeftButton)
        inpLMB = false;
}
