#ifndef ADMINMAIN_HPP
#define ADMINMAIN_HPP

#include <QWidget>

namespace Ui {
class AdminMain;
}

class AdminMain : public QWidget {
    Q_OBJECT

    public:
        explicit AdminMain(QWidget *parent = nullptr);
        ~AdminMain();

    private:
        Ui::AdminMain *ui;
};

#endif // ADMINMAIN_HPP
