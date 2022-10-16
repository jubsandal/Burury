#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <QWidget>
#include <QJsonObject>

#include "Burury/Service/Service.hpp"

namespace Ui {
class Profile;
}

class Profile : public QWidget {
Q_OBJECT

public:
        explicit Profile(int userID, bool HideWapon = false, QWidget *parent = nullptr);
        ~Profile();

        Q_SIGNALS:
        void loadPersonInfo(int, QJsonObject, ResponseWaiter *);

public Q_SLOTS:
        void personInfoLoaded(QJsonObject);
        void personInfoLoadFailed(int, QString);

private:
        Ui::Profile *ui;
        ResponseWaiter * _waiter;
};

#endif // PROFILE_HPP
