#ifndef Jobs_H
#define Jobs_H

#include <QWidget>

namespace Ui {
class Jobs;
}

class Jobs : public QWidget {
Q_OBJECT

public:
        explicit Jobs(QWidget *parent = nullptr);
        ~Jobs();

private:
        Ui::Jobs *ui;
};

#endif // Jobs_H
