#include "API/Handlers/GetRoleDetails.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

GetRoleDetails::GetRoleDetails(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

GetRoleDetails::~GetRoleDetails()
{ }

HndlError
GetRoleDetails::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
GetRoleDetails::Serialize() const
{
        return {};
}

HndlError
GetRoleDetails::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */