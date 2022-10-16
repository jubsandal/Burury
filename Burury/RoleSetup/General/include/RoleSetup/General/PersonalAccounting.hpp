#ifndef PRSONALACCOUNTING_HPP
#define PRSONALACCOUNTING_HPP

#include <QWidget>

namespace Ui {
class PersonalAccounting;
}

class PersonalAccounting : public QWidget
{
    Q_OBJECT

public:
    explicit PersonalAccounting(QWidget *parent = nullptr);
    ~PersonalAccounting();

private:
    Ui::PersonalAccounting *ui;
};

#endif // PRSONALACCOUNTING_HPP
