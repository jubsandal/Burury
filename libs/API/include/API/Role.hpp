#ifndef ROLE_HPP_TLNRD93C
#define ROLE_HPP_TLNRD93C

#include <QString>

namespace API {

// id, name
#define ROLE_MAP(XX) \
        XX( 0, SYS    ) \
        XX( 1, Admin  ) \
        XX( 2, Worker ) \
        XX( 3, Manager) \

        namespace Roles {
                #define XX(id, name) name = id,
                        enum RoleId {
                                ROLE_MAP(XX)
                                COUNT
                        };
                #undef XX
        } /* Roles */

} /* API */ 

#endif /* end of include guard: ROLE_HPP_TLNRD93C */
