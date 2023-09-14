QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    Ransac.cpp \
    main.cpp \
    Interface.cpp

HEADERS += \
    Interface.h \
    Ransac.h

FORMS += \
    Interface.ui

LIBS += -lqwt-qt5

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
