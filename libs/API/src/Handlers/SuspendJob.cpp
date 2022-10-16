#include "API/Handlers/SuspendJob.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

SuspendJob::SuspendJob(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

SuspendJob::~SuspendJob()
{ }

HndlError
SuspendJob::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
SuspendJob::Serialize() const
{
        return {};
}

HndlError
SuspendJob::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */