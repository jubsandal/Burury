#ifndef NOTIFYITEMFACTORY_HPP_VOFLIWUB
#define NOTIFYITEMFACTORY_HPP_VOFLIWUB

#include "Widgets/Notifier/NotifyItem.hpp"

class NotifyItemFactory {
    public:
        NotifyItemFactory();
        virtual ~NotifyItemFactory();

        virtual NotifyItem * produce(QWidget * p = nullptr) = 0;

        void setTitle(const QString&);
        void setNotifyLevel(NotifyItem::NotifyLevel);
        void setCompleteTimeout(int ms);
        void setActivationDuration(int ms);
        void setDiactivationDuration(int ms);
        void setSizeHints(const QSize&);

    protected:
        QString _title = "No title";
        NotifyItem::NotifyLevel _type = NotifyItem::NotififyNormal;
        int _cmplTimeout = 0;
        int _aDur = 1000;
        int _daDur = 1000;
        QSize _minSize = {100, 40};
};

#endif /* end of include guard: NOTIFYITEMFACTORY_HPP_VOFLIWUB */
