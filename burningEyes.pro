#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T11:15:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = burningEyes
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    irisrecognition.cpp \
    segment.cpp \
    normlize.cpp \
    gaborfilter.cpp \
    match.cpp

HEADERS  += mainwindow.h \
    irisrecognition.h \
    segment.h \
    normlize.h \
    gaborfilter.h \
    match.h

FORMS    += mainwindow.ui

INCLUDEPATH+=E:\Qt\opencv244-remaked\include\opencv\
                    E:\Qt\opencv244-remaked\include\opencv2\
                    E:\Qt\opencv244-remaked\include

LIBS+=E:\Qt\opencv244-remaked\lib\libopencv_calib3d244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_contrib244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_core244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_features2d244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_flann244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_gpu244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_highgui244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_imgproc244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_legacy244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_ml244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_objdetect244.dll.a\
        E:\Qt\opencv244-remaked\lib\libopencv_video244.dll.a
