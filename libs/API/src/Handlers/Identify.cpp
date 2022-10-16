#include "API/Handlers/Identify.hpp"
#include "API/PayloadItem.hpp"
#include "Crypto/Hash.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

Identify::Identify(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

Identify::~Identify()
{ }

HndlError
Identify::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }
        QSqlQuery q;

        q.prepare("SELECT id, name, role, password, salt FROM Users "
                  "WHERE login = :login");
        q.bindValue(":login", _login);

        if (!q.exec()) {
                return HndlError(SQLError, q.lastQuery() + " " + q.lastError().text());
        }
        if (!q.next()) {
                return HndlError(AccessDenied, "No user registreted with login: " + _login);
        }

        QByteArray salt = q.record().value("salt").toByteArray();
        QByteArray real_passwordHash = q.record().value("password").toByteArray();
        QByteArray passed_passwordHash = encryptPassword(_password.toLatin1(), salt);

        if (real_passwordHash != passed_passwordHash) {
                return HndlError(AccessDenied, "Invalid password");
        }

        req["identified"] = true;
        req["role"] = q.record().value("role_id").toInt();
        req["name"] = q.record().value("name").toString();
        req["id"] = q.record().value("id").toString();

        return HndlError();
}

QJsonObject
Identify::Serialize() const
{
        return {};
}

HndlError
Identify::Deserialize(const QJsonObject& obj)
{
        auto loginEx = Payload::Item<QString>(obj["login"], Payload::NotEmpty).extract();
        auto passwordEx = Payload::Item<QString>(obj["password"], Payload::NotEmpty).extract();
        if (loginEx.error != Payload::Ok || passwordEx.error != Payload::Ok) {
                obj["identified"] = false;
                return HndlError(InvalidParam, "Login or password is empty string");
        }

        _login = loginEx.value;
        _password = passwordEx.value;

        return HndlError();
}

} /* API::HNDL */
