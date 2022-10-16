#include "API/Handlers/RegisterManager.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

RegisterManager::RegisterManager(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

RegisterManager::~RegisterManager()
{ }

HndlError
RegisterManager::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
RegisterManager::Serialize() const
{
        return {};
}

HndlError
RegisterManager::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */