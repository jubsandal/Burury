#include "API/Handlers/AddGrade.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

AddGrade::AddGrade(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

AddGrade::~AddGrade()
{ }

HndlError
AddGrade::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
AddGrade::Serialize() const
{
        return {};
}

HndlError
AddGrade::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */
