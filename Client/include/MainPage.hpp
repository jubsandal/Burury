#ifndef MAINPAGE_HPP
#define MAINPAGE_HPP

#include <QWidget>

namespace Ui {
class MainPage;
}

class MainPage : public QWidget {
    Q_OBJECT

    public:
        explicit MainPage(QWidget *parent = nullptr);
        ~MainPage();

    Q_SIGNALS:
        void loginClicked();
        void registerCliecked();

    private:
        Ui::MainPage *ui;
};

#endif // MAINPAGE_HPP
