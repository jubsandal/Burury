#ifndef ROLEMANAGMENT_HPP
#define ROLEMANAGMENT_HPP

#include <QWidget>

namespace Ui {
class RoleManagment;
}

class RoleManagment : public QWidget
{
    Q_OBJECT

public:
    explicit RoleManagment(QWidget *parent = nullptr);
    ~RoleManagment();

private:
    Ui::RoleManagment *ui;
};

#endif // ROLEMANAGMENT_HPP
