#include "API/Handlers/AddJob.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

AddJob::AddJob(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

AddJob::~AddJob()
{ }

HndlError
AddJob::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
AddJob::Serialize() const
{
        return {};
}

HndlError
AddJob::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */