#ifndef USERSLIST_HPP
#define USERSLIST_HPP

#include <QWidget>

namespace Ui {
class UsersList;
}

class UsersList : public QWidget
{
    Q_OBJECT

public:
    explicit UsersList(QWidget *parent = nullptr);
    ~UsersList();

private:
    Ui::UsersList *ui;
};

#endif // USERSLIST_HPP
