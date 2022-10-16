#!/bin/bash

template="#include \"API/Handlers/%s.hpp\"
#include \"API/PayloadItem.hpp\"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>

namespace API::HNDL {

%s::%s(QString name, QString desc, const std::initializer_list<Roles::RoleId>& roles)
        : Handler(name, desc, roles)
{ }

%s::~%s()
{ }

HndlError
%s::exec(QJsonObject& req)
{
        HndlError deser_err = Deserialize(req);
        if (!deser_err.Ok()) {
                return deser_err;
        }

        return HndlError();
}

QJsonObject
%s::Serialize() const
{
        return {};
}

HndlError
%s::Deserialize(const QJsonObject& obj)
{

        return HndlError();
}

} /* API::HNDL */"

for hpp in *.hpp
do
        ins="${hpp/.hpp/}"
        out="../../../src/Handlers/${hpp/.hpp/.cpp}"
        if [[ -e $out ]]; then
                lc=$(wc -l "$out" | cut -d ' ' -f 1)
                (( lc == 0 )) && printf "$template"  "$ins" "$ins" "$ins" "$ins" "$ins" "$ins" "$ins" "$ins" >> $out
        else
                printf "$template" "$ins" "$ins" "$ins" "$ins" "$ins" "$ins" "$ins" "$ins" >> $out
        fi
done
