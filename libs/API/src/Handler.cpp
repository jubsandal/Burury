#include "API/Handler.hpp"
#include "API/Role.hpp"

namespace API {

Handler::Handler(const QString& name, const QString& description, const std::initializer_list<Roles::RoleId>& roles)
        : name(name),
        description(description),
        _accessRoles(roles)
{ }

Handler::~Handler()
{ }

bool
Handler::haveAccess(Roles::RoleId role) const
{
        if (!_accessRoles.length() || _accessRoles.indexOf(role) != -1) {
                return true;
        }
        return false;
}

QJsonObject
Handler::createCall(int handler_id, QJsonObject payload)
{
        return QJsonObject{
                { "handler_id", handler_id },
                { "payload", payload }
        };
}

Handler::ParsedCall
Handler::parseCall(const QJsonObject& call)
{
        struct ParsedCall parsed;

        parsed.handler_id = call["handler_id"].toDouble(-1);
        parsed.payload = call["payload"].toObject();

        return parsed;
}

} /* API */ 
