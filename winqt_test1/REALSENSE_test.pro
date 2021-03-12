QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#INCLUDEPATH += F:/Qt_pro_include/G_drive/intel_reslsense_SDK2.16.5/include
#HEADERS += F:/Qt_pro_include/G_drive/intel_reslsense_SDK2.16.5/src/
#LIBS += F:/Qt_pro_include/G_drive/intel_reslsense_SDK2.16.5/lib/realsense2.lib

INCLUDEPATH += $$quote(C:\Program Files (x86)\Intel RealSense SDK 2.0\include)
#HEADERS += F:/Qt_pro_include/G_drive/intel_reslsense_SDK2.16.5/src/
LIBS += $$quote(C:\Program Files (x86)\Intel RealSense SDK 2.0\lib\x64\realsense2.lib)

# cuda opencv 4.1
INCLUDEPATH += $$quote(D:\lib\opencv_4.1.1\include)\
               $$quote(D:\lib\opencv_4.1.1\include\opencv2)
LIBS +=  D:\lib\opencv_4.1.1\x64\vc15\lib\opencv_world411.lib #D:\lib\opencv3.4.2\opencv\build\x64\vc14\lib\opencv_world342.lib\

INCLUDEPATH += $$quote(C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.1\include)\

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
