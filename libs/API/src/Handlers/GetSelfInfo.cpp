#include "API/Handlers/GetSelfInfo.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

GetSelfInfo::GetSelfInfo(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

GetSelfInfo::~GetSelfInfo()
{ }

HndlError
GetSelfInfo::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
GetSelfInfo::Serialize() const
{
        return {};
}

HndlError
GetSelfInfo::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */