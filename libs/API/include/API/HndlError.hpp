#ifndef CMDERROR_HPP_NSKT1MCP
#define CMDERROR_HPP_NSKT1MCP

#include <QString>

#define HANDLER_ERRNO_MAP(XX) \
        XX( 0, OK,             "Success" )                   \
        XX( 1, InvalidParam,   "Invalid command parameter" ) \
        XX( 2, AccessDenied,   "Access denied" )             \
        XX( 3, InvalidCommand, "Invalid command passed" )    \
        XX( 4, MiscError,      "Misc error" )                \
        XX( 5, SQLError,       "SQL Query Error" )           \

namespace API {

#define XX(id, name, str) name = id,
        enum HandlerErrorNo {
                HANDLER_ERRNO_MAP(XX)
        };
#undef XX

        class HndlError {
                public:
                        HndlError(); //mean no error
                        HndlError(int, QString = "");

                        bool Ok();

                        int Type();
                        QString String();
                        QString Name();
                        QString Details();

                private:
                        int _errno;
                        QString _details;
        };

} /* API */ 

#endif /* end of include guard: CMDERROR_HPP_NSKT1MCP */
