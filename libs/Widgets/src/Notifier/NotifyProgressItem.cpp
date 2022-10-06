#include "Widgets/Notifier/NotifyProgressItem.hpp"
#include <qnamespace.h>

NotifyProgressItem::NotifyProgressItem(
    bool exitOnComplete,
    int maxProgress,
    const QString& title,
    NotifyLevel notifyLevel,
    int completeTmeout,
    int activationDuration,
    int diactivationDuration,
    QSize minSize,
    QWidget* p)
    : NotifyItem(title, notifyLevel, completeTmeout, activationDuration, diactivationDuration, minSize, p),
        _exitOnComplete(exitOnComplete),
        _pbar(new QProgressBar(this))
{
    _pbar->setRange(0, maxProgress);
    _layout->addWidget(_pbar, 0, Qt::AlignBottom);
    _pbar->setAlignment(Qt::AlignBottom);
    _pbar->setFormat(""); // TODO make as property

    connect(_pbar, SIGNAL(valueChanged(int)), this, SIGNAL(progressChanged(int)));
}

NotifyProgressItem::~NotifyProgressItem()
{
    delete _pbar;
}

void NotifyProgressItem::setExitOnCompleted(bool exit) { _exitOnComplete = exit; }

void
NotifyProgressItem::setProgress(int count)
{
    _pbar->setValue(count);
    if (count >= _pbar->maximum() && _exitOnComplete) {
        setSchemeByType(NotifySuccess);
        /* diactivate(); */
        /* connect(this, SIGNAL(diactivated()), this, SIGNAL(completed())); */
        Q_EMIT completed();
    }
}

void
NotifyProgressItem::setMaxProgress(int max)
{
    _pbar->setMaximum(max);
}

void
NotifyProgressItem::endProgress(NotifyLevel type)
{
    setSchemeByType(type);
    diactivate();
    connect(this, SIGNAL(diactivated()), this, SIGNAL(completed()));
}
