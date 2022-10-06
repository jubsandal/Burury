#ifndef NOTIFYMSGITEMFACTORY_HPP_AF9CTNBD
#define NOTIFYMSGITEMFACTORY_HPP_AF9CTNBD

#include "Widgets/Notifier/NotifyItemFactory.hpp"

class NotifyMsgItemFactory : public NotifyItemFactory {
    public:
        NotifyMsgItemFactory();
        virtual ~NotifyMsgItemFactory();

        virtual NotifyItem * produce(QWidget * p = nullptr) override;

        void setMsg(const QString& msg);

    protected:
        QString _msg = "Message";
};

#endif /* end of include guard: NOTIFYMSGITEMFACTORY_HPP_AF9CTNBD */
