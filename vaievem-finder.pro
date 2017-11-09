TEMPLATE = app

TARGET = vaievem-finder
CONFIG += c++11
QT += qml quick sql positioning

SOURCES = src/main-desktop.cpp \
    src/timetable.cpp \
    src/database.cpp

HEADERS = \
    src/timetable.h \
    src/database.h

DISTFILES += qml/vaievem-finder.qml \
    qml/Cover.qml \
    qml/MapPage.qml \
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
    TODO.txt

