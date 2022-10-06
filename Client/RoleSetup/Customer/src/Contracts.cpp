#include "RoleSetup/Customer/Contracts.hpp"
#include "ui_Contracts.h"

#include <QFormLayout>
#include <QComboBox>
#include <QJsonArray>
#include <qnamespace.h>

Contracts::Contracts(int id, QWidget *parent) :
    QFrame(parent),
        ui(new Ui::Contracts),
        _id(id)
{
    ui->setupUi(this);

    this->ui->employee_1->addItem("-", -1);
    connect(this->ui->addEmpl, SIGNAL(clicked()), this, SLOT(onEmployeeAdd()));
    this->ui->submitContract->setEnabled(false);

    _emplsWaiter = new ResponseWaiter(this);
    _rolesWaiter = new ResponseWaiter(this);
    _objTypeWaiter = new ResponseWaiter(this);
    _createWaiter = new ResponseWaiter(this);
    _contractsWaiter = new ResponseWaiter(this);

    connect(_emplsWaiter, SIGNAL(success(QJsonObject)), this, SLOT(freeEmployeesLoaded(QJsonObject)));
    /* connect(_waiter, SIGNAL(failed(int, QString)), this, SLOT(personInfoLoadFailed(int, QString))); */

    connect(_rolesWaiter, SIGNAL(success(QJsonObject)), this, SLOT(rolesLoaded(QJsonObject)));
    /* connect(_waiter, SIGNAL(failed(int, QString)), this, SLOT(personInfoLoadFailed(int, QString))); */

    connect(_objTypeWaiter, SIGNAL(success(QJsonObject)), this, SLOT(objectTypesLoaded(QJsonObject)));
    /* connect(_waiter, SIGNAL(failed(int, QString)), this, SLOT(personInfoLoadFailed(int, QString))); */

    connect(_createWaiter, SIGNAL(success(QJsonObject)), this, SLOT(myContractsLoaded(QJsonObject)));
    /* connect(_waiter, SIGNAL(failed(int, QString)), this, SLOT(personInfoLoadFailed(int, QString))); */

    connect(this->ui->employee_1, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputChaned()));
    connect(this->ui->objectType_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputChaned()));

    this->ui->startdate->setDateTime(QDateTime::currentDateTime());
    this->ui->startdate->setMaximumDate(this->ui->experationdate->date());
    this->ui->experationdate->setDateTime(QDateTime::currentDateTime());
    this->ui->experationdate->setMinimumDate(this->ui->startdate->date());

    connect(ui->startdate, SIGNAL(dateChanged(QDate)), this, SLOT(onInputChaned()));
    connect(ui->experationdate, SIGNAL(dateChanged(QDate)), this, SLOT(onInputChaned()));

    connect(ui->update, &QPushButton::click, [this]() {
            Q_EMIT loadMyContracts(QJsonObject{
                    { "id", "*" },
                    { "where", QString("customer_id = ") + QString::number(_id) }
                    }, _contractsWaiter);
    });

    connect(ui->submitContract, &QPushButton::click, [this]() {
        QFormLayout * layout = static_cast<QFormLayout*>(this->ui->employees->layout());
        QJsonArray empls;
        for (int j = 0; j < layout->rowCount(); ++j) {
            int id = static_cast<QComboBox*>(layout->itemAt(j, QFormLayout::ItemRole::FieldRole)->widget())->currentData().toInt();
            empls.push_back(id);
        }
        Q_EMIT createNewContract(QJsonObject{
                { "command", Database::CMD_MAKE_CONTRACT },
                { "arg",
                    QJsonObject{
                        { "employees", empls },
                        { "customer", this->_id },
                        { "objectType", this->ui->objectType_cb->currentData().toInt() },
                        { "address", this->ui->address->text() },
                        { "wayPoint", this->ui->waypoint->text() },
                        { "experation", this->ui->experationdate->date().toJulianDay() },
                        { "start", this->ui->startdate->date().toJulianDay() },
                        { "weekends", 0 }
                    }
                }
                }, _createWaiter);
    });

    QTimer::singleShot(300, [this] {
        Q_EMIT loadFreeEmployees(
            QJsonObject{{"command", Database::CMD_GET_USER_INFO},
                        {"arg", QJsonObject{{"id", "*"}, {"takeImage", false}}}},
            _emplsWaiter);
        QTimer::singleShot(100, [this] {
            Q_EMIT loadObjectTypes(
                QJsonObject{{"command", Database::CMD_GET_OBJECT_DETAILS},
                            {"arg", QJsonObject{{"id", "*"}}}},
                _objTypeWaiter);
        });
    });
}

Contracts::~Contracts()
{
    //todo
    delete ui;
}

void
Contracts::myContractsLoaded(QJsonObject obj)
{
    this->ui->tableWidget->clear();
    QJsonArray arr = obj["contracts"].toArray();

    int i = 0;
    for (auto c : arr) {
        QJsonObject cur = c.toObject();
        this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QDate::fromJulianDay(cur["date"].toInteger()).toString()));
        this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QDate::fromJulianDay(cur["experation"].toInteger()).toString()));
        objT o;
        for (int i = 0; i < _objT.length(); ++i) {
            if (_objT[i].id == cur["objectType_id"].toInt())
                o = _objT[i];
        }
        this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem(o.name));
        this->ui->tableWidget->setItem(i, 3, new QTableWidgetItem(cur["assignedEmployees_id"].toInt()));
        i++;
    }
}

void
Contracts::freeEmployeesLoaded(QJsonObject obj)
{
    // do not realy free :) add check
    QJsonArray arr = obj["users"].toArray();
    _freeEmpl.clear();
    for (const auto& user : arr) {
        QJsonObject cur = user.toObject();
        empl e;
        e.id = cur["id"].toInt();
        e.name = cur["name"].toString();
        e.role = cur["role"].toInt();
        e.priceMult = -1;
        if (e.role == Database::ROLE_Inkosor || e.role == Database::ROLE_Security) {
            _freeEmpl.push_back(e);
        }
    }
    Q_EMIT loadRolesDetails(
        QJsonObject{{"command", Database::CMD_GET_ROLE_DETAILS},
                    {"arg", QJsonObject{{"id", "*"}}}},
        _rolesWaiter);
}

void
Contracts::rolesLoaded(QJsonObject obj)
{
    QJsonArray arr = obj["roles"].toArray();
    for (const auto& role : arr) {
        QJsonObject cur = role.toObject();
        for (int i = 0; i < _freeEmpl.length(); ++i) {
            if (cur["id"].toInt() == _freeEmpl[i].role) {
                _freeEmpl[i].priceMult = cur["mult"].toDouble();
                _freeEmpl[i].period = cur["payPeriod"].toInt();
            }
        }
    }

    for (auto& empl : _freeEmpl) {
        qDebug() << empl.name << empl.priceMult << empl.role;
        this->ui->employee_1->addItem(empl.name, empl.id);
    }
}

void
Contracts::objectTypesLoaded(QJsonObject obj)
{
    QJsonArray arr = obj["objects"].toArray();
    _objT.clear();
    this->ui->objectType_cb->clear();
    for (const auto& object : arr) {
        QJsonObject cur = object.toObject();
        objT o;
        o.id = cur["id"].toInt();
        o.name = cur["name"].toString();
        o.price = cur["price"].toDouble();
        _objT.push_back(o);
        this->ui->objectType_cb->addItem(o.name, o.id);
    }
}

void
Contracts::onInputChaned() {
    this->ui->startdate->setMaximumDate(this->ui->experationdate->date());
    this->ui->experationdate->setMinimumDate(this->ui->startdate->date());

    double price = 0;
    QVector<empl> assigned;

    int objId = this->ui->objectType_cb->currentData().toInt();

    if (objId == -1) {
        this->ui->submitContract->setEnabled(false);
        return;
    }

    objT o;
    for (int i = 0; i < _objT.length(); ++i) {
        if (_objT[i].id == objId)
            o = _objT[i];
    }

    int days = this->ui->startdate->date().daysTo(ui->experationdate->date());

    QFormLayout * layout = static_cast<QFormLayout*>(this->ui->employees->layout());
    for (int j = 0; j < layout->rowCount(); ++j) {
        int id = static_cast<QComboBox*>(layout->itemAt(j, QFormLayout::ItemRole::FieldRole)->widget())->currentData().toInt();
        empl e;
        for (int i = 0; i < _freeEmpl.length(); ++i) {
            if (_freeEmpl[i].id == id)
                e = _freeEmpl[i];
        }

        qDebug() << e.priceMult << o.price << days << e.period;;

        price += e.priceMult * o.price * days/e.period;
    }

    price += o.price;

    this->ui->price->setText(QString::number(price));

    if (this->ui->address->text().length()) {
        this->ui->submitContract->setEnabled(true);
    } else {
        this->ui->submitContract->setEnabled(false);
    }
}

void
Contracts::onEmployeeAdd() {
    QFormLayout * layout = static_cast<QFormLayout*>(this->ui->employees->layout());

    QComboBox * cb = new QComboBox();
    connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputChaned()));

    /* QVector<empl> empls; */
    /* int c; */
    /* for (int i = 0; i < _freeEmpl.length(); ++i) { */
    /*     c = 0; */
    /*     for (int j = 0; j < layout->rowCount(); ++j) { */
    /*         if (auto item = layout->itemAt(j, QFormLayout::ItemRole::FieldRole); */
    /*                 static_cast<QComboBox*>(item->widget())->currentData(0).toInt() != _freeEmpl[i].id) { */
    /*             c++; */
    /*         } */
    /*     } */
    /*     if (c == layout->rowCount()-1) { */
    /*         empls.push_back(_freeEmpl[i]); */
    /*     } */
    /* } */

    cb->addItem("-", -1);
    for (auto& empl : _freeEmpl) {
        cb->addItem(empl.name, empl.id);
    }

    layout->setWidget(layout->rowCount(), QFormLayout::ItemRole::LabelRole, new QLabel("Employee"));
    layout->setWidget(layout->rowCount()-1, QFormLayout::ItemRole::FieldRole, cb);
}
