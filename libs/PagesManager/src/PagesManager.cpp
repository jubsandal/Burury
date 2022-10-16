#include "PagesManager/PagesManager.hpp"
#include <functional>
#include <qnamespace.h>

#include "Widgets/ClickableLabel.hpp"

#include <QLine>
#include <QException>

#include "Crypto/Hash.hpp"

static const QString GO_BACK_SEQUENCE = saltGen(charWeakness::Alpha | charWeakness::Digit).toHex();

NavFrame::NavFrame(const QVector<QString>& pages, bool createBack, QWidget * p) : QFrame(p)
{
        _layout = new QVBoxLayout(this);
        _layout->setAlignment(Qt::AlignTop);
        _layout->setSpacing(0);

        for (auto p : pages) {
                QPushButton * btn = new QPushButton(this);
                btn->setText(p);
                connect(btn, &QPushButton::clicked, [this, p] { clicked(p); });
                _layout->addWidget(btn);
        }
        if (createBack) {
                QPushButton * backBtn = new QPushButton(this);
                backBtn->setText("Back");
                connect(backBtn, &QPushButton::clicked, [this] { clicked(GO_BACK_SEQUENCE); });
                _layout->addWidget(backBtn);
        }

        // this->setFrameShape(QFrame::Shape::Box);
        // this->setFrameShadow(QFrame::Shadow::Sunken);

        adjustSize();
}

NavFrame::~NavFrame()
{
        QLayoutItem * itm;
        while ((itm = _layout->takeAt(0)) != nullptr) {
                delete itm->widget();
                delete itm;
        }
}

int
NavFrame::entries() const {
        return _layout->count();
}

/* ------------------------------------------- */

NavWidget::NavWidget(QWidget * p) : QStackedWidget(p)
{
        // this->setFrameShape(QFrame::Shape::Box);
        this->setContentsMargins(0, 0, 0, 0);
}

NavWidget::~NavWidget()
{
        reset();
}

void
NavWidget::reset()
{
        QWidget * w;
        while ((w = this->currentWidget()) != nullptr) {
                delete w;
        }
}

int
NavWidget::addNav(const QVector<QString>& pages, bool createBack)
{
        NavFrame * fnav = new NavFrame(pages, createBack, this);
        this->addWidget(fnav);
        connect(fnav, SIGNAL(clicked(QString)), this, SIGNAL(clicked(QString)));
        adjustSize();
        return this->count()-1;
}

void
NavWidget::changeNav(int id)
{
        this->setCurrentIndex(id);
}

const NavFrame*
NavWidget::getNav(int navUID) const
{
        return dynamic_cast<NavFrame*>(this->widget(navUID));
}

/* ------------------------------------------- */

PagePathFrame::PagePathFrame(QWidget * p)
        : QFrame(p),
        _delemiter("->")
{
        _layout = new QHBoxLayout(this);
        _layout->setAlignment(Qt::AlignLeft);
        this->setFrameShape(QFrame::Shape::WinPanel);
        this->setFrameShadow(QFrame::Shadow::Raised);
        this->layout()->setContentsMargins(5, 2, 5, 2);
        this->layout()->setSpacing(2);
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        adjustSize();
}

PagePathFrame::~PagePathFrame()
{
        reset();
        delete _layout;
}

void
PagePathFrame::reset()
{
        QLayoutItem * itm;
        while ((itm = _layout->takeAt(0)) != nullptr) {
                delete itm->widget();
                delete itm;
        }
}

// TODO redraw on set
void PagePathFrame::setDelemiter(const QString& d) { _delemiter = d; }

void
PagePathFrame::changePath(const QVector<QString>& path)
{
        reset();
        for (auto node : path) {
                QWidget * lbl = new QLabel(_delemiter, this);
                lbl->setFont(QFont("Jet Brains Mono", 9));
                lbl->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
                _layout->addWidget(lbl);
                ClickableLabel * clbl = new ClickableLabel(node, this);
                clbl->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
                clbl->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
                clbl->setStyleSheet("color: #b78620");
                connect(clbl, &ClickableLabel::clicked, [this, node] { Q_EMIT clicked(node); });
                _layout->addWidget(clbl);
        }
        adjustSize();
}

/* ------------------------------------------- */

PagesManager::PagesManager(QWidget * p) : QFrame(p)
{
        _view = new QStackedWidget(this);
        _nav = new NavWidget(this);
        _pagePath = new PagePathFrame(this);

        _view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        _pagePath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        _nav->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

        _layout_main = new QVBoxLayout(this);
        _layout_sub = new QHBoxLayout(this);

        _layout_main->addWidget(_pagePath, 0, Qt::AlignTop);
        _layout_main->addLayout(_layout_sub);
        _layout_sub->addWidget(_nav, 0, Qt::AlignTop);
        _layout_sub->addWidget(_view);

        connect(_nav, SIGNAL(clicked(QString)),
                this, SLOT(changePage(QString)));
        connect(_pagePath, SIGNAL(clicked(QString)),
                this, SLOT(changePage(QString)));

        this->adjustSize();
}

PagesManager::~PagesManager()
{
        reset();
}

void
PagesManager::reset()
{
        for (auto p : _pages) {
                if (p.widget) {
                        QString name = p.widget->objectName();
                        _view->removeWidget(p.widget);
                        delete p.widget;
                        _pages.remove(name);
                }
        }
        _nav->reset();
        _pagePath->reset();
}

void
PagesManager::bindPages(const QString& parent, const QVector<QString>& childs)
{
        int nid = _nav->addNav(childs, parent != _root);
        _pages[parent].navId = nid;
        for (auto child : childs) {
                if (_pages[child].edges.length() > 0) {
                        bindPages(child, _pages[child].edges);
                } else {
                        _pages[child].navId = nid;
                }
        }
}

void
PagesManager::finalize()
{
        if (_root == QString()) {
                throw "Cannot finalize PagesManager without root page";
        }
        bindPages(_root, _pages[_root].edges);

        changePage(_root);

        if (_nav->isHidden()) {
                _nav->show();
        }

        qInfo() << _nav->count();
        if (_nav->count() == 1 && _nav->getNav(0)->entries() == 0) {
                _nav->hide();
        }
}

void
PagesManager::changePage(QString page)
{
        if (page == GO_BACK_SEQUENCE) {
                auto path = pagePath(this->_view->currentWidget()->objectName());
                page = path.at(path.length()-2);
        }

        _view->setCurrentWidget(_pages[page].widget);
        _nav->changeNav(_pages[page].navId);
        _pagePath->changePath(pagePath(page));

        this->adjustSize();
}

int PagesManager::pages() { return _pages.size(); }

void
PagesManager::addRoot(const QString& name, QWidget* wp,
                      const QVector<QString>& edges)
{
        _root = name;
        addPage(name, wp, edges);
}

void
PagesManager::addPage(const QString& name, QWidget* wp,
                      const QVector<QString>& edges)
{
        if (!wp) {
                throw QString("empty widgt pathed");
        }
        wp->setObjectName(name);
        //add check for existance
        _pages[name] = {wp, -1, edges};
        _view->addWidget(wp);
}

QVector<QString>
PagesManager::pagePath(const QString& page)
{
        QVector<QString> path { page };
        QString search = page;
        bool done = false;
        int tries = 0;

        while (!done && tries < _pages.size() + 1) {
                for (auto i : _pages) {
                        if (search == _root) {
                                done = true;
                                break;
                        }
                        for (auto node : i.edges) {
                                if (node == search) {
                                        search = i.widget->objectName();
                                        path.push_front(search);
                                        // exit outer?
                                        break;
                                }
                        }
                }
                tries++;
        }

        return path;
}
