#ifndef TYPES_HPP_GKJPPIHT
#define TYPES_HPP_GKJPPIHT

#include <QString>
#include <QJsonObject>

/*
 * Types used in command syntacsis
 * Purpose: unpack from / pack to json objects. true check of fields before use
 */

struct Human {
        QString name;
        quint64 entryDate;
        int role;
        int wapon;
        QString email;
        QString login;
        QString password;
        QByteArray image;

        bool fromJson(const QJsonObject& obj)
        {
                auto map = obj.toVariantMap();

                return true;
        }

        QJsonObject toJson() const
        {
                QJsonObject obj {
                        { "name",      this->name },
                        { "entryDate", (long long)this->entryDate },
                        { "role",      this->role },
                        { "email",     this->email },
                        { "login",     this->login },
                        { "password",  this->password },
                        { "image",     this->image.data() }
                };
                return obj;
        }

        QJsonObject jsonPattern
        {
                { "name",      "s" },
                { "entryDate", 1   },
                { "role",      1   },
                { "wapon",     0   },
                { "email",     ""  },
                { "login",     "s" },
                { "password",  "s" },
                { "image",     "s" },
        };
};


#endif /* end of include guard: TYPES_HPP_GKJPPIHT */
