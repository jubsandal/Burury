#include <QApplication>

#include "Bururyd.hpp"

int main(int argc, char *argv[])
{
        QApplication app(argc, argv);
        Settings settings;

        // read settings

        if (!settings.useSsl) {
                qWarning() << QString("SSL Disabled!");
        }
        BururyDaemon daemon(settings);
        return app.exec();
}
