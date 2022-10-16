#include "API/Handlers/AttachJobTokens.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

AttachJobTokens::AttachJobTokens(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

AttachJobTokens::~AttachJobTokens()
{ }

HndlError
AttachJobTokens::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
AttachJobTokens::Serialize() const
{
        return {};
}

HndlError
AttachJobTokens::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */