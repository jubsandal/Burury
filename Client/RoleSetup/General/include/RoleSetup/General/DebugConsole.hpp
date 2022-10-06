#ifndef DEBUGCONSOLE_HPP
#define DEBUGCONSOLE_HPP

#include <QDialog>

namespace Ui {
class DebugConsole;
}

class DebugConsole : public QDialog
{
    Q_OBJECT

public:
    explicit DebugConsole(QWidget *parent = nullptr);
    ~DebugConsole();

private:
    Ui::DebugConsole *ui;
};

#endif // DEBUGCONSOLE_HPP
