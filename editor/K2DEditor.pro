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

INCLUDEPATH += F:/KEngineLab/Lab/CMakeBase/extlibs/headers
INCLUDEPATH += F:/KEngineLab/Lab/CMakeBase/include
INCLUDEPATH += F:/KEngineLab/Lab/CMakeBase/include/KiteMeta


win32:LIBS += F:/KEngineLab/Lab/CMakeBase/extlibs/lib-vc/win32/lua_dll.lib
win32:LIBS += C:/Users/_M1L4D_/Desktop/cmtest/lib/Debug/Kite2Dd.lib

SOURCES += main.cpp\
        mainwindow.cpp \
    gridscene.cpp \
	expander.cpp \
    frmnewproj.cpp\
    comproperty.cpp\
	resourcetree.cpp\
	objecttree.cpp\
	componenttree.cpp

HEADERS  += mainwindow.h \
    shared.h \
    gridscene.h \
	expander.h \
    frmnewproj.h\
    comproperty.h\
	resourcetree.h\
	objecttree.h\
	componenttree.h

RESOURCES += \
    res.qrc

FORMS += \
    frmnewproj.ui
