#include "API/Handlers/EditUserInfo.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

EditUserInfo::EditUserInfo(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

EditUserInfo::~EditUserInfo()
{ }

HndlError
EditUserInfo::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
EditUserInfo::Serialize() const
{
        return {};
}

HndlError
EditUserInfo::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */