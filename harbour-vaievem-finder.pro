TEMPLATE = app

TARGET = harbour-vaievem-finder

CONFIG += c++11 sailfishapp
QT += qml quick sql positioning

SOURCES = src/vaievem-finder.cpp \
    src/timetable.cpp \
    src/unitwrappermodel.cpp \
    src/database.cpp

HEADERS = \
    src/timetable.h \
    src/unitwrappermodel.h \
    src/database.h
