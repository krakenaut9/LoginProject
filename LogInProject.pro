QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutwindow.cpp \
    accounteditorwindow.cpp \
    chpasswindow.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    manageaccountswindow.cpp \
    manageusers.cpp \
    technologieswindow.cpp

HEADERS += \
    aboutwindow.h \
    accounteditorwindow.h \
    chpasswindow.h \
    loginwindow.h \
    mainwindow.h \
    manageaccountswindow.h \
    manageusers.h \
    pch.h \
    technologieswindow.h

FORMS += \
    aboutwindow.ui \
    accounteditorwindow.ui \
    chpasswindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    manageaccountswindow.ui \
    technologieswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
