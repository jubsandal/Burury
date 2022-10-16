#include "API/Handlers/UnattachJobTokens.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

UnattachJobTokens::UnattachJobTokens(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

UnattachJobTokens::~UnattachJobTokens()
{ }

HndlError
UnattachJobTokens::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
UnattachJobTokens::Serialize() const
{
        return {};
}

HndlError
UnattachJobTokens::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */