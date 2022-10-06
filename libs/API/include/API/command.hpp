#ifndef COMMAND_HPP_KQHNGFS8
#define COMMAND_HPP_KQHNGFS8

#include "API/CmdError.hpp"

#include <QObject>
#include <QJsonDocument>
#include <QRunnable>

// id, name, validator, executor
#define COMMANDS_MAP(XX) \
        XX( 0,  MAKE_CONTRACT,        exec_make_contract      ) \
        XX( 1,  MAKE_DUTY_SCHEDULE,   exec_make_duty_schedule ) \
        XX( 2,  REGISTER_ACCIDENT,    exec_register_accident  ) \
        XX( 3,  REGISTER_EMPLOYEE,    exec_register_employee  ) \
        XX( 4,  REGISTER_CUSTOMER,    exec_register_customer  ) \
        XX( 5,  REGISTER_RECRUT,      exec_register_recrut    ) \
        XX( 6,  REGISTER_OBJECT_TYPE, exec_register_object_type) \
        XX( 7,  REGISTER_WAPON,       exec_register_wapon     ) \
        XX( 8,  ASSIGN_WAPON,         exec_assign_wapon       ) \
        XX( 9,  PAY_AMMO,             exec_pay_ammo           ) \
        XX( 10, PAY_EMPLOYEE,         exec_pay_employee       ) \
        XX( 11, PAY_ACCIDENT,         exec_pay_accident       ) \
        XX( 12, EDIT_OBJECT_TYPE,     exec_edit_object_type   ) \
        XX( 13, UPDATE_ROLE,          exec_update_role        ) \
        XX( 14, GET_USER_INFO,        exec_get_user_info      ) \
        XX( 15, GET_CONTRACT_DETAILS, exec_get_contract_details ) \
        XX( 16, GET_ACCIDENT_DETAILS, exec_get_accident_details) \
        XX( 17, GET_ACCOUNTING_ENTRY, exec_get_accounting_entry) \
        XX( 18, GET_OBJECT_DETAILS,   exec_get_object_detalils) \
        XX( 19, GET_ROLE_DETAILS,     exec_get_role_details   ) \
        XX( 20, GET_WAPON_DETAILS,    exec_get_wapon_details  ) \
        XX( 21, GET_DUTY_SCHEDULE,    exec_get_duty_schedule  ) \
        XX( 22, CREATE_TABLE,         exec_create_table       ) \
        XX( 23, IDENTIFY,             exec_identify           ) \

QPixmap QPixmapFromQString(const QString& str);
QIcon   QIconFromQString(const QString& str);
QImage  QImageFromQString(const QString& str);
QString QStringFromQImage(const QImage& img);

namespace API {

        using command_exec_t = std::function<CmdError(QJsonObject&)>;

        // AUTO only commands
        /* CmdError exec_send_moneys(QJsonObject& obj); */
        CmdError exec_create_table(QJsonObject& obj);
        CmdError exec_identify(QJsonObject& obj);

        // Role avalible commands
        CmdError exec_make_contract(QJsonObject& obj);
        CmdError exec_make_duty_schedule(QJsonObject& obj);
        CmdError exec_register_accident(QJsonObject& obj);
        CmdError exec_register_employee(QJsonObject& obj);
        CmdError exec_register_customer(QJsonObject& obj);
        CmdError exec_register_recrut(QJsonObject& obj);
        CmdError exec_register_object_type(QJsonObject& obj);
        CmdError exec_register_wapon(QJsonObject& obj);
        CmdError exec_assign_wapon(QJsonObject& obj);
        CmdError exec_pay_ammo(QJsonObject& obj);
        CmdError exec_pay_employee(QJsonObject& obj);
        CmdError exec_pay_accident(QJsonObject& obj);
        CmdError exec_edit_object_type(QJsonObject& obj);
        CmdError exec_update_role(QJsonObject& obj);
        CmdError exec_get_contract_details(QJsonObject& obj);
        CmdError exec_get_user_info(QJsonObject& obj);
        CmdError exec_get_accident_details(QJsonObject& obj);
        CmdError exec_get_accounting_entry(QJsonObject& obj);
        CmdError exec_get_object_detalils(QJsonObject& obj);
        CmdError exec_get_role_details(QJsonObject& obj);
        CmdError exec_get_wapon_details(QJsonObject& obj);
        CmdError exec_get_duty_schedule(QJsonObject& obj);
        // mean validate and execute
        // on success return 0 or CommandErrno::OK
        // on faild return error number

#define XX(id, name, e) CMD_ ##name = id,
        enum CommandId {
                COMMANDS_MAP(XX)
                COMMANDS_COUNT
        };
#undef XX

} /* API */
#endif /* end of include guard: COMMAND_HPP_KQHNGFS8 */
