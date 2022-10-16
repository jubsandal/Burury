#ifndef TABLES_HPP_URXDMEZB
#define TABLES_HPP_URXDMEZB

#include "API/Query.hpp"

#include <QMap>
#include <QString>
#include <QDate>

#define TABLES_MAP(XX) \
        XX( 0, Scripts,    CREATE_SCRIPTS_TABLE     ) \
        XX( 1, UserGrades, CREATE_USER_GRADES_TABLE ) \
        XX( 2, Job,        CREATE_JOB_TABLE         ) \
        XX( 3, Users,      CREATE_USER_TABLE        ) \
        XX( 4, Tokens,     CREATE_TOKENS_TABLE      ) \


namespace API {

        namespace Tables {
                #define XX(num, name, query) name = num,
                        enum Tables {
                                TABLES_MAP(XX)
                                COUNT
                        };
                #undef XX
        } /* Tables */

} /* API */

#endif /* end of include guard: TABLES_HPP_URXDMEZB */
