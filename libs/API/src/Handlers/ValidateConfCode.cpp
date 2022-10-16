#include "API/Handlers/ValidateConfCode.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

ValidateConfCode::ValidateConfCode(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

ValidateConfCode::~ValidateConfCode()
{ }

HndlError
ValidateConfCode::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
ValidateConfCode::Serialize() const
{
        return {};
}

HndlError
ValidateConfCode::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */