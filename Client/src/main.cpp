#include <QApplication>
#include "Controller.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QFile styleFile(":/qss/global");
    QString style;
    if (styleFile.open(QFile::ReadOnly)) {
        style = QLatin1String(styleFile.readAll());
    } else {
        qDebug() << "Cannot load styles";
    }
    app.setStyleSheet(style);

    Settings settings;

    Controller theUserInterface(settings);

    theUserInterface.show();
    return app.exec();
}
