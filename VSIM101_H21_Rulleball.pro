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
    BSpline/BSpline0.txt \
    BSpline/BSpline1.txt \
    BSpline/BSpline11.txt \
    BSpline/BSpline16.txt \
    BSpline/BSpline17.txt \
    BSpline/BSpline21.txt \
    BSpline/BSpline22.txt \
    BSpline/BSpline25.txt \
    BSpline/BSpline26.txt \
    BSpline/BSpline27.txt \
    BSpline/BSpline28.txt \
    BSpline/BSpline29.txt \
    BSpline/BSpline3.txt \
    BSpline/BSpline31.txt \
    BSpline/BSpline35.txt \
    BSpline/BSpline36.txt \
    BSpline/BSpline38.txt \
    BSpline/BSpline39.txt \
    BSpline/BSpline4.txt \
    BSpline/BSpline40.txt \
    BSpline/BSpline41.txt \
    BSpline/BSpline42.txt \
    BSpline/BSpline43.txt \
    BSpline/BSpline46.txt \
    BSpline/BSpline47.txt \
    BSpline/BSpline48.txt \
    BSpline/BSpline49.txt \
    BSpline/BSpline5.txt \
    BSpline/BSpline50.txt \
    BSpline/BSpline51.txt \
    BSpline/BSpline52.txt \
    BSpline/BSpline53.txt \
    BSpline/BSpline54.txt \
    BSpline/BSpline55.txt \
    BSpline/BSpline56.txt \
    BSpline/BSpline57.txt \
    BSpline/BSpline58.txt \
    BSpline/BSpline6.txt \
    BSpline/BSpline60.txt \
    BSpline/BSpline61.txt \
    BSpline/BSpline62.txt \
    BSpline/BSpline64.txt \
    BSpline/BSpline65.txt \
    BSpline/BSpline69.txt \
    BSpline/BSpline7.txt \
    BSpline/BSpline70.txt \
    BSpline/BSpline71.txt \
    BSpline/BSpline72.txt \
    BSpline/BSpline73.txt \
    BSpline/BSpline75.txt \
    BSpline/BSpline77.txt \
    BSpline/BSpline78.txt \
    BSpline/BSpline79.txt \
    BSpline/BSpline8.txt \
    BSpline/BSpline80.txt \
    BSpline/BSpline81.txt \
    BSpline/BSpline82.txt \
    BSpline/BSpline83.txt \
    BSpline/BSpline84.txt \
    BSpline/BSpline85.txt \
    BSpline/BSpline86.txt \
    BSpline/BSpline87.txt \
    BSpline/BSpline88.txt \
    BSpline/BSpline9.txt \
    BSpline/BSpline90.txt \
    BSpline/BSpline91.txt \
    BSpline/BSpline92.txt \
    BSpline/BSpline93.txt \
    BSpline/BSpline94.txt \
    BSpline/BSpline95.txt \
    BSpline/BSpline96.txt \
    BSpline/BSpline97.txt \
    BSpline/BSpline98.txt \
    BSpline/BSpline99.txt \
    dagfragment.frag \
    dagvertex.vert \
    phongfragment.frag \
    phongvertex.vert
