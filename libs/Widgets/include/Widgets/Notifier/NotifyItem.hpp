#ifndef NOTIFYITEM_HPP_WKRUDOKF
#define NOTIFYITEM_HPP_WKRUDOKF

#include <QObject>
#include <QWidget>
#include <QPropertyAnimation>
#include <QFrame>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QLayout>

class NotifyItem : public QFrame {
    Q_OBJECT

    Q_PROPERTY(QString title WRITE setTitle)
    Q_PROPERTY(NotifyLevel level MEMBER _notifyLevel READ notifyLevel WRITE setNotifyLevel)
    Q_PROPERTY(int completeTimeout MEMBER _completeTimeout READ completeTimeout WRITE setCompleteTimeout)
    Q_PROPERTY(NotifyLevel forceComplete WRITE forceComplete)

    public:
        enum NotifyLevel {
            NotifySuccess,
            NotififyNormal,
            NotifyWarning,
            NotifyError,
        };
        Q_ENUM(NotifyLevel)

        NotifyItem(const QString& title, NotifyLevel notifyLevel,
                   int completeTmeout,
                   int activationDuration,
                   int diactivationDuration,
                   QSize = {160, 42},
                   QWidget* p = nullptr);
        virtual ~NotifyItem();

        NotifyLevel notifyLevel() const;

        bool isOnDiactivation() const;
        bool isOnActivation() const;
        bool isNotActivated() const;

        int completeTimeout() const;

        void setUID(int);
        int UID();

    Q_SIGNALS:
        void clicked();
        void resized(QResizeEvent*);
        void activated();
        void onActivation(); // appear animation started
        void onDiactivation(); // disapear animation started
        void diactivated(); // disapear animation completed
        void completed(); // diactivated

    public Q_SLOTS:
        void activate(const QPoint& pos);
        void diactivate();
        void forceComplete(NotifyLevel = NotifySuccess);

        void setCompleteTimeout(int ms);
        void setNotifyLevel(NotifyLevel);
        void setTitle(const QString&);

    protected Q_SLOTS:
        void setSchemeByType(NotifyLevel);
        virtual void mousePressEvent(QMouseEvent*) override;
        /* virtual void resizeEvent(QResizeEvent*) override; */

        virtual void on_activated();

        void setActive();

    protected:
        int _uid;

        NotifyLevel _notifyLevel;
        bool _diactivating;
        bool _activating;
        bool _notActivated;
        QLabel       * _title;
        /* QPushButton  * _closeBtn; */
        QVBoxLayout  * _layout;

        QSize _minSize;
            // remove

        int _completeTimeout = 0;
        int _activationDur;
        int _diactivationDur;
};

#endif /* end of include guard: NOTIFYITEM_HPP_WKRUDOKF */
