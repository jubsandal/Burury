#include "API/Handlers/RemoveGrade.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

RemoveGrade::RemoveGrade(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

RemoveGrade::~RemoveGrade()
{ }

HndlError
RemoveGrade::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
RemoveGrade::Serialize() const
{
        return {};
}

HndlError
RemoveGrade::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */