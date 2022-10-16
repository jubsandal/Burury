#ifndef REGISTERACCIDENTDIALOG_HPP
#define REGISTERACCIDENTDIALOG_HPP

#include <QDialog>

namespace Ui {
class RegisterAccidentDialog;
}

class RegisterAccidentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterAccidentDialog(QWidget *parent = nullptr);
    ~RegisterAccidentDialog();

private:
    Ui::RegisterAccidentDialog *ui;
};

#endif // REGISTERACCIDENTDIALOG_HPP
