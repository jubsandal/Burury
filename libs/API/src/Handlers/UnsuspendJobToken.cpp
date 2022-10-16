#include "API/Handlers/UnsuspendJobToken.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

UnsuspendJobToken::UnsuspendJobToken(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

UnsuspendJobToken::~UnsuspendJobToken()
{ }

HndlError
UnsuspendJobToken::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
UnsuspendJobToken::Serialize() const
{
        return {};
}

HndlError
UnsuspendJobToken::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */