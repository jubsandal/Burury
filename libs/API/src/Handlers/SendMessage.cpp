#include "API/Handlers/SendMessage.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

SendMessage::SendMessage(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

SendMessage::~SendMessage()
{ }

HndlError
SendMessage::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
SendMessage::Serialize() const
{
        return {};
}

HndlError
SendMessage::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */