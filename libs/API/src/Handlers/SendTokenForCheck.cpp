#include "API/Handlers/SendTokenForCheck.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

SendTokenForCheck::SendTokenForCheck(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

SendTokenForCheck::~SendTokenForCheck()
{ }

HndlError
SendTokenForCheck::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
SendTokenForCheck::Serialize() const
{
        return {};
}

HndlError
SendTokenForCheck::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */