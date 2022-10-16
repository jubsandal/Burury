#include "API/Handlers/SuspendJobToken.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

SuspendJobToken::SuspendJobToken(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

SuspendJobToken::~SuspendJobToken()
{ }

HndlError
SuspendJobToken::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
SuspendJobToken::Serialize() const
{
        return {};
}

HndlError
SuspendJobToken::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */