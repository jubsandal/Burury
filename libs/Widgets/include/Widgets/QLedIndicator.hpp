#ifndef QLEDINDICATOR_H_TKD8G2PZ
#define QLEDINDICATOR_H_TKD8G2PZ

#include <QWidget>

class QGridLayout;
class QLabel;

class QLedIndicator : public QWidget {
        Q_OBJECT

        Q_PROPERTY(LedState ledState READ ledState WRITE setLedState NOTIFY ledStateChanged)
        Q_PROPERTY(int ledSize READ ledSize WRITE setLedSize)

    public:
        enum LedState { Error = -1, Inactive = 0, Active = 1, Warning = 2 };
        Q_ENUMS(LedState)

        QLedIndicator(QWidget *parent = nullptr);

        LedState ledState() const;
        int ledSize() const;

    signals:
        void ledStateChanged(const LedState state);
        void ledStateChanged(const int state);

    public Q_SLOTS:
        void setLedState(const LedState state);
        void setLedState(const int state);
        void setLedSize(const int size);

    private:
        inline void setupUi();
        inline void initLed();
        void forceLedState();

    private:
        QLabel* mLed;

        LedState mLedState;
        int mLedSize;
};

#endif /* end of include guard: QLEDINDICATOR_H_TKD8G2PZ */
