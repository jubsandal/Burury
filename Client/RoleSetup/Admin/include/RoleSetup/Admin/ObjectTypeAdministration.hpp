#ifndef OBJECTTYPEADMINISTRATION_HPP
#define OBJECTTYPEADMINISTRATION_HPP

#include <QWidget>

namespace Ui {
class ObjectTypeAdministration;
}

class ObjectTypeAdministration : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectTypeAdministration(QWidget *parent = nullptr);
    ~ObjectTypeAdministration();

private:
    Ui::ObjectTypeAdministration *ui;
};

#endif // OBJECTTYPEADMINISTRATION_HPP
