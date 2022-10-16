#ifndef Liderboard_H
#define Liderboard_H

#include <QWidget>

namespace Ui {
class Liderboard;
}

class Liderboard : public QWidget {
Q_OBJECT

public:
        explicit Liderboard(QWidget *parent = nullptr);
        ~Liderboard();

private:
        Ui::Liderboard *ui;
};

#endif // Liderboard_H
