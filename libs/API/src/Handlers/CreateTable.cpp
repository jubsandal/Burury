#include "API/Handlers/CreateTable.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

CreateTable::CreateTable(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

CreateTable::~CreateTable()
{ }


HndlError
CreateTable::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        QSqlQuery q;
        if (!q.exec(_query)) {
                qDebug() << "CMD: " << q.lastError();
                return HndlError(SQLError, q.lastQuery() + " " + q.lastError().text());
        }

        return HndlError();
}

QJsonObject
CreateTable::Serialize() const
{
        return {};
}

HndlError
CreateTable::Deserialize(const QJsonObject& obj)
{
        auto extraction = Payload::Item<QString>(obj["query"], Payload::NotEmpty).extract();
        if (extraction.error != Payload::Ok) {
                qDebug() << extraction.error;
                return HndlError(InvalidParam, "No sql query to create table");
        }
        _query = extraction.value;

        return HndlError();
}

} /* API::HNDL */
