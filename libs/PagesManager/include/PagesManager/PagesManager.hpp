#ifndef PAGESMANAGER_HPP_AC3HJAY0
#define PAGESMANAGER_HPP_AC3HJAY0

#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QMainWindow>
#include <QStackedWidget>
#include <QPair>
#include <QDebug>
#include <QMap>
#include <QWidget>
#include <QString>

class NavFrame : public QFrame {
    Q_OBJECT

    public:
     NavFrame(const QVector<QString>& pages, bool createBack = true,
              QWidget* p = nullptr);
     virtual ~NavFrame();

    Q_SIGNALS:
        void clicked(QString);

    private:
        QVBoxLayout * _layout;

};

class NavWidget : public QStackedWidget {
    Q_OBJECT

    public:
        NavWidget(QWidget * p = nullptr);
        virtual ~NavWidget();

        void reset();

        int addNav(const QVector<QString>& pages, bool createBack = true);
            // creates new nav frame in stack
            // return nav page uid
        void changeNav(int navUID);
            // change current nav page index to navUID

    Q_SIGNALS:
        void clicked(QString);
};

class PagePathFrame : public QFrame {
    Q_OBJECT

    public:
        PagePathFrame(QWidget * p = nullptr);
        virtual ~PagePathFrame();

        void reset();

        void changePath(const QVector<QString>& path);
        void setDelemiter(const QString& d);

    Q_SIGNALS:
        void clicked(QString);

    private:
        QHBoxLayout * _layout;
        QString _delemiter;
};

class PagesManager : public QFrame {
    Q_OBJECT

    private:
        struct Page {
            QWidget * widget = nullptr;
            int navId = -1;
            QVector<QString> edges = {};
        };

        QMap<QString, Page> _pages;
        QString _root;

        PagePathFrame * _pagePath;
        NavWidget * _nav;
        QStackedWidget * _view;
        QVBoxLayout * _layout_main;
        QHBoxLayout * _layout_sub;

    public:
        PagesManager(QWidget * p = nullptr);
        virtual ~PagesManager();

        void reset();

        int pages();
        void addRoot(const QString& accessName, QWidget*,
                     const QVector<QString>& edges = {});
        void addPage(const QString& accessName, QWidget*,
                     const QVector<QString>& edges = {});
        void finalize();

    public Q_SLOTS:
        void changePage(QString);

    private:
        void bindPages(const QString& parent, const QVector<QString>&);
        QVector<QString> pagePath(const QString& pageName);
};

#endif /* end of include guard: PAGESMANAGER_HPP_AC3HJAY0 */
