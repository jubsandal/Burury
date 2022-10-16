#!/bin/bash

template="#pragma once

#include <QJsonObject>

#include \"API/Handler.hpp\"
#include \"API/HndlError.hpp\"

namespace API::HNDL {

class %s : public Handler {
public:
        %s(QString name, QString desc, const std::initializer_list<Roles::RoleId>& = {});
        virtual ~%s();

        virtual HndlError exec(QJsonObject&) override;

        virtual QJsonObject Serialize() const override;

private:
        virtual HndlError Deserialize(const QJsonObject&) override;

};

} /* API::HNDL */"

for hpp in  *.hpp
do
        lc=$(wc -l "$hpp" | cut -d ' ' -f 1)
        (( lc == 0 )) && printf "$template" "${hpp/.hpp/}" "${hpp/.hpp/}" "${hpp/.hpp/}" > "$hpp"
done
