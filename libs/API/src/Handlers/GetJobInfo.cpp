#include "API/Handlers/GetJobInfo.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

GetJobInfo::GetJobInfo(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

GetJobInfo::~GetJobInfo()
{ }

HndlError
GetJobInfo::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
GetJobInfo::Serialize() const
{
        return {};
}

HndlError
GetJobInfo::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */