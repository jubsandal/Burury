#ifndef NOTIFYMSGITEM_HPP_5QXIAV3D
#define NOTIFYMSGITEM_HPP_5QXIAV3D

#include "Widgets/Notifier/NotifyItem.hpp"

class NotifyMsgItem : public NotifyItem {
    Q_OBJECT

    Q_PROPERTY(QString message WRITE setMessage)

    public:
        NotifyMsgItem(const QString& title,
                      const QString& msg,
                      NotifyLevel notifyLevel,
                      int completeTmeout,
                      int activationDuration,
                      int diactivationDuration,
                      QSize = {160, 40}, QWidget* p = nullptr);
        virtual ~NotifyMsgItem();

    public Q_SLOTS:
        void setMessage(const QString& msg);

    protected:
        QLabel * _msg;
};


#endif /* end of include guard: NOTIFYMSGITEM_HPP_5QXIAV3D */
