#include "Database/CmdError.hpp"
#include "Database/Utility.hpp"

namespace API {

#define XX(id, name, desc) { QUOTE(name), QUOTE(desc) },
        static struct CommandErrorDesc_t {
                const char * name;
                const char * desc;
        } const CommandErrorDesc[] {
                COMMANDS_ERRNO_MAP(XX)
        };
#undef XX

        CmdError::CmdError()
                : CmdError(OK)
        {
        }

        CmdError::CmdError(int error_n, QString details)
                : _errno(error_n), _details(details)
        {
        }

        bool    CmdError::Ok()      { return _errno == OK; }
        int     CmdError::Type()    { return _errno; }
        QString CmdError::String()  { return QString(CommandErrorDesc[_errno].desc) +  ": " + _details; }
        QString CmdError::Name()    { return CommandErrorDesc[_errno].name; }
        QString CmdError::Details() { return _details; }

} /* API */ 
