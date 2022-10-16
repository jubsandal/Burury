#include <QApplication>
#include "Burury.hpp"

void applyStypes(QApplication &app, QFile stylesFile) {
        QString style;
        if (stylesFile.open(QFile::ReadOnly)) {
                style = QLatin1String(stylesFile.readAll());
        } else {
                qWarning() << "Cannot load styles";
        }
        app.setStyleSheet(style);
}

int main(int argc, char* argv[])
{
        QApplication app(argc, argv);

        applyStypes(app, QFile(":/qss/global"));
        Settings settings;
        // read settings
        Burury burury(settings);
        burury.show();
        return app.exec();
}
