#include "API/Handlers/RefuseTokens.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

RefuseTokens::RefuseTokens(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

RefuseTokens::~RefuseTokens()
{ }

HndlError
RefuseTokens::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
RefuseTokens::Serialize() const
{
        return {};
}

HndlError
RefuseTokens::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */