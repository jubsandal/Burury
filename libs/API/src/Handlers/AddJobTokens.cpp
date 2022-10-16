#include "API/Handlers/AddJobTokens.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

AddJobTokens::AddJobTokens(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

AddJobTokens::~AddJobTokens()
{ }

HndlError
AddJobTokens::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
AddJobTokens::Serialize() const
{
        return {};
}

HndlError
AddJobTokens::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */