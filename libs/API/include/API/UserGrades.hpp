#pragma once

// id, title, description, value
#define USER_GRADES_MAP(XX) \
        XX( 0, Minion, "Just a minion, not allowed to high-rang work.", 1 ) \


namespace API {

        namespace UserGrades {
                #define XX(id, title, description, value) title = id,
                        enum UserGradesId {
                                USER_GRADES_MAP(XX)
                                COUNT
                        };
                #undef XX
        } /* UserGrades */
        
} /* API */
