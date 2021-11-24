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
    camera.h \
    mainwindow.h \
    octahedronball.h \
    renderwindow.h \
    rollingball.h \
    shader.h \
    surf.h \
    trianglesurface.h \
    visualobject.h \
    xyz.h

SOURCES += \
    Matrix4x4/matrix4x4.cpp \
    Matrix4x4/vertex.cpp \
    camera.cpp \
    main.cpp \
    mainwindow.cpp \
    octahedronball.cpp \
    renderwindow.cpp \
    rollingball.cpp \
    shader.cpp \
    surf.cpp \
    trianglesurface.cpp \
    visualobject.cpp \
    xyz.cpp

DISTFILES += \
    Phongfragment.frag \
    Phonvertex.vert \
    dagfragment.frag \
    dagvertex.vert
