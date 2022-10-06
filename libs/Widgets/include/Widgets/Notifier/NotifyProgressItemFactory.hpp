#ifndef NOTIFYPROGRESSFACTORY_HPP_5JG6ZKSA
#define NOTIFYPROGRESSFACTORY_HPP_5JG6ZKSA

#include "Widgets/Notifier/NotifyItemFactory.hpp"

class NotifyProgressItemFactory : public NotifyItemFactory {
    public:
        NotifyProgressItemFactory();
        virtual ~NotifyProgressItemFactory();

        virtual NotifyItem * produce(QWidget * p = nullptr) override;

        void setMaximum(int maxProgress);
        void setExitOnCompleted(bool exit);

    protected:
        int _maxProgress = 0;
        bool _exitOnComplete = false;
};


#endif /* end of include guard: NOTIFYPROGRESSFACTORY_HPP_5JG6ZKSA */
