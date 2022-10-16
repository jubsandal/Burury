#pragma once

#include "API/HndlError.hpp"
#include "API/Role.hpp"

#include <QObject>
#include <QJsonObject>
#include <initializer_list>

#define ROLES_SYS {\
        Roles::SYS\
}
#define ROLES_A {\
        Roles::Admin\
}
#define ROLES_W {\
        Roles::Worker\
}
#define ROELS_M { \
        Roles::Manager\
}
#define ROLES_AS { \
        Roles::Admin,\
        Roles::SYS,\
}
#define ROLES_AM { \
        Roles::Admin,\
        Roles::Manager,\
}
#define ROLES_AMS { \
        Roles::Admin,\
        Roles::Manager, \
        Roles::SYS,\
}
#define ROLES_ALL {}

// id, name, executor, roles
#define HANDLERS_MAP(XX) \
        XX( 0,  CreateTable,       ROLES_SYS   ) \
        XX( 1,  Identify,          ROLES_SYS   ) \
        XX( 2,  RegisterWorker,    ROLES_AM  ) \
        XX( 3,  RegisterManager,   ROLES_ALL   ) \
        XX( 4,  GetRoleDetails,    ROLES_ALL ) \
        XX( 5,  UpdateRole,        ROLES_AMS ) \
        XX( 6,  GetUserInfo,       ROLES_AMS ) \
        XX( 7,  EditUserInfo,      ROLES_AMS ) \
        XX( 8,  GetSelfInfo,       ROLES_ALL ) \
        XX( 9 , EditSelfInfo,      ROLES_ALL ) \
        XX( 10, ValidateConfCode,  ROLES_ALL ) \
        XX( 11, SendMoney,         ROLES_AMS ) \
        XX( 12, AddJob,            ROLES_AMS ) \
        XX( 13, SuspendJob,        ROLES_AMS ) \
        XX( 14, RemoveJob,         ROLES_AMS ) \
        XX( 15, EditJob,           ROLES_AMS ) \
        XX( 16, GetJobInfo,        ROLES_ALL ) \
        XX( 17, AddJobTokens,      ROLES_AMS ) \
        XX( 18, AttachJobTokens,   ROLES_SYS   ) \
        XX( 19, SetTokenDeadline,  ROLES_AMS ) \
        XX( 20, UnattachJobTokens, ROLES_AMS ) \
        XX( 21, SuspendJobToken,   ROLES_AMS ) \
        XX( 22, UnsuspendJobToken, ROLES_AMS ) \
        XX( 23, SendMessage,       ROLES_ALL ) \
        XX( 24, AddGrade,          ROLES_AMS ) \
        XX( 25, RemoveGrade,       ROLES_AMS ) \
        XX( 26, EditGrade,         ROLES_AMS ) \
        XX( 27, GetGradeInfo,      ROLES_ALL ) \
        XX( 28, RequestTokens,     ROLES_W   ) \
        XX( 29, RefuseTokens,      ROLES_W   ) \
        XX( 30, SendTokenForCheck, ROLES_W   ) \
        XX( 31, CreateUser,        ROLES_AS  ) \


namespace API {

class Handler {
protected:
        Handler(const QString& name, const QString& desc,
                const std::initializer_list<Roles::RoleId>& = ROLES_ALL);

public:
        virtual ~Handler();

        struct ParsedCall {
                int handler_id;
                QJsonObject payload;
        };

        static QJsonObject createCall(int handler_id, QJsonObject payload);
        static ParsedCall parseCall(const QJsonObject& call);

        const QString name;
        const QString description;

        // TODO create request class instean QJsonObject
        virtual HndlError exec(QJsonObject&) = 0;

        virtual QJsonObject Serialize() const = 0;

        bool haveAccess(Roles::RoleId) const;

private:
        // create single deserializator and for each handler create input data structure
        virtual HndlError Deserialize(const QJsonObject&) = 0;
                // decode QJsonObject
        const QVector<Roles::RoleId> _accessRoles;
                // roles avalible to perform execution command
};

// TODO add ban user command

// All commands:
// on success return 0 or CommandErrno::OK
// on faild return error number
//
// For all commands appliend sender commands info

namespace HNDL::Config {
        #define XX(id, name, roles) name = id,
        enum HandlerId {
                HANDLERS_MAP(XX)
                COUNT
        };
        #undef XX
}

} /* API */
