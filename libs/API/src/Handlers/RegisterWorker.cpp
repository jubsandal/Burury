#include "API/Handlers/RegisterWorker.hpp"
#include "API/PayloadItem.hpp"
#include "API/UserGrades.hpp"
#include "Crypto/Hash.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>
#include <QJsonObject>

namespace API::HNDL {

RegisterWorker::RegisterWorker(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

RegisterWorker::~RegisterWorker()
{ }

HndlError
RegisterWorker::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        QByteArray image;
        QByteArray passwordBytes = _password.toLatin1();
        if (_imageBase64.length()) {
                image = QByteArray::fromBase64(_imageBase64.toLatin1());
        }

        // -- password encrypting
        // check password
        if (passwordQuality(passwordBytes) == PasswordQuality::UNAVALIBLE) {
                return HndlError(InvalidParam, "Unavalible password"); // add more informative message aha
        }

        QByteArray salt = saltGen(passWeaknesses(passwordBytes));
        QByteArray passwordHash =
                encryptPassword(passwordBytes, salt);

        QSqlQuery q;
        q.prepare("INSERT INTO Users"
                  "(name, role, grade_id, entryDate, email, telegram, vk, login, password, salt, image) "
                  "VALUES (:name, :role, :grade_id, :entryDate, :email, :telegram, :vk, :login, :password, :salt, :image)");
        q.bindValue(":name", _name);
        q.bindValue(":role", _role);
        q.bindValue(":grade_id", UserGrades::Minion);
        q.bindValue(":entryDate", _entryDate);
        q.bindValue(":email", _email);
        q.bindValue(":telegram", _telegram);
        q.bindValue(":vk", _vk);
        q.bindValue(":login", _login);
        q.bindValue(":password", passwordHash);
        q.bindValue(":salt", salt);
        q.bindValue(":image", (image.length() > 0 ? image : QVariant()));

        if (!q.exec()) {
                return HndlError(SQLError, q.lastQuery() + " " + q.lastError().text());
        }

        req["id"] = q.lastInsertId().toInt();

        return HndlError();
}

QJsonObject
RegisterWorker::Serialize() const
{
        auto a = QJsonObject{};
        return a;
}

HndlError
RegisterWorker::Deserialize(const QJsonObject& obj)
{
        auto nameEx        = Payload::Item<QString>(obj["name"],      Payload::NotEmpty).extract();
        auto roleEx        = Payload::Item<int>    (obj["role"],      Payload::NotEmpty).extract();
        auto entryDateEx   = Payload::Item<qint64> (obj["entryDate"], QDateTime::currentMSecsSinceEpoch()).extract();
        auto emailEx       = Payload::Item<QString>(obj["email"],     Payload::NotEmpty).extract();
        auto telegramEx    = Payload::Item<QString>(obj["telegram"]).extract();
        auto vkEx          = Payload::Item<QString>(obj["vk"]).extract();
        auto loginEx       = Payload::Item<QString>(obj["login"],     Payload::NotEmpty).extract();
        auto passwordEx    = Payload::Item<QString>(obj["password"],  Payload::NotEmpty).extract();
        auto imageBase64Ex = Payload::Item<QString>(obj["image"]).extract();

        // check values
        if (nameEx.error != Payload::Ok ||
                // now only workers can be registred, not impled to register other
                (roleEx.error != Payload::Ok || (roleEx.error == Payload::Ok && roleEx.value != API::Roles::Worker)) ||
                // TODO check email by REGEX
                emailEx.error != Payload::Ok ||
                loginEx.error != Payload::Ok ||
                passwordEx.error != Payload::Ok)
        {
                return HndlError(InvalidParam, "Some parameters not passed or eq zero");
        }

        _name        =  nameEx.value;
        _role        =  roleEx.value;
        _entryDate   =  entryDateEx.value;
        _email       =  emailEx.value;
        _telegram    =  telegramEx.value;
        _vk          =  vkEx.value;
        _login       =  loginEx.value;
        _password    =  passwordEx.value;
        _imageBase64 =  imageBase64Ex.value;

        return HndlError();
}

} /* API::HNDL */
