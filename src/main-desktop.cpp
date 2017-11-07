#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QtQml>
#include <QDebug>
#include <QQuickView>

#include <sailfishapp.h>

#include "timetable.h"


int main(int argc, char *argv[])
{

    // Set up QML engine.
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    // QGuiApplication app(argc, argv);

    QScopedPointer<QQuickView> view(SailfishApp::createView());

    // QQmlApplicationEngine engine;
    // QQmlContext *ctxt = engine.rootContext();

    Timetable timetable;
    view->rootContext()->setContextProperty("timetableModel", &timetable);
    // ctxt->setContextProperty("timetableModel", &timetable);

    view->setSource(SailfishApp::pathTo("qml/harbour-vaievem.qml"));
    // engine.load("qml/harbour-vaievem.qml");

    view->show();
    return app->exec();

    // return app.exec();
}
