#include "Logger/logger.hpp"

#include <QDateTime>
#include <QString>

logger::logger(int level, QString _name, QObject* _parent)
    : logger(level, _name, QDir::currentPath(), _parent)
{
}

logger::logger(int level, QString _name, QString _dir, QObject* _parent)
    : QObject(_parent),
        name(_name),
        dir(_dir),
        _level(level)
{
    openLogFile();
    logDebug(tr("Starting log"));
}

logger::~logger()
{
    logDebug(tr("Stopping log"));
    if (logFile.isOpen() == true)
    {
        logFile.close();
    }
}

void
logger::openLogFile()
{
    QDateTime utcnow = QDateTime::currentDateTimeUtc();
    QString filename = QString("%2_%1.log").arg(utcnow.toString("yyyymmdd_HHmmss-zzz")).arg(name);

    if (!QDir(dir).exists()) {
        QFileInfo dir_info(dir);
        if (!dir_info.isWritable() && !QDir().mkdir(dir)) {
            throw "Cannot create log in " + dir; //TODO
        }
    }

    logFile.setFileName(dir + QDir::separator() + filename);
    logFile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Unbuffered);
    theLog.setDevice(&logFile);
}

int  logger::loggingLevel() const       { return _level; }
void logger::setLoggingLevel(int level) { _level = level; }

//its properly???
QString logger::logPath() const { return dir+QDir::separator()+logFile.fileName(); }

void
logger::setLogPath(QString path)
{
    logFile.close();
    name = QFileInfo(path).fileName();
    dir  = QDir(path).dirName();
    openLogFile();
}

void logger::logMessage(QString _message, int level)
{
    if (level > Fatal || level < Trace) {
        qDebug() << tr("Undefined log level: %1").arg(_level);
        return;
    }

    if (level >= _level) {
        write(_message);
    }
}

void
logger::write(const QString& msg)
{
    if (logFile.isOpen() == true)
    {
        QDateTime utcnow = QDateTime::currentDateTimeUtc();
        QString message = QString("%1 - %2\n").arg(utcnow.toString("yyyy-mm-dd [HH:mm:ss.zzz]")).arg(msg);
        theLog<<message;
    } else {
        qDebug() << tr("Cannot write log message to file: %1").arg(logFile.errorString());
    }
}

void logger::logTrace(QString   _message) { logMessage(_message, Trace); }
void logger::logDebug(QString   _message) { logMessage(_message, Debug); }
void logger::logInfo(QString    _message) { logMessage(_message, Info); }
void logger::logWarning(QString _message) { logMessage(_message, Warning); }
void logger::logError(QString   _message) { logMessage(_message, Error); }
void logger::logFatal(QString   _message) { logMessage(_message, Fatal); }
