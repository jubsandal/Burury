#include "API/Handlers/GetUserInfo.hpp"
#include "API/PayloadItem.hpp"
#include "API/Utility.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

GetUserInfo::GetUserInfo(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

GetUserInfo::~GetUserInfo()
{ }

HndlError
GetUserInfo::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        QSqlQuery q;

        q.prepare("SELECT * FROM Users" + (_user_id > 0 ? QString("WHERE id = ") : QString()));
        if (!q.exec()) {
                return HndlError(SQLError, q.lastError().text());
        }

        QJsonArray users;
        while (q.next()) {
            users.append(QJsonObject{
                { "id",              q.record().value("id").toInt() },
                { "name",            q.record().value("name").toString() },
                { "role",            q.record().value("role").toInt() },
                { "grade_id",        q.record().value("grade_id").toInt() },
                { "entryDate",       q.record().value("entryDate").toInt() },
                { "email",           q.record().value("email").toString() },
                { "telegram",        q.record().value("telegram").toString() },
                { "vk",              q.record().value("vk").toString() },
                { "tokensCompleted", q.record().value("tokensCompleted").toInt() },
                { "tokensTotal",     q.record().value("tokensTotal").toInt() },
                { "login",           q.record().value("login").toString() },
                { "password",        q.record().value("password").toString() },
                { "salt",            q.record().value("salt").toString() },
                { "image",           (_take_image ? extractImg(q.record().value("image")) : "") }, // remove to another? use a model?
                { "bannedDue",       q.record().value("bannedDue").toInt() },
            });
        }

        req["users"] = users;
        req["count"] = -1;

        return HndlError();
}

QJsonObject
GetUserInfo::Serialize() const
{
        return {};
}

HndlError
GetUserInfo::Deserialize(const QJsonObject& obj)
{
        auto user_idEx = Payload::Item<int>(obj["user_id"], -1).extract();
        auto take_imageEx = Payload::Item<bool>(obj["take_image"], false).extract();

        _user_id = user_idEx.value;
        _take_image = take_imageEx.value;

        return HndlError();
}

} /* API::HNDL */
