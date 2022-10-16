#include "API/Handlers/CreateUser.hpp"
#include "API/PayloadItem.hpp"
#include "Crypto/Hash.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

CreateUser::CreateUser(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

CreateUser::~CreateUser()
{ }

HndlError
CreateUser::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        QSqlQuery q;

        QByteArray passwordBytes = _password.toLatin1();
        QByteArray salt = saltGen(passWeaknesses(passwordBytes));
        QByteArray passwordHash =
                encryptPassword(passwordBytes, salt);

        // TODO add id check
        q.prepare("INSERT INTO Users (" +
                          (_id > 0 ? QString("id") : QString("")) +
                          "name, role," +
                          (_grade_id > 0 ? QString("grade_id, ") : QString()) +
                          "email, telegram, vk, " +
                          (_tokensCompleted > 0 ? QString("tokensCompleted, ") : QString("")) +
                          (_tokensTotal > 0 ? QString("tokensTotal, ") : QString("")) +
                          "login, password, salt" +
                          (_imageBase64.length() > 0 ? QString("image") : QString("")) + ")"
                  "VALUES(" +
                          (_id > 0 ? QString(":id") : QString("")) +
                          ":name, :role," +
                          (_grade_id > 0 ? QString(":grade_id, ") : QString()) +
                          ":email, :telegram, :vk, " +
                          (_tokensCompleted > 0 ? QString(":tokensCompleted, ") : QString("")) +
                          (_tokensTotal > 0 ? QString(":tokensTotal, ") : QString("")) +
                          ":login, :password, :salt" +
                          (_imageBase64.length() > 0 ? QString(":image") : QString("")) + ")");
        q.bindValue(":id", _id);
        q.bindValue(":name", _name);
        q.bindValue(":role", _role);
        q.bindValue(":grade_id", _grade_id);
        q.bindValue(":email", _email);
        q.bindValue(":telegram", _telegram);
        q.bindValue(":vk", _vk);
        q.bindValue(":tokensCompleted", _tokensCompleted);
        q.bindValue(":tokensTotal", _tokensTotal);
        q.bindValue(":login", _login);
        q.bindValue(":password", passwordHash);
        q.bindValue(":salt", salt);
        q.bindValue(":image", _imageBase64);

        if (!q.exec()) {
                return HndlError(SQLError, q.lastError().text());
        }

        return HndlError();
}

QJsonObject
CreateUser::Serialize() const
{
        return {};
}

HndlError
CreateUser::Deserialize(const QJsonObject& obj)
{
        auto idEx = Payload::Item<int>(obj["id"], -1).extract();
        auto nameEx = Payload::Item<QString>(obj["name"], Payload::NotEmpty).extract();
        auto roleEx = Payload::Item<int>(obj["role"], Payload::NotEmpty).extract();
        auto grade_idEx = Payload::Item<int>(obj["grade_id"], -1).extract();
        auto emailEx = Payload::Item<QString>(obj["email"]).extract();
        auto telegramEx = Payload::Item<QString>(obj["telegram"]).extract();
        auto vkEx = Payload::Item<QString>(obj["vk"]).extract();
        auto tokensComplEx = Payload::Item<int>(obj["tokensCompleted"], -1).extract(); 
        auto tokensTotalEx = Payload::Item<int>(obj["tokensTotal"], -1).extract(); 
        auto loginEx = Payload::Item<QString>(obj["login"], Payload::NotEmpty).extract();
        auto passwordEx = Payload::Item<QString>(obj["password"], Payload::NotEmpty).extract();
        auto imageEx = Payload::Item<QString>(obj["image"]).extract();
        auto bannedDueEx = Payload::Item<int>(obj["bannedDue"], -1).extract();

        if (nameEx.error != Payload::Ok ||
                // TODO check email by REGEX
                loginEx.error != Payload::Ok ||
                passwordEx.error != Payload::Ok)
        {
                return HndlError(InvalidParam, "Some parameters not passed or eq zero");
        }

        _id = idEx.value;
        _name = nameEx.value;
        _role = roleEx.value;
        _grade_id = grade_idEx.value;
        _email = emailEx.value;
        _telegram = telegramEx.value;
        _vk = vkEx.value;
        _tokensCompleted = tokensComplEx.value;
        _tokensTotal = tokensTotalEx.value;
        _login = loginEx.value;
        _password = passwordEx.value;
        _imageBase64 = imageEx.value;
        _bannedDue = bannedDueEx.value;

        // TODO checks

        return HndlError();
}

} /* API::HNDL */
