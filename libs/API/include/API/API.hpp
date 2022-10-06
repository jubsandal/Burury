#ifndef API_HPP_DP4AIHQZ
#define API_HPP_DP4AIHQZ

#include <QtSql>
#include <QString>
#include <QJsonObject>
#include <QQueue>

#include "API/tables.hpp"
#include "API/Role.hpp"
#include "API/command.hpp"
#include "API/CmdError.hpp"

#include "Logger/logger.hpp"
#include "Widgets/Notifier/NotifyProgressItemFactory.hpp"

namespace API {

        class DriverAssistant : public QObject {
                Q_OBJECT

                public:
                        DriverAssistant(QObject *p = nullptr);
                        /* DriverAssistant(bool deleteOnSuccess = false, QObject *p = nullptr); */
                        virtual ~DriverAssistant();

                public Q_SLOTS:
                        // Driver use it interface to emit this->success()/failed() chain
                        void Success(QJsonObject obj);
                        void Failed(API::CmdError);

                Q_SIGNALS:
                        void success(QJsonObject obj);
                        void failed(API::CmdError);

                private:
                        /* bool _delete_on_success; */
        };

        struct DriverCmd {
                int executorRole;
                QJsonObject data;
                DriverAssistant * waiter;
        };

        // own thread-oriented driver
        class Driver : public QObject {
                Q_OBJECT

                public:
                        struct table_def {
                                int id;
                                const char * name;
                                const char * createQuery;
                        };

                        struct role_set {
                                int id; // equal to index
                                const char * name;
                                double payMult;
                                double payPeriod;
                                QVector<CommandId> commands;
                        };

                        struct command_set {
                                int id; // equal to index
                                const char * name;
                                command_exec_t executor;
                                bool sendback;
                        };

                        struct accounting_set {
                                int id;
                                const char * name;
                        };

                public:
                        Driver(const QString& path, QObject * p = nullptr);
                        virtual ~Driver();

                        const QMap<RoleId, role_set>& avalibleRoles() const;

                Q_SIGNALS:
                        void Inited();
                        void InitizlizationFailed(QSqlError);

                        void logMessage(QString, int = LoggingLevel::Trace);

                        void setNotifyItemPropery(int, const QByteArray &, const QVariant &);
                        void createNotifyItem(NotifyItemFactory *, int& uid);

                public Q_SLOTS:
                        void Run();
                        void Stop();

                private Q_SLOTS:
                        void worker();
                                // worker ¯\_(ツ)_/¯

                        void addCommand(API::DriverCmd);

                        void executeCommand(API::RoleId role, QJsonObject o, DriverAssistant*);
                                // all permission controll system BASED on this small role variable
                        bool autoExecCommand(QJsonObject o);
                                // no callable outside, only for serve
                                // return true/false on success/failed
                                // TODO change return val to CmdError

                private:
                        void Initialize();
                        void checkTables();
                        void insertDefaultsRoles();
                        void insertAccounting();

                        QMap<RoleId, role_set> _roles;
                        QVector<table_def>     _tables;
                        QVector<command_set>   _commands;
                        QVector<accounting_set> _accounting;

                        QMutex _queueMtx;
                        QQueue<DriverCmd> _cmdQueue;

                        NotifyProgressItemFactory * _pf;
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
