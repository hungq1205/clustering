QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clustermethod.cpp \
    cmeans.cpp \
    colormap.cpp \
    datautility.cpp \
    main.cpp \
    mainwindow.cpp \
    sscmeans.cpp \
    supervisorwindow.cpp

HEADERS += \
    clustermethod.h \
    cmeans.h \
    colormap.h \
    datautility.h \
    mainwindow.h \
    point.h \
    sscmeans.h \
    supervisorwindow.h

FORMS += \
    mainwindow.ui \
    supervisorwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
