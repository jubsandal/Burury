#ifndef PAGERPRESETS_HPP_CCDGDJA4
#define PAGERPRESETS_HPP_CCDGDJA4

#define MAIN_PAGE_ID -1092837
    // try true unique

#include <functional>
#include <QMap>

#include "Client/Service/Service.hpp"

#include "PagesManager/PagesManager.hpp"
#include "Database/Role.hpp"

#include "MainPage.hpp"

#include "RoleSetup/Admin/AdminMain.hpp"
#include "RoleSetup/Admin/Accidents.hpp"
#include "RoleSetup/Admin/AccidentList.hpp"
#include "RoleSetup/Admin/ObjectTypeAdministration.hpp"
#include "RoleSetup/Admin/RegisterAccidentDialog.hpp"
#include "RoleSetup/Admin/RoleManagment.hpp"
#include "RoleSetup/Admin/UsersList.hpp"

#include "RoleSetup/Customer/CustomerMain.hpp"
#include "RoleSetup/Customer/Contracts.hpp"

#include "RoleSetup/General/Profile.hpp"
#include "RoleSetup/General/DebugConsole.hpp"
#include "RoleSetup/General/DutySchedule.hpp"
#include "RoleSetup/General/PersonalAccounting.hpp"

#include "Controller.hpp"

using pagemanSetuper = std::function<void(Controller *, PagesManager *, Service *)>;

inline pagemanSetuper mainPage = [](Controller * w, PagesManager * pm, Service *) {
    pm->reset();
    QWidget * ml = new MainPage;
    pm->addRoot("Main", ml);

    ml->connect(ml, SIGNAL(loginClicked()), w, SLOT(showLogin()));
    ml->connect(ml, SIGNAL(registerCliecked()), w, SLOT(showRegister()));

    pm->finalize();
};

inline pagemanSetuper admin = [](Controller * w, PagesManager * pm , Service * serv) {
    pm->reset();

    QWidget * profile = new Profile(w->userId());

    w->connect(
            profile, SIGNAL(loadPersonInfo(QJsonObject, ResponseWaiter *)),
            serv, SLOT(sendCommand(QJsonObject, ResponseWaiter *)));

    pm->addRoot("Admin", new AdminMain, { "Profile", "Accidents", "Object types", "PSC Roles", "Users list", "Help" });
    pm->addPage("Profile", profile, { "Accounting", "Schedule" });
    pm->addPage("Accounting", new PersonalAccounting);
    pm->addPage("Schedule", new DutySchedule);
    pm->addPage("Accidents", new AccidentList);
    pm->addPage("Object types", new ObjectTypeAdministration);
    pm->addPage("PSC Roles", new RoleManagment);
    pm->addPage("Users list", new UsersList);

    pm->finalize();
};

inline pagemanSetuper security = [](Controller *, PagesManager * , Service *) {
};

inline pagemanSetuper recruter = [](Controller *, PagesManager * , Service *) {
};

inline pagemanSetuper incosor = [](Controller *, PagesManager * , Service *) {
};

inline pagemanSetuper waponManager = [](Controller *, PagesManager * , Service *) {
};

inline pagemanSetuper customer = [](Controller *w, PagesManager * pm, Service *serv) {
    pm->reset();

    QWidget * profile = new Profile(w->userId()),
            * contracts = new Contracts(w->userId(), w);

    w->connect(
            profile, SIGNAL(loadPersonInfo(QJsonObject, ResponseWaiter *)),
            serv, SLOT(sendCommand(QJsonObject, ResponseWaiter *)));

    //

    w->connect(
            contracts, SIGNAL(loadMyContracts(QJsonObject, ResponseWaiter *)),
            serv, SLOT(sendCommand(QJsonObject, ResponseWaiter *)));
    w->connect(
            contracts, SIGNAL(loadRolesDetails(QJsonObject, ResponseWaiter *)),
            serv, SLOT(sendCommand(QJsonObject, ResponseWaiter *)));
    w->connect(
            contracts, SIGNAL(loadFreeEmployees(QJsonObject, ResponseWaiter *)),
            serv, SLOT(sendCommand(QJsonObject, ResponseWaiter *)));
    w->connect(
            contracts, SIGNAL(createNewContract(QJsonObject, ResponseWaiter *)),
            serv, SLOT(sendCommand(QJsonObject, ResponseWaiter *)));
    w->connect(
            contracts, SIGNAL(loadObjectTypes(QJsonObject, ResponseWaiter *)),
            serv, SLOT(sendCommand(QJsonObject, ResponseWaiter *)));

    pm->addRoot("Customer", new CustomerMain, { "Profile", "Accidents", "Contracts", "Help" });
    pm->addPage("Profile", profile, { "Accounting" });
    pm->addPage("Accounting", new PersonalAccounting);
    pm->addPage("Contracts", contracts);

    pm->finalize();
};

inline QMap<int, pagemanSetuper> pagerPresets{
    { MAIN_PAGE_ID,                mainPage },
    { (int)Database::ROLE_Admin,        admin },
    { (int)Database::ROLE_Security,     security },
    { (int)Database::ROLE_Recruter,     recruter },
    { (int)Database::ROLE_Inkosor,      incosor },
    { (int)Database::ROLE_WaponManager, waponManager },
    { (int)Database::ROLE_Customer,     customer },
};

#endif /* end of include guard: PAGERPRESETS_HPP_CCDGDJA4 */
