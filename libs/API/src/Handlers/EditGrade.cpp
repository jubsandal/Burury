#include "API/Handlers/EditGrade.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

EditGrade::EditGrade(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

EditGrade::~EditGrade()
{ }

HndlError
EditGrade::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
EditGrade::Serialize() const
{
        return {};
}

HndlError
EditGrade::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */