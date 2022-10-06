#ifndef DUTYSCHEDULE_HPP
#define DUTYSCHEDULE_HPP

#include <QWidget>

namespace Ui {
class DutySchedule;
}

class DutySchedule : public QWidget
{
    Q_OBJECT

public:
    explicit DutySchedule(QWidget *parent = nullptr);
    ~DutySchedule();

private:
    Ui::DutySchedule *ui;
};

#endif // DUTYSCHEDULE_HPP
