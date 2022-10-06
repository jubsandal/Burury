#ifndef NOTIFYPROGRESSITEM_HPP_WF9NQV3H
#define NOTIFYPROGRESSITEM_HPP_WF9NQV3H

#include "Widgets/Notifier/NotifyItem.hpp"

class NotifyProgressItem : public NotifyItem {
    Q_OBJECT

    Q_PROPERTY(int progress WRITE setProgress)
    Q_PROPERTY(int maxProgress WRITE setMaxProgress)
    Q_PROPERTY(bool exitOnComplete MEMBER _exitOnComplete WRITE setExitOnCompleted)

    public:
     NotifyProgressItem(bool exitOnComplete,
                        int maxProgress,
                        const QString& title,
                        NotifyLevel notifyType,
                        int completeTmeout,
                        int activationDuration,
                        int diactivationDuration,
                        QSize = {160, 40}, QWidget* p = nullptr);
     ~NotifyProgressItem();

    Q_SIGNALS:
        void progressChanged(int);

    public Q_SLOTS:
        void setExitOnCompleted(bool);
        void setProgress(int count);
        void setMaxProgress(int count);
        void endProgress(NotifyLevel success);

    private:
        QProgressBar * _pbar;
        bool _exitOnComplete;

};

#endif /* end of include guard: NOTIFYPROGRESSITEM_HPP_WF9NQV3H */
