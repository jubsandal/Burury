#include "API/Handlers/RequestTokens.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

RequestTokens::RequestTokens(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

RequestTokens::~RequestTokens()
{ }

HndlError
RequestTokens::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
RequestTokens::Serialize() const
{
        return {};
}

HndlError
RequestTokens::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */