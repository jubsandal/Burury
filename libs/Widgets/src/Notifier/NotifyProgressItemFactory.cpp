#include "Widgets/Notifier/NotifyProgressItemFactory.hpp"
#include "Widgets/Notifier/NotifyProgressItem.hpp"

NotifyProgressItemFactory::NotifyProgressItemFactory()
{
}

NotifyProgressItemFactory::~NotifyProgressItemFactory()
{
}

NotifyItem *
NotifyProgressItemFactory::produce(QWidget * p)
{
    return new NotifyProgressItem(
            _exitOnComplete, _maxProgress, _title, _type,
            _cmplTimeout, _aDur, _daDur, _minSize, p);
}

void NotifyProgressItemFactory::setMaximum(int maxProgress) { _maxProgress = maxProgress; }
void NotifyProgressItemFactory::setExitOnCompleted(bool exit) { _exitOnComplete = exit; }
