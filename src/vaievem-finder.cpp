#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QtQml>
#include <QDebug>
#include <QQuickView>

#include <sailfishapp.h>

#include "timetable.h"
#include "unitwrappermodel.h"

int main(int argc, char *argv[])
{

    // Set up QML engine.
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    // remove stutter
    app->setOrganizationName("");
    app->setOrganizationDomain("");

    QScopedPointer<QQuickView> view(SailfishApp::createView());

    Timetable timetable;
    view->rootContext()->setContextProperty("timetableModel", &timetable);
    UnitWrapperModel coverModel(&timetable);
    view->rootContext()->setContextProperty("coverModel", &coverModel);

    view->setSource(SailfishApp::pathTo("qml/harbour-vaievem-finder.qml"));

    view->show();
    return app->exec();
}
