#include "API/Handlers/EditJob.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

EditJob::EditJob(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

EditJob::~EditJob()
{ }

HndlError
EditJob::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
EditJob::Serialize() const
{
        return {};
}

HndlError
EditJob::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */