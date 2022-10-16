#include "API/Handlers/SendMoney.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

SendMoney::SendMoney(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

SendMoney::~SendMoney()
{ }

HndlError
SendMoney::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
SendMoney::Serialize() const
{
        return {};
}

HndlError
SendMoney::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */