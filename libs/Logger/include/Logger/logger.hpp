#ifndef QT_SSL_EXAMPLE_COMMON_LOGGER_H__
#define QT_SSL_EXAMPLE_COMMON_LOGGER_H__

#include <QObject>
#include <QFile>
#include <QDir>
#include <QString>
#include <QPointer>
#include <QTextStream>

enum LoggingLevel {
    Trace = 0,
    Debug,
    Info,
    Warning,
    Error,
    Fatal,
};

class logger: public QObject
{
    Q_OBJECT
    public:
        logger(int level, QString filename, QString dir = QDir::currentPath(), QObject* parent=nullptr);
        logger(int level, QString filename, QObject* parent=nullptr);
        virtual ~logger();

        int loggingLevel() const;
        void setLoggingLevel(int);

        QString logPath() const;
        void setLogPath(QString);

    public Q_SLOTS:
        void logMessage(QString _message, int level);

        void logTrace(QString   _message);
        void logDebug(QString   _message);
        void logInfo(QString    _message);
        void logWarning(QString _message);
        void logError(QString   _message);
        void logFatal(QString   _message);

    private:
        void openLogFile();

    protected:
        void write(const QString& msg);

        int         _level;
        QString     name;
        QString     dir;
        QFile       logFile;
        QTextStream theLog;
};

#endif //QT_SSL_EXAMPLE_COMMON_LOGGER_H__
