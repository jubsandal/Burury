#ifndef ACCIDENTLIST_HPP
#define ACCIDENTLIST_HPP

#include <QWidget>

namespace Ui {
class AccidentList;
}

class AccidentList : public QWidget
{
    Q_OBJECT

public:
    explicit AccidentList(QWidget *parent = nullptr);
    ~AccidentList();

private:
    Ui::AccidentList *ui;
};

#endif // ACCIDENTLIST_HPP
