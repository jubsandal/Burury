#pragma once

#include <QJsonObject>

#include "API/Handler.hpp"
#include "API/HndlError.hpp"

namespace API::HNDL {

class CreateTable : public Handler {
public:
        CreateTable(QString, QString, const std::initializer_list<Roles::RoleId>& = {});
        virtual ~CreateTable();

        virtual HndlError exec(QJsonObject&) override;

        virtual QJsonObject Serialize() const override;

private:
        virtual HndlError Deserialize(const QJsonObject&) override;

        QString _query;

};

} /* API::HNDL */
