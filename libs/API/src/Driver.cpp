#include "API/Driver.hpp"
#include "API/tables.hpp"
#include "API/UserGrades.hpp"
#include "API/Utility.hpp"
#include "API/Role.hpp"

#include <QMutexLocker>
#include <QTimer>
#include <functional>
#include <QDebug>

// // BASH AUTO APPEND INCLUDE START <- signature for append include list by bash script
#include "API/Handlers/AddGrade.hpp"
#include "API/Handlers/AddJob.hpp"
#include "API/Handlers/AddJobTokens.hpp"
#include "API/Handlers/AttachJobTokens.hpp"
#include "API/Handlers/CreateTable.hpp"
#include "API/Handlers/CreateUser.hpp"
#include "API/Handlers/EditGrade.hpp"
#include "API/Handlers/EditJob.hpp"
#include "API/Handlers/EditSelfInfo.hpp"
#include "API/Handlers/EditUserInfo.hpp"
#include "API/Handlers/GetGradeInfo.hpp"
#include "API/Handlers/GetJobInfo.hpp"
#include "API/Handlers/GetRoleDetails.hpp"
#include "API/Handlers/GetSelfInfo.hpp"
#include "API/Handlers/GetUserInfo.hpp"
#include "API/Handlers/Identify.hpp"
#include "API/Handlers/RefuseTokens.hpp"
#include "API/Handlers/RegisterManager.hpp"
#include "API/Handlers/RegisterWorker.hpp"
#include "API/Handlers/RemoveGrade.hpp"
#include "API/Handlers/RemoveJob.hpp"
#include "API/Handlers/RequestTokens.hpp"
#include "API/Handlers/SendMessage.hpp"
#include "API/Handlers/SendMoney.hpp"
#include "API/Handlers/SendTokenForCheck.hpp"
#include "API/Handlers/SetTokenDeadline.hpp"
#include "API/Handlers/SuspendJob.hpp"
#include "API/Handlers/SuspendJobToken.hpp"
#include "API/Handlers/UnattachJobTokens.hpp"
#include "API/Handlers/UnsuspendJobToken.hpp"
#include "API/Handlers/UpdateRole.hpp"
#include "API/Handlers/ValidateConfCode.hpp"
// // BASH AUTO APPEND INCLUDE END

namespace API {

DriverAssistant::DriverAssistant(QObject * p)
: QObject(p)
{ }
DriverAssistant::~DriverAssistant() { }
void DriverAssistant::Success(QJsonObject o) { Q_EMIT success(o); }
void DriverAssistant::Failed(API::HndlError err) { Q_EMIT failed(err); }

#define XX(id, name, roles) new HNDL::name(QUOTE(name), QUOTE(), roles),
Driver::Driver(const QString& path, QObject * p)
        : QObject(p),
        _path(path),
        // _pf(new NotifyProgressItemFactory),
        _handlers({ HANDLERS_MAP(XX) })
{
        // _pf->setExitOnCompleted(true);

        for (auto hndl : _handlers) {
                qDebug() << ":: " << hndl->name << "applied";
        }
        qDebug() << _handlers.length() << " handlers applied";
}
#undef XX

Driver::~Driver()
{
        // delete _pf;
        if (_db && _db->isOpen()) {
                _db->close();
                delete _db;
        }
}

// TODO add check if inited
void Driver::Initialize()
{
        // _pf->setTitle("Database initialization");
        // _pf->setMaximum(3);
        // Q_EMIT createNotifyItem(_pf, _p_uid_main);

        _db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
        _db->setDatabaseName(_path);
        if (!_db->open()) {
                qDebug() << "Cannot open database";
                throw _db->lastError();
        }
        qDebug() << "SQLite database opened: " << _path;

        // Q_EMIT setNotifyItemPropery(_p_uid_main, "progress", 1);
        checkTables();
        // Q_EMIT setNotifyItemPropery(_p_uid_main, "progress", 2);
        checkDefaultUserGrades();
        // Q_EMIT setNotifyItemPropery(_p_uid_main, "progress", 3);

        Q_EMIT Inited();
}

// check for table existense, and create unexisted
void Driver::checkTables()
{
        #define XX(id, name, createQuery) { Tables::name, QUOTE(name), createQuery },
                struct table {
                        int id;
                        const char * name;
                        const char * createQuery;
                } mustExistsTables[] = { TABLES_MAP(XX) };
        #undef XX
        // _pf->setTitle("Looking for tables");
        // _pf->setMaximum(Tables::COUNT);
        // Q_EMIT createNotifyItem(_pf, _p_uid);
        QStringList tables = _db->tables();
        for (int i = 0; i < Tables::COUNT; i++) {
                qDebug() << "Checking " << mustExistsTables[i].name << " table";
                if (!tables.contains(mustExistsTables[i].name)) {
                        if (!autoExecCommand(HNDL::Config::CreateTable, { { "query", mustExistsTables[i].createQuery } })) {
                                qDebug() << "Errer while creating default tables";
                                throw _db->lastError();
                        }
                }
                // Q_EMIT setNotifyItemPropery(_p_uid, "progress", i+1);
        }
}


// TODO create more intelegent checker for check setted to user not existense user grades and add base grades with
// randomly generated title & description with 1 value
// TODO add handler call to check this info
void Driver::checkDefaultUserGrades()
{
        // _pf->setTitle("Looking for default user grades");
        // _pf->setMaximum(UserGrades::COUNT);
        // Q_EMIT createNotifyItem(_pf, _p_uid);


        #define XX(id, title, description, value) { QUOTE(title), QUOTE(description), value },
                struct userGrades_set {
                        const char * title;
                        const char * description;
                        int value;
                } grades[] = { USER_GRADES_MAP(XX) };
        #undef XX

        QSqlQuery q;
        bool deleted = false;
        for (int i = 0; i < UserGrades::COUNT; i++) {
                auto grade = grades[(UserGrades::UserGradesId)i];
                q.prepare("SELECT * FROM UserGrades WHERE title = :title");
                q.bindValue(":title", grade.title);
                if (!q.exec()) {
                        qDebug() << "Error while fetching existed user grades";
                        throw q.lastError();
                }
                if (!q.next()) {
                        q.clear();
                        q.prepare("INSERT INTO UserGrades (title, description, value) "
                                  "VALUES (:title, :description, :value)");
                        q.bindValue(":title", grade.title);
                        q.bindValue(":description", grade.description);
                        q.bindValue(":value", grade.value);
                        if (!q.exec()) {
                                qDebug() << "Error while creating defult user grades";
                                throw q.lastError();
                        }
                }
                // Q_EMIT setNotifyItemPropery(_p_uid, "progress", i+1);
        }
}

void Driver::Run()
{
        bool failed = false;
        try {
                this->Initialize();
        } catch (QSqlError e) {
                qDebug() << e;
                // TODO add force end progress
                Q_EMIT InitizlizationFailed(e);
                failed = true;
        }

        if (!failed) {
                _running=true;
                this->worker();
        }
}

void Driver::Stop()
{
        _running = false;
}

// TODO create thread pool!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
void Driver::worker()
{
        QMutexLocker lock(&_queueMtx);
        // TODO maybe bag
        while (_hndlQueue.length()) {
                DriverCmd cmd = _hndlQueue.dequeue();
                this->executeCommand(cmd.handler_id, cmd.caller_role_id, cmd.payload, cmd.waiter);
        }

        if (_running) {
                QTimer::singleShot(100, this, SLOT(worker()));
        }
}

void Driver::addCommand(API::DriverCmd cmd) {
        QMutexLocker lock(&_queueMtx);
        _hndlQueue.append(cmd);
}

void Driver::executeCommand(int handler_id,
                            int caller_role_id,
                            QJsonObject payload,
                            DriverAssistant * waiter)
{
        QElapsedTimer timer;
        Handler * handler;
        timer.start();

        QSqlQuery q;

        if (!waiter) {
                throw QString("Driver::" + QString(__func__) + ": Null waiter passed!");
        }

        if (handler_id > HNDL::Config::COUNT || handler_id < 0) {
                waiter->Failed(HndlError(InvalidCommand, "Trying to call unexists handler"));
                return;
        }

        handler = _handlers[handler_id];

        if (caller_role_id > Roles::COUNT || caller_role_id < 0) {
                waiter->Failed(HndlError(AccessDenied, "Invalid caller role ID"));
                return;
        }

        if (!handler->haveAccess((Roles::RoleId)caller_role_id)) {
                waiter->Failed(HndlError(AccessDenied, "You not have access to preform this request"));
                return;
        }

        HndlError rc = handler->exec(payload);
        if (rc.Ok()) {
                waiter->Success(payload);
        } else {
                waiter->Failed(rc);
        }
        qDebug() << "Handler " << handler->name << " exec time: " << timer.elapsed() << "\t" << timer.nsecsElapsed();
}

bool Driver::autoExecCommand(int handler, QJsonObject payload)
{
        HndlError rc = _handlers[handler]->exec(payload);
        if (rc.Ok()) {
                return true;
        } else {
                return false;
        }
}

} /* API */ 
