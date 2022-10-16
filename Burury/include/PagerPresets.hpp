#ifndef PAGERPRESETS_HPP_CCDGDJA4
#define PAGERPRESETS_HPP_CCDGDJA4

#define MAIN_PAGE_ID -1092837
// try true unique

#include <functional>
#include <QMap>

#include "Burury/Service/Service.hpp"

#include "PagesManager/PagesManager.hpp"
#include "API/Role.hpp"

#include "MainPage.hpp"

#include "RoleSetup/Admin/AdminMain.hpp"
#include "RoleSetup/Admin/RoleManagment.hpp"
#include "RoleSetup/Admin/UsersList.hpp"

#include "RoleSetup/General/DebugConsole.hpp"
#include "RoleSetup/General/PersonalAccounting.hpp"
#include "RoleSetup/General/Profile.hpp"

#include "Burury.hpp"

using pagemanSetuper = std::function<void(Burury *, PagesManager *, Service *)>;

inline pagemanSetuper mainPage = [](Burury * w, PagesManager * pm, Service *) {
        pm->reset();
        QWidget * ml = new MainPage;
        pm->addRoot("Main", ml);

        ml->connect(ml, SIGNAL(loginClicked()), w, SLOT(showLogin()));
        ml->connect(ml, SIGNAL(registerCliecked()), w, SLOT(showRegister()));

        pm->finalize();
};

inline pagemanSetuper admin = [](Burury * w, PagesManager * pm , Service * serv) {
        pm->reset();

        QWidget * profile = new Profile(w->userId());

        w->connect(
                        profile, SIGNAL(loadPersonInfo(QJsonObject, ResponseWaiter *)),
                        serv, SLOT(sendCommand(QJsonObject, ResponseWaiter *)));

        pm->addRoot("Admin", new AdminMain, { "Profile", "Accidents", "Object types", "PSC Roles", "Users list", "Help" });
        pm->addPage("Profile", profile, { "Accounting", "Schedule" });
        pm->addPage("Accounting", new PersonalAccounting);
        pm->addPage("Users list", new UsersList);

        pm->finalize();
};

inline pagemanSetuper worker = [](Burury *w, PagesManager * pm, Service *serv) {
        pm->reset();

        QWidget * profile = new Profile(w->userId());

        w->connect(
                        profile, SIGNAL(loadPersonInfo(int, QJsonObject, ResponseWaiter *)),
                        serv, SLOT(sendCommand(int, QJsonObject, ResponseWaiter *)));

        pm->addPage("Profile", profile, { "Accounting" });
        pm->addPage("Accounting", new PersonalAccounting);

        pm->finalize();
};

inline pagemanSetuper manager = [](Burury *w, PagesManager * pm, Service *serv) {
        pm->reset();

        QWidget * profile = new Profile(w->userId());

        w->connect(
                        profile, SIGNAL(loadPersonInfo(int, QJsonObject, ResponseWaiter *)),
                        serv, SLOT(sendCommand(int, QJsonObject, ResponseWaiter *)));

        pm->addPage("Profile", profile, { "Accounting" });
        pm->addPage("Accounting", new PersonalAccounting);

        pm->finalize();
};

inline QMap<int, pagemanSetuper> pagerPresets{
        { MAIN_PAGE_ID,             mainPage },
        { (int)API::Roles::Admin,   admin },
        { (int)API::Roles::Worker,  worker },
        { (int)API::Roles::Manager, manager },
};

#endif /* end of include guard: PAGERPRESETS_HPP_CCDGDJA4 */
