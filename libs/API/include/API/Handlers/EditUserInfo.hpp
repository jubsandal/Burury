#pragma once

#include <QJsonObject>

#include "API/Handler.hpp"
#include "API/HndlError.hpp"

namespace API::HNDL {

class EditUserInfo : public Handler {
public:
        EditUserInfo(QString name, QString desc, const std::initializer_list<Roles::RoleId>& = {});
        virtual ~EditUserInfo();

        virtual HndlError exec(QJsonObject&) override;

        virtual QJsonObject Serialize() const override;

private:
        virtual HndlError Deserialize(const QJsonObject&) override;

};

} /* API::HNDL */