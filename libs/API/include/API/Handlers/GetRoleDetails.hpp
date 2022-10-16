#pragma once

#include <QJsonObject>

#include "API/Handler.hpp"
#include "API/HndlError.hpp"

namespace API::HNDL {

class GetRoleDetails : public Handler {
public:
        GetRoleDetails(QString name, QString desc, const std::initializer_list<Roles::RoleId>& = {});
        virtual ~GetRoleDetails();

        virtual HndlError exec(QJsonObject&) override;

        virtual QJsonObject Serialize() const override;

private:
        virtual HndlError Deserialize(const QJsonObject&) override;

};

} /* API::HNDL */