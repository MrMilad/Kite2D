#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T23:22:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = K2DEditor
TEMPLATE = app
CONFIG += console

DEFINES += KITE_DEV_DEBUG
DEFINES += KITE_EDITOR

INCLUDEPATH += F:/KEngineLab/Lab/CMakeBase/extlibs/headers
INCLUDEPATH += F:/KEngineLab/Lab/CMakeBase/include
INCLUDEPATH += F:/KEngineLab/Lab/CMakeBase/include/KiteMeta


win32:LIBS += opengl32.lib
win32:LIBS += F:/KEngineLab/Lab/CMakeBase/extlibs/lib-vc/win32/lua_dll.lib
win32:LIBS += F:/KEngineLab/Lab/CMakeBase/extlibs/lib-vc/win32/SDL2.lib
win32:LIBS += F:/KEngineLab/Lab/CMakeBase/extlibs/lib-vc/win32/SDL2main.lib
win32:LIBS += C:/Users/_M1L4D_/Desktop/cmtest/lib/Debug/Kite2Dd.lib

SOURCES += main.cpp\
    mainwindow.cpp \
    gridscene.cpp \
    expander.cpp \
    frmnewproj.cpp\
    comproperty.cpp\
    resourcedock.cpp\
    objectdock.cpp\
    codeeditor.cpp\
    componentdock.cpp\
    maintab.cpp\
    completer.cpp\
    kiteinfo.cpp\
    highlighter.cpp \
    frmprojsettings.cpp \
    executer.cpp\
    outputdock.cpp \
    frmexeorder.cpp \
    frmaddobj.cpp \
    frmnewres.cpp

HEADERS  += mainwindow.h \
    shared.h \
    gridscene.h \
    expander.h \
    frmnewproj.h\
    comproperty.h\
    resourcedock.h\
    objectdock.h\
    codeeditor.h\
    componentdock.h\
    maintab.h\
    completer.h\
    kiteinfo.h\
    highlighter.h \
    frmprojsettings.h \
    executer.h\
    outputdock.h \
    frmexeorder.h \
    frmaddobj.h \
    frmnewres.h

RESOURCES += \
    res.qrc

FORMS += \
    frmnewproj.ui \
    frmprojsettings.ui \
    frmexeorder.ui \
    frmaddobj.ui \
    frmnewres.ui
