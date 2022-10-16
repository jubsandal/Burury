#pragma once

#include <QJsonObject>

#include "API/Handler.hpp"
#include "API/HndlError.hpp"

namespace API::HNDL {

class CreateUser : public Handler {
public:
        CreateUser(QString name, QString desc, const std::initializer_list<Roles::RoleId>& = {});
        virtual ~CreateUser();

        virtual HndlError exec(QJsonObject&) override;

        virtual QJsonObject Serialize() const override;

private:
        virtual HndlError Deserialize(const QJsonObject&) override;

        int _id; // -1 to auto
        QString _name;
        int _role;
        int _grade_id; // -1 to default
        qint64 _entryDate; // -1 to default
        QString _email;
        QString _telegram;
        QString _vk;
        int _tokensCompleted; // -1 to default
        int _tokensTotal; // -1 to default
        QString _login;
        QString _password;
        QString _imageBase64; // empty to skip
        int _bannedDue; // -1 to default
};

} /* API::HNDL */
