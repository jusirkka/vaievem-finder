TEMPLATE = app

TARGET = vaievem-finder
CONFIG += c++11
QT += qml quick sql positioning

SOURCES = src/main-desktop.cpp \
    src/timetable.cpp \
    src/database.cpp \
    src/unitwrappermodel.cpp

HEADERS = \
    src/timetable.h \
    src/database.h \
    src/unitwrappermodel.h

DISTFILES += qml/vaievem-finder.qml \
    qml/ScaleBar.qml \
    qml/MapPage.qml \
    qml/AboutPage.qml \
    qml/SettingsPage.qml \
    qml/SchedulePage.qml \
    qml/Python.qml \
    qml/BusStop.qml \
    qml/Tile.qml \
    qml/StateButton.qml \
    qml/Selection.qml \
    qml/Alpha.qml \
    qml/Omega.qml \
    qml/utils.js \
    qml/MyMap.qml \
    qml/ScheduleDelegate.qml \
    qml/CoverDelegate.qml \
    TODO.txt \
    qml/Runner.qml \
    qml/CoverPage.qml

