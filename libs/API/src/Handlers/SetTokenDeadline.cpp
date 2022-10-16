#include "API/Handlers/SetTokenDeadline.hpp"
#include "API/PayloadItem.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

SetTokenDeadline::SetTokenDeadline(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

SetTokenDeadline::~SetTokenDeadline()
{ }

HndlError
SetTokenDeadline::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
SetTokenDeadline::Serialize() const
{
        return {};
}

HndlError
SetTokenDeadline::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */