#pragma once

#include <QJsonObject>

#include "API/Handler.hpp"
#include "API/HndlError.hpp"

namespace API::HNDL {

class Identify : public Handler {
public:
        Identify(QString name, QString desc, const std::initializer_list<Roles::RoleId>& = {});
        virtual ~Identify();

        virtual HndlError exec(QJsonObject&) override;

        virtual QJsonObject Serialize() const override;

private:
        virtual HndlError Deserialize(const QJsonObject&) override;

        QString _login;
        QString _password;

};

} /* API::HNDL */
