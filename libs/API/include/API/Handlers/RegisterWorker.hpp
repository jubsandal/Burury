#pragma once

#include <QJsonObject>

#include "API/Handler.hpp"
#include "API/HndlError.hpp"

namespace API::HNDL {

class RegisterWorker : public Handler {
public:
        RegisterWorker(QString name, QString desc, const std::initializer_list<Roles::RoleId>& = {});
        virtual ~RegisterWorker();

        virtual HndlError exec(QJsonObject&) override;

        virtual QJsonObject Serialize() const override;

private:
        virtual HndlError Deserialize(const QJsonObject&) override;

        QString _name;
        int     _role;
        qint64  _entryDate;
        QString _email;
        QString _telegram;
        QString _vk;
        QString _login;
        QString _password;
        QString _imageBase64;

};

} /* API::HNDL */
