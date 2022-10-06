#include "Widgets/Notifier/NotifyMsgItemFactory.hpp"
#include "Widgets/Notifier/NotifyMsgItem.hpp"

NotifyMsgItemFactory::NotifyMsgItemFactory()
{
}

NotifyMsgItemFactory::~NotifyMsgItemFactory()
{
}

NotifyItem *
NotifyMsgItemFactory::produce(QWidget * p)
{
    return new NotifyMsgItem(
            _msg, _title, _type,
            _cmplTimeout, _aDur, _daDur, _minSize, p);
}

void NotifyMsgItemFactory::setMsg(const QString& msg) { _msg = msg; }
