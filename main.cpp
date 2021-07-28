#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "canMessageHandler.h"
#include "mcp_can_rpi/mcp_can_rpi.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickView view;

    CANMessagesHandler canInterface;

    view.engine()->rootContext()->setContextProperty("canInterface", &canInterface);

    view.setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    view.show();

    return app.exec();
}
