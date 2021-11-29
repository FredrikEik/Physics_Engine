QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++11
INCLUDEPATH += .\Matrix4x4\
TARGET      = Rulleball

FORMS += \
    mainwindow.ui

HEADERS += \
    Matrix4x4/matrix4x4.h \
    Matrix4x4/vector3d.h \
    Matrix4x4/vertex.h \
    bsplinecurve.h \
    camera.h \
    flatefil.h \
    light.h \
    mainwindow.h \
    marchingsquare.h \
    octahedronball.h \
    renderwindow.h \
    rollingball.h \
    shader.h \
    trianglesurface.h \
    visualobject.h \
    xyz.h

SOURCES += \
    Matrix4x4/matrix4x4.cpp \
    Matrix4x4/vertex.cpp \
    bsplinecurve.cpp \
    camera.cpp \
    flatefil.cpp \
    light.cpp \
    main.cpp \
    mainwindow.cpp \
    marchingsquare.cpp \
    octahedronball.cpp \
    renderwindow.cpp \
    rollingball.cpp \
    shader.cpp \
    trianglesurface.cpp \
    visualobject.cpp \
    xyz.cpp

DISTFILES += \
    dagfragment.frag \
    dagvertex.vert \
    phongfragment.frag \
    phongvertex.vert
