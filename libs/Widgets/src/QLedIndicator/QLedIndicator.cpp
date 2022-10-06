#include "Widgets/QLedIndicator.hpp"

#include <QGridLayout>
#include <QLabel>

QLedIndicator::QLedIndicator(QWidget *parent) :
    QWidget(parent),
    mLedState(Inactive),
    mLedSize(20)
{
    setupUi();
}

QLedIndicator::LedState QLedIndicator::ledState() const
{
    return mLedState;
}

int QLedIndicator::ledSize() const
{
    return mLedSize;
}

void QLedIndicator::setLedState(const LedState state)
{
    if(mLedState == state)
        return;

    mLedState = state;
    forceLedState();
    emit ledStateChanged(mLedState);
    emit ledStateChanged(static_cast<int>(mLedState));
}

void QLedIndicator::setLedState(const int state)
{
    setLedState(static_cast<LedState>(state));
}

void QLedIndicator::setLedSize(const int size)
{
    /* if(mLedSize == size || size < 15 || size > 64) */
    if(mLedSize == size || size > 64)
        return;

    mLedSize = size;
    forceLedState();
    this->setFixedSize(mLedSize, mLedSize);
}

void QLedIndicator::setupUi()
{
    initLed();

    this->setFixedSize(mLedSize, mLedSize);
}

void QLedIndicator::initLed()
{
    mLed = new QLabel(this);
    forceLedState();
}

void QLedIndicator::forceLedState()
{
    QPixmap icon;
    switch (mLedState) {
    case Error:
        icon.load(":/QLedIndicator/red");
        break;
    case Inactive:
        icon.load(":/QLedIndicator/gray");
        break;
    case Active:
        icon.load(":/QLedIndicator/green");
        break;
    case Warning:
        icon.load(":/QLedIndicator/orange");
        break;
    }

    mLed->setPixmap(icon.scaled(QSize(mLedSize, mLedSize), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
