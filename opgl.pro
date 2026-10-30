# D:\Qt\Qt5.12.2\Tools\mingw730_64\bin\mingw32-make.exe
QT += core gui widgets concurrent multimedia
CONFIG += c++11 resources_big
QMAKE_CXXFLAGS += -Wa,-mbig-obj -O1

SOURCES += \
    main.cpp \
    openglwidget.cpp \
    dt.cpp \
    aud.cpp \
#    conv.cpp

HEADERS += \
    openglwidget.h \
    dt.h \
    aud.h \
#    conv.h

#DEFINES += usedlib
#SOURCES += dlibfr.cpp
#HEADERS += dlibfr.h
#LIBS += "D:\depend\dlib\lib\libdlib.a"

CVDIR = C:/code/pkg/build-opencv-4.2.0-Desktop_Qt_5_14_2_MinGW_32_bit-Debug/install

INCLUDEPATH += \
$$CVDIR\include \
$$CVDIR\include\opencv2 \
D:\depend\dlib\include

LIBS += \
$$CVDIR/x64/mingw/bin/libopencv_core420d.dll \
#$$CVDIR/x64/mingw/bin/libopencv_video420d.dll \
#$$CVDIR/x64/mingw/bin/libopencv_videoio420d.dll \
$$CVDIR/x64/mingw/bin/libopencv_highgui420d.dll \
$$CVDIR/x64/mingw/bin/libopencv_imgproc420d.dll \
$$CVDIR/x64/mingw/bin/libopencv_imgcodecs420d.dll

win32:LIBS += -lOpengl32

RC_ICONS = logo.ico

#CMU Sphinx
