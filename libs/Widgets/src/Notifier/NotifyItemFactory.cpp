#include "Widgets/Notifier/NotifyItemFactory.hpp"

NotifyItemFactory::NotifyItemFactory()
{
}

NotifyItemFactory::~NotifyItemFactory()
{
}

void NotifyItemFactory::setTitle(const QString& t) { _title = t; }
void NotifyItemFactory::setNotifyLevel(NotifyItem::NotifyLevel t) { _type = t; }
void NotifyItemFactory::setCompleteTimeout(int ms) { _cmplTimeout = ms; }
void NotifyItemFactory::setActivationDuration(int ms) { _aDur = ms; }
void NotifyItemFactory::setDiactivationDuration(int ms) { _daDur = ms; }
void NotifyItemFactory::setSizeHints(const QSize& s) { _minSize = s; }
