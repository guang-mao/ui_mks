QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    actuatorpanel.cpp \
    api_uartcomm.c \
    commandworker.cpp \
    ifacelistupdater.cpp \
    main.cpp \
    mainwindow.cpp \
    mks.equipment.actuator.actuateCommand.c \
    serial_port.cpp \
    serialportmanager.cpp \
    setup_window.cpp \

HEADERS += \
    ThreadSafeQueue.h \
    actuatorpanel.h \
    api_uartcomm.h \
    commandworker.h \
    ifacelistupdater.h \
    mainwindow.h \
    mks.equipment.actuator.actuateCommand.h \
    serial_port.h \
    serialportmanager.h \
    setup_window.h \

FORMS += \
    mainwindow.ui \
    setup_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
