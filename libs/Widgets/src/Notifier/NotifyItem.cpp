#include "Widgets/Notifier/NotifyItem.hpp"

#include <QTimer>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QGraphicsOpacityEffect>
#include <qnamespace.h>

NotifyItem::NotifyItem(const QString& title,
           NotifyLevel notifyLevel,
           int completeTmeout,
           int activationDuration,
           int diactivationDuration,
           QSize minSize,
           QWidget* p)
    : QFrame(p),
        _notifyLevel(notifyLevel),
        _activationDur(activationDuration),
        _diactivationDur(diactivationDuration),
        _minSize(minSize),
        _diactivating(false),
        _activating(false),
        _notActivated(true),
        _completeTimeout(completeTmeout)
{
    this->setFrameShadow(QFrame::Sunken);
    this->setFrameShape(QFrame::Shape::StyledPanel);

    this->setWindowFlag(Qt::WindowType::Widget);
    this->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
    this->setMinimumSize(minSize.width(), _minSize.height());
    this->setMaximumSize(this->parentWidget()->size().width() * 0.2, _minSize.height());

    _title = new QLabel(title, this);
    _title->setAlignment(Qt::AlignHCenter);
    _title->setFont(QFont("Monaco", 8));

    _layout = new QVBoxLayout(this);
    _layout->setDirection(QVBoxLayout::Direction::Down);
    _layout->setSpacing(5);
    _layout->setContentsMargins(6, 4, 4, 6);
    _layout->setAlignment(Qt::AlignHCenter);
    _layout->addWidget(_title, 0);
    _layout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);

    _layout->activate();
    this->adjustSize();

    this->setSchemeByType(notifyLevel);

    if (_completeTimeout > 0) {
        QTimer::singleShot(_completeTimeout, this, SLOT(forceComplete()));
    }
}

NotifyItem::~NotifyItem()
{
    delete _title;
    /* delete _closeBtn; */
    delete _layout;
}

void
NotifyItem::forceComplete(NotifyLevel l)
{
    _completeTimeout = 0;
    setSchemeByType(l);
    diactivate();
    connect(this, SIGNAL(diactivated()), this, SIGNAL(completed()));
}

void
NotifyItem::setSchemeByType(NotifyItem::NotifyLevel t) {
    QString scheme = "null";
    switch (t) {
        case NotifyItem::NotifySuccess: scheme = "NotifySuccess";
            break;
        case NotifyItem::NotififyNormal: scheme = "NotifyNormal";
            break;
        case NotifyItem::NotifyWarning: scheme = "NotifyWarning";
            break;
        case NotifyItem::NotifyError: scheme = "NotifyError";
            break;
    }
    /* qDebug() << scheme; */
    this->setObjectName(scheme);
    /* this->setProperty("level", scheme); */
}

void NotifyItem::setUID(int uid) { _uid = uid; }
int NotifyItem::UID() { return _uid; }

NotifyItem::NotifyLevel NotifyItem::notifyLevel() const { return _notifyLevel; }
bool NotifyItem::isOnDiactivation() const               { return _diactivating; }
bool NotifyItem::isOnActivation() const                 { return _activating; }
bool NotifyItem::isNotActivated() const                 { return _notActivated; }
int  NotifyItem::completeTimeout() const                { return _completeTimeout; }

void NotifyItem::setTitle(const QString& title)   { _title->setText(title); adjustSize(); }
void NotifyItem::setNotifyLevel(NotifyLevel type) { _notifyLevel = type; setSchemeByType(type); }
void NotifyItem::setCompleteTimeout(int ms)       { _completeTimeout = ms; }

void NotifyItem::setActive() { _activating = false; }

void
NotifyItem::diactivate()
{
    auto e = new QGraphicsOpacityEffect(this);
    QPropertyAnimation * a = new QPropertyAnimation(e, "opacity");
    a->setDuration(_diactivationDur);
    a->setStartValue(1);
    a->setEndValue(0);
    /* a->setStartValue(this->pos()); */
    /* a->setEndValue(QPoint(this->pos().x(), -this->size().height())); */
    a->setEasingCurve(QEasingCurve::InBack);

    connect(a, SIGNAL(finished()), this, SIGNAL(diactivated()));

    _diactivating = true;
    Q_EMIT onDiactivation();

    this->setGraphicsEffect(e);
    a->start(QPropertyAnimation::DeleteWhenStopped);
}

void
NotifyItem::activate(const QPoint& pos)
{
    _notActivated = false;
    _activating = true;
    Q_EMIT onActivation();

    auto e = new QGraphicsOpacityEffect(this);
    QPropertyAnimation * a = new QPropertyAnimation(e, "opacity");
    a->setDuration(_activationDur);
    a->setStartValue(0);
    a->setEndValue(1);
    /* a->setDuration(_activationDur); */
    /* a->setStartValue(QPoint(pos.x(), this->parentWidget()->size().height() + */
    /*                                  this->size().height())); */
    /* a->setEndValue(pos); */
    a->setEasingCurve(QEasingCurve::InBack);

    this->setGraphicsEffect(e);
    this->move(pos);
    this->show();
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a, SIGNAL(finished()), this, SIGNAL(activated()));
    connect(a, SIGNAL(finished()), this, SLOT(setActive()));
}

void
NotifyItem::on_activated()
{
}

void
NotifyItem::mousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::MouseButton::LeftButton) {
        Q_EMIT clicked();
    }
}
