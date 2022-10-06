#ifndef ROLE_HPP_TLNRD93C
#define ROLE_HPP_TLNRD93C

#include <QString>

#include "Database/command.hpp"
#include "Database/tables.hpp"

namespace API {

#define AdminCommands { \
        CMD_EDIT_OBJECT_TYPE,     \
        CMD_MAKE_DUTY_SCHEDULE,   \
        CMD_REGISTER_ACCIDENT,    \
        CMD_REGISTER_EMPLOYEE,    \
        CMD_GET_USER_INFO,   \
        CMD_REGISTER_OBJECT_TYPE, \
        CMD_UPDATE_ROLE,          \
        CMD_GET_ROLE_DETAILS,     \
        CMD_GET_OBJECT_DETAILS,   \
        CMD_GET_DUTY_SCHEDULE,    \
}

#define SecurityCommands { \
        CMD_PAY_ACCIDENT,         \
        CMD_GET_ROLE_DETAILS,     \
        CMD_GET_OBJECT_DETAILS,   \
        CMD_GET_USER_INFO,   \
        CMD_GET_ACCIDENT_DETAILS, \
        CMD_GET_WAPON_DETAILS,    \
        CMD_GET_DUTY_SCHEDULE,    \
}

#define InkosorCommands { \
        CMD_PAY_ACCIDENT,         \
        CMD_GET_ROLE_DETAILS,     \
        CMD_GET_OBJECT_DETAILS,   \
        CMD_GET_USER_INFO,   \
        CMD_GET_ACCIDENT_DETAILS, \
        CMD_GET_WAPON_DETAILS,    \
        CMD_GET_DUTY_SCHEDULE,    \
}

#define RecruterCommands { \
        CMD_REGISTER_EMPLOYEE,  \
        CMD_GET_ROLE_DETAILS,   \
        CMD_GET_OBJECT_DETAILS, \
        CMD_GET_USER_INFO,   \
        CMD_GET_DUTY_SCHEDULE,  \
}

#define WaponManagerCommands { \
        CMD_REGISTER_WAPON,     \
        CMD_ASSIGN_WAPON,       \
        CMD_GET_ROLE_DETAILS,   \
        CMD_GET_USER_INFO,   \
        CMD_GET_OBJECT_DETAILS, \
        CMD_GET_WAPON_DETAILS,  \
        CMD_GET_DUTY_SCHEDULE,  \
}

#define CusomerCommands { \
        CMD_GET_DUTY_SCHEDULE,\
        CMD_GET_USER_INFO,   \
        CMD_GET_OBJECT_DETAILS, \
        CMD_GET_ROLE_DETAILS, \
}

// id, name, commands, privilegyID, payMult, payPeriod
// AUTO - only initiated by server logic automaticaly and have all permissions
#define ROLE_MAP(XX) \
        XX( -1, AUTO,         0,   0,  {} ) \
        XX( 0,  Admin,        1,   30, AdminCommands ) \
        XX( 1,  Security,     1.2, 30, SecurityCommands ) \
        XX( 2,  Inkosor,      1.4, 30, InkosorCommands ) \
        XX( 3,  Recruter,     0.9, 30, RecruterCommands ) \
        XX( 4,  WaponManager, 1,   30, WaponManagerCommands ) \
        XX( 5,  Customer,     0,   0,  CusomerCommands ) \

#define XX(id, name, mult, period, commands) ROLE_ ##name = id,
enum RoleId {
        ROLE_MAP(XX)
        ROLES_COUNT
};
#undef XX

} /* API */ 

#endif /* end of include guard: ROLE_HPP_TLNRD93C */
