#include "API/Handlers/RemoveJob.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

RemoveJob::RemoveJob(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

RemoveJob::~RemoveJob()
{ }

HndlError
RemoveJob::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
RemoveJob::Serialize() const
{
        return {};
}

HndlError
RemoveJob::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */