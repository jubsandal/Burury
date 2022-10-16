#pragma once

#include <QJsonObject>

#include "API/Handler.hpp"
#include "API/HndlError.hpp"

namespace API::HNDL {

class GetUserInfo : public Handler {
public:
        GetUserInfo(QString name, QString desc, const std::initializer_list<Roles::RoleId>& = {});
        virtual ~GetUserInfo();

        virtual HndlError exec(QJsonObject&) override;

        virtual QJsonObject Serialize() const override;

private:
        virtual HndlError Deserialize(const QJsonObject&) override;

        HndlError fetchSingle(QJsonObject&);
        HndlError fetchMultiple(QJsonObject&);

        int _user_id;
        bool _take_image;
        // QVector<QString> _select_fields;

        // multiple select
        // int _page;
        // int _per_page;

};

} /* API::HNDL */
