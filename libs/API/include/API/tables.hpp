#ifndef TABLES_HPP_URXDMEZB
#define TABLES_HPP_URXDMEZB

#include "Database/Query.hpp"

#include <QMap>
#include <QString>
#include <QDate>

#define TABLES_MAP(XX) \
        XX(  0, AssignedEmployees,     CREATE_ASSIGNED_EMPLOYEES_TABLE )      \
        XX(  1, Accidents,             CREATE_ACCIDENTS_TABLE )               \
        XX(  2, Accounting,            CREATE_ACCOUNTING_TABLE )              \
        XX(  3, AccountingType,        CREATE_ACCOUNTING_TYPE_TABLE )         \
        XX(  4, Contracts,             CREATE_CONTRACTS_TABLE )               \
        XX(  5, DutySchedule,          CREATE_DUTY_SCHEDULE_TABLE )           \
        XX(  6, Users, CREATE_USERS_TABLE ) \
        XX(  7, RoleCommands,          CREATE_ROLE_COMMANDS_TABLE )           \
        XX(  8, Role,                  CREATE_ROLES_TABLE )                   \
        XX(  9, Wapons,                CREATE_WAPONS_TABLE )                  \
        XX( 10, objectType,            CREATE_OBJECT_TYPE_TABLE )             \

namespace API {

#define XX(num, name, query) TABLE_ ##name = num,
        enum Tables {
                TABLES_MAP(XX)
                TABLES_COUNT
        };
#undef XX

} /* API */

#endif /* end of include guard: TABLES_HPP_URXDMEZB */
