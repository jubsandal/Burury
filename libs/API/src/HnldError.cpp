#include "API/HndlError.hpp"
#include "API/Utility.hpp"

namespace API {

#define XX(id, name, desc) { QUOTE(name), QUOTE(desc) },
        static struct HandlerErrorDesc_t {
                const char * name;
                const char * desc;
        } const HandlerErrorDesc[] {
                HANDLER_ERRNO_MAP(XX)
        };
#undef XX

        HndlError::HndlError()
                : HndlError(OK)
        {
        }

        HndlError::HndlError(int error_n, QString details)
                : _errno(error_n), _details(details)
        {
        }

        bool    HndlError::Ok()      { return _errno == OK; }
        int     HndlError::Type()    { return _errno; }
        QString HndlError::String()  { return QString(HandlerErrorDesc[_errno].desc) +  ": " + _details; }
        QString HndlError::Name()    { return HandlerErrorDesc[_errno].name; }
        QString HndlError::Details() { return _details; }

} /* API */ 
