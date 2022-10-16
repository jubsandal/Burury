#pragma once

#include <QJsonObject>

#include "API/Handler.hpp"
#include "API/HndlError.hpp"

namespace API::HNDL {

class EditSelfInfo : public Handler {
public:
        EditSelfInfo(QString name, QString desc, const std::initializer_list<Roles::RoleId>& = {});
        virtual ~EditSelfInfo();

        virtual HndlError exec(QJsonObject&) override;

        virtual QJsonObject Serialize() const override;

private:
        virtual HndlError Deserialize(const QJsonObject&) override;

};

} /* API::HNDL */