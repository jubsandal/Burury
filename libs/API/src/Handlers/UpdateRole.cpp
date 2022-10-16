#include "API/Handlers/UpdateRole.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

UpdateRole::UpdateRole(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

UpdateRole::~UpdateRole()
{ }

HndlError
UpdateRole::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
UpdateRole::Serialize() const
{
        return {};
}

HndlError
UpdateRole::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */