#ifndef PROGRESSES_HPP_ANX5GJAI
#define PROGRESSES_HPP_ANX5GJAI

#include <QMainWindow>
#include <QMargins>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QResizeEvent>
#include <QTimer>
#include <QVariant>

#include "Widgets/Notifier/NotifyItem.hpp"
#include "Widgets/Notifier/NotifyItemFactory.hpp"

// TODO improve with linked list items organization
// Dont move this in another thread
class NotifyManager : public QObject {
    Q_OBJECT

    public:
        // TODO
        enum StackDirection {
            StackAbove,
            StackUnder,
        };

        // TODO
        enum DockArea {
            DockLeft,
        };

        explicit NotifyManager(QWidget* mainWindow,
                               QMargins margins,
                               QSize _itemSize, // hixed height and minimum width
                               int maxVisible = 5, //  TODO
                               StackDirection stacking = StackAbove, // TODO
                               QObject* p = nullptr);
        virtual ~NotifyManager();

        static int freeUID();

        /* void setDiactivationDur(int ms); */
        /* void setActivationDur(int ms); */

        /* int diactivationDur() const; */
        /* int activationDur() const; */

    Q_SIGNALS:
        void windowResized(QResizeEvent*);
        void maxWidthChanged(int);

    public Q_SLOTS:
        int activeItemsCount(std::function<bool(NotifyItem*)> filter = [](NotifyItem*){return false;});
        QList<NotifyItem*> activeItems(std::function<bool(NotifyItem*)> filter = [](NotifyItem*){return false;}) const;
        int itemsCount();

        void setItemPropery(int uid, const QByteArray& name, const QVariant& value);
        void createNotifyItem(NotifyItemFactory*, int& uid);

    private Q_SLOTS:
        int findMaxItemWidth();
        QPoint NextPosition(int n);
        void reorganize();
        void resizeItems(int width);
        void hideItem();
        void removeItem();

    private:
        /*const*/ QWidget* _win;  // link, mybe use this->parent() ?
        QSize _winSize;
        QMutex _mtx;

        QMap<int, NotifyItem*> _items;

        bool _hideOnClick;

        int _prevMaxWidth = 0;

        StackDirection _stackDir;
        QMargins _margins;
        int _spacing;
        QSize _itemHints;
            // width - maximum avalible width
            // height - fixed item height
        int _maxActiveItems;
        int _activationDur;
        int _diactivationDur;
};

#endif /* end of include guard: PROGRESSES_HPP_ANX5GJAI */
