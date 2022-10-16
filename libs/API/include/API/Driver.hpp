#ifndef API_HPP_DP4AIHQZ
#define API_HPP_DP4AIHQZ

#include <QtSql>
#include <QString>
#include <QJsonObject>
#include <QQueue>

#include "API/Handler.hpp"
#include "API/HndlError.hpp"

#include "Logger/logger.hpp"
// #include "Widgets/Notifier/NotifyProgressItemFactory.hpp"

namespace API {

class DriverAssistant : public QObject {
Q_OBJECT

public:
        DriverAssistant(QObject *p = nullptr);
        /* DriverAssistant(bool deleteOnSuccess = false, QObject *p = nullptr); */
        virtual ~DriverAssistant();

public Q_SLOTS:
        void Success(QJsonObject obj);
        void Failed(API::HndlError);
                // Driver use it interface to emit this->success()/failed() chain

Q_SIGNALS:
        void success(QJsonObject obj);
        void failed(API::HndlError);

private:
        /* bool _delete_on_success; */
};

// ---------------------- //

struct DriverCmd {
        int handler_id;
        int caller_role_id;
        QJsonObject payload;
        DriverAssistant * waiter;
};

// own thread-oriented driver
class Driver : public QObject {
Q_OBJECT

public:
        Driver(const QString& path, QObject * p = nullptr);
        virtual ~Driver();

Q_SIGNALS:
        void Inited();
        void InitizlizationFailed(QSqlError);

        void logMessage(QString, int = LoggingLevel::Trace);

        // void setNotifyItemPropery(int, const QByteArray &, const QVariant &);
        // void createNotifyItem(NotifyItemFactory *, int& uid);

public Q_SLOTS:
        void Run();
        void Stop();

        void addCommand(API::DriverCmd);

private Q_SLOTS:
        void worker();
                // worker ¯\_(ツ)_/¯

        // TODO rename to async
        void executeCommand(int handler_id, int caller_role_id, QJsonObject payload, DriverAssistant * waiter);
                // all permission controll system BASED on this small role variable
        // TODO rename to sync
        bool autoExecCommand(int handler_id, QJsonObject payload);
                // no callable outside, only for serve
                // return true/false on success/failed
                // TODO change return val to HndlError

private:
        void Initialize();
        void checkTables();
        void checkDefaultUserGrades();

        QVector<Handler *> _handlers;

        QMutex _queueMtx;
        QQueue<DriverCmd> _hndlQueue;

        // NotifyProgressItemFactory * _pf;
        int _p_uid;
        int _p_uid_main;
        int _p_uid_sub;

        bool _running;
        QString _path;
        QSqlDatabase * _db;
};


} /* API */

Q_DECLARE_METATYPE(API::DriverCmd)

#endif /* end of include guard: API_HPP_DP4AIHQZ */
