#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QtQml>
#include <QDebug>
#include <QQuickView>

#include "timetable.h"
#include "unitwrappermodel.h"


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext *ctxt = engine.rootContext();

    Timetable timetable;
    ctxt->setContextProperty("timetableModel", &timetable);
    UnitWrapperModel coverModel(&timetable);
    ctxt->setContextProperty("coverModel", &coverModel);

    engine.load("qml/vaievem-finder.qml");

    return app.exec();
}
