#ifndef CUSTOMERMAIN_HPP
#define CUSTOMERMAIN_HPP

#include <QWidget>

namespace Ui {
class CustomerMain;
}

class CustomerMain : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerMain(QWidget *parent = nullptr);
    ~CustomerMain();

private:
    Ui::CustomerMain *ui;
};

#endif // CUSTOMERMAIN_HPP
