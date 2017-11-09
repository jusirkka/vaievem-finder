#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QtQml>
#include <QDebug>
#include <QQuickView>

#include "timetable.h"


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext *ctxt = engine.rootContext();

    Timetable timetable;
    ctxt->setContextProperty("timetableModel", &timetable);

    engine.load("qml/vaievem-finder.qml");

    return app.exec();
}
