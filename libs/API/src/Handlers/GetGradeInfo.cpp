#include "API/Handlers/GetGradeInfo.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

GetGradeInfo::GetGradeInfo(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

GetGradeInfo::~GetGradeInfo()
{ }

HndlError
GetGradeInfo::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
GetGradeInfo::Serialize() const
{
        return {};
}

HndlError
GetGradeInfo::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */