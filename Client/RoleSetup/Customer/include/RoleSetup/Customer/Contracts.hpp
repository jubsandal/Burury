#ifndef CONTRACTS_HPP
#define CONTRACTS_HPP

#include <QFrame>
#include <QJsonObject>
#include "Client/Service/Service.hpp"

namespace Ui {
    class Contracts;
}

class Contracts : public QFrame
{
    Q_OBJECT

    public:
        explicit Contracts(int id, QWidget *parent = nullptr);
        ~Contracts();

    Q_SIGNALS:
        void loadMyContracts(QJsonObject, ResponseWaiter *);
        void loadFreeEmployees(QJsonObject, ResponseWaiter *);
        void loadRolesDetails(QJsonObject, ResponseWaiter *);
        void loadObjectTypes(QJsonObject, ResponseWaiter *);
        void createNewContract(QJsonObject, ResponseWaiter *);

    public Q_SLOTS:
        void myContractsLoaded(QJsonObject);
        void freeEmployeesLoaded(QJsonObject);
        void rolesLoaded(QJsonObject);
        void objectTypesLoaded(QJsonObject);

    private Q_SLOTS:
        void onInputChaned();

        void onEmployeeAdd();

    private:
        Ui::Contracts *ui;

        struct objT {
            int id;
            QString name;
            double price;
        };

        struct empl {
            int id;
            int role;
            QString name;
            double priceMult;
            int period;
        };

        int _id;

        QVector<int> _selectedEmpl;

        QVector<empl> _freeEmpl;
        QVector<objT> _objT;

        ResponseWaiter * _emplsWaiter;
        ResponseWaiter * _rolesWaiter;
        ResponseWaiter * _contractsWaiter;
        ResponseWaiter * _objTypeWaiter;
        ResponseWaiter * _createWaiter;
};

#endif // CONTRACTS_HPP
