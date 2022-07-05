#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include "QtCalculator.h"

int main (int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute (Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app (argc, argv);

    app.setOrganizationName ("QML Calculator Company");
    app.setOrganizationDomain ("qmlcalculator.com");
    app.setApplicationName ("QML Calculator");

    QtCalculator sec;

    QQmlApplicationEngine engine;
    const QUrl url (QStringLiteral ("qrc:/main.qml"));

    QObject::connect (&engine, &QQmlApplicationEngine::objectCreated,
                      &app, [url, &sec](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl) {
            QCoreApplication::exit (-1);
        } else {
            sec.setInvoker (obj);
        }
    }, Qt::QueuedConnection);

    engine.rootContext ()->setContextProperty ("qtcalculator", &sec);

    engine.load (url);
    return app.exec ();
}
