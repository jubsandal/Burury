#ifndef ACCIDENTS_HPP
#define ACCIDENTS_HPP

#include <QWidget>

namespace Ui {
class Accidents;
}

class Accidents : public QWidget
{
    Q_OBJECT

public:
    explicit Accidents(QWidget *parent = nullptr);
    ~Accidents();

private:
    Ui::Accidents *ui;
};

#endif // ACCIDENTS_HPP
