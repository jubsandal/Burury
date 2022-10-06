#include "API/command.hpp"
#include "API/CmdError.hpp"
#include "API/Utility.hpp"
#include "Crypto/Hash.hpp"

#include <QLatin1String>
#include <QtSql>
#include <ctype.h>

#include <QElapsedTimer>
// onlye for Debug

#include <exception>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <qnamespace.h>

// move to another pack
QPixmap QPixmapFromQString(const QString& str)
{
        return QPixmap::fromImage(QImageFromQString(str));
}

QIcon QIconFromQString(const QString& str)
{
        return QIcon(QPixmapFromQString(str));
}

QImage QImageFromQString(const QString& str)
{
        QByteArray by = QByteArray::fromBase64(str.toLatin1());
        QImage img = QImage::fromData(by, "JPEG");
        return img;
}

QString QStringFromQImage(const QImage& img)
{
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        img.save(&buffer, "JPEG");
        QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());
        return iconBase64;
}

namespace API {

        static bool conntainJoinRequest(const QString& whereReq)
        {
                if (whereReq.contains(" JOIN ", Qt::CaseSensitivity::CaseInsensitive)) {
                        return true;
                }
                return false;
        }

        static QString extractImg(QVariant const& v) {
                return QString::fromLatin1(v.toByteArray().toBase64());
        }

        /*
         * query: string
         */
        CmdError exec_create_table(QJsonObject& obj)
        {
                QString query_s = obj.take("query").toString();

                if (!query_s.length()) {
                        qDebug() << "NO QUERY";
                        return CmdError(InvalidParam, "No query passed");
                }

                QSqlQuery q;
                if (!q.exec(query_s)) {
                        qDebug() << "CMD: " << q.lastError();
                        return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                }

                return CmdError();
        }

        /*
         * login: string
         * password: string
         */
        CmdError exec_identify(QJsonObject& obj)
        {
                QSqlQuery q;
                QString login;
                QString password;

                login = obj.take("login").toString();
                password = obj.take("password").toString();

                if (!login.length() || !password.length()) {
                        obj = QJsonObject{ {"identified", false} };
                        return CmdError(InvalidParam, "Passed empty parameters");
                }

                q.prepare("SELECT id, name, role_id, password, salt FROM Users "
                                "WHERE login = :login");
                q.bindValue(":login", login);

                if (!q.exec()) {
                        return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                }
                if (!q.next()) {
                        return CmdError(AccessDenied, "No user registreted with login: " + login);
                }

                QByteArray salt = q.record().value("salt").toByteArray();
                QByteArray real_passwordHash = q.record().value("password").toByteArray();
                QByteArray passed_passwordHash = encryptPassword(password.toLatin1(), salt);

                if (real_passwordHash != passed_passwordHash) {
                        return CmdError(AccessDenied, "Invalid password");
                }

                obj["role"] = q.record().value("role_id").toInt();
                obj["name"] = q.record().value("name").toString();
                obj["id"] = q.record().value("id").toString();

                return CmdError();
        }

        /*
         * employees: Array<int>,
         * customer: int,
         * objectType: int,
         * address: string,
         * wayPoint: string,
         * experation: int,
         * start: int,
         * weekends: Array<int>,
         */
        CmdError exec_make_contract(QJsonObject& obj)
        {
                QVector<int> employees;
                int customer;
                int objType;
                QString addr;
                QString wayPoint;
                QDate experation;
                QDate start;
                Weekends weekends;

                if (obj["employees"].isArray()) {
                        auto employeesJson = obj["employees"].toArray();
                        for (auto emp : employeesJson) {
                                if (emp.isDouble()) {
                                        employees.push_back(obj["employees"].toInt());
                                } else {
                                        return CmdError(InvalidParam, "");
                                }
                        }
                } else {
                        return CmdError(InvalidParam, "");
                }

                if (auto val = obj["customer"]; val.isDouble()) {
                        customer = val.toInt();
                } else {
                        return CmdError(InvalidParam, "");
                }

                if (auto val = obj["objectType"]; val.isDouble()) {
                        objType = val.toInt();
                } else {
                        return CmdError(InvalidParam, "");
                }

                if (auto val = obj["address"]; val.isString()) {
                        addr = val.toString();
                } else {
                        return CmdError(InvalidParam, "");
                }

                if (auto val = obj["wayPoint"]; val.isString()) {
                        wayPoint = val.toString();
                }

                if (auto val = obj["start"]; val.isDouble()) {
                        start = QDate::fromJulianDay(val.toInt());
                } else {
                        return CmdError(InvalidParam, "");
                }

                if (auto val = obj["experation"]; val.isDouble()) {
                        experation = QDate::fromJulianDay(val.toInt());
                } else {
                        return CmdError(InvalidParam, "");
                }

                QSqlQuery q;

                q.prepare("INSERT INTO AssignedEmployees (employee_id, guiltyPercent, usedAmmo) VALUES(:employee, :guilty, :ammo)");
                q.bindValue(":employee", employees[0]);
                q.bindValue(":guilty", 0);
                q.bindValue(":ammo", 0);

                if (!q.exec()) {
                        return CmdError(SQLError, q.lastError().text());
                }

                int aid = q.lastInsertId().toInt();

                for (int i = 1; i < employees.length(); ++i) {
                        q.prepare("INSERT INTO AssignedEmployees (id, employee_id, guiltyPercent, usedAmmo) VALUES(:id, :employee, :guilty, :ammo)");
                        q.bindValue(":id", aid);
                        q.bindValue(":employee", employees[0]);
                        q.bindValue(":guilty", 0);
                        q.bindValue(":ammo", 0);

                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastError().text());
                        }
                }

                q.prepare("INSERT INTO Contracts (assignedEmployees_id, customer_id, objectType_id, objectAddress, objectWayPoint, date, experationDate, weekends)"
                                " Values(:employees, :customer, :object, :address, :waypoint, :date, :experation, :weekends)");
                q.bindValue(":employees", aid);
                q.bindValue(":customer", customer);
                q.bindValue(":object", objType);
                q.bindValue(":address", addr);
                q.bindValue(":waypoint", wayPoint);
                q.bindValue(":date", start);
                q.bindValue(":experation", experation);
                q.bindValue(":weekends", "");
                if (!q.exec()) {
                        return CmdError(SQLError, q.lastError().text());
                }

                QJsonObject objT{{ "id", objType }};
                CmdError e = exec_get_object_detalils(objT);
                if (!e.Ok()) {
                        return e;
                }
                double o_price = objT["price"].toDouble();
                double pay = o_price;

                QJsonObject roles{{"id", "*"}};
                e = exec_get_role_details(roles);
                if (!e.Ok()) {
                        return e;
                }

                int days = start.daysTo(experation);

                for (auto& e : employees) {
                        QJsonObject employee{{"id", e}};
                        CmdError err = exec_get_user_info(employee);
                        if (!err.Ok()) {
                                return err;
                        }
                        int mult = 0;
                        int period = 0;
                        for (auto role : roles["roles"].toArray()) {
                                if (role.toObject()["id"].toInt() == employee["role_id"].toInt()) {
                                        mult = role.toObject()["mult"].toDouble();
                                        period = role.toObject()["payPeriod"].toInt();
                                        break;
                                }
                        }
                        pay += mult * o_price * days/period;
                }

                q.prepare("INSERT INTO Accounting (pay, date, user_id, accountingType_id) VALUES(:pay, :date, :user, :type)");
                q.bindValue(":pay", pay);
                q.bindValue(":date", QDate::currentDate().toJulianDay());
                q.bindValue(":user", customer);
                q.bindValue(":type", AccountingType::ACC_Contract);

                if (!q.exec()) {
                        // TODO remove CONTRACT !!
                        return CmdError(SQLError, q.lastError().text());
                }

                return CmdError();
        }

        /*
         * contract: int,
         */
        CmdError exec_make_duty_schedule(QJsonObject& obj)
        {
                int contractId;

                if (auto val = obj["contract"]; !val.isDouble()) {
                        return CmdError(InvalidParam, "No contract id passed");
                }

                contractId = obj.take("contract").toDouble();

                QSqlQuery q;

                q.prepare("SELECT weekends FROM Contracts WHERE id=:id");
                q.bindValue(":id", contractId);

                if (!q.exec()) {
                        return CmdError(SQLError, q.lastError().text());
                }

                return CmdError();
        }

        /*
         * name: string,
         * contract: int,
         * damagePrice: double,
         * guiltyEmployees: Array<{
         *                          employee: int,
         *                          usedAmmo: int,
         *                          guiltyPercent: double
         *                        }
         */
        CmdError exec_register_accident(QJsonObject& obj)
        {
                return CmdError();
        }

        /*
         * login and password must be passed as plaintext
         *
         * name: string,
         * entryDate: int, <- optional (default today)
         * role: int,
         * wapon: int, <- optional
         * email: string, <- optional
         * login: string,
         * password: string,
         * image: string, <- optional
         */
        CmdError exec_register_employee(QJsonObject& obj)
        {
                QString name;
                qint64 entryDate;
                int role;
                int wapon;
                QString email;
                QString login;
                QByteArray password;
                QString imagestr;
                QByteArray image;

                QJsonValue buf;

                buf = obj.take("entryDate");
                entryDate = (buf.isUndefined() ? QDateTime::currentMSecsSinceEpoch() : buf.toInteger());

                buf = obj["role"];
                role = (buf.isUndefined() ? -1 : buf.toInteger());

                buf = obj.take("wapon");
                wapon = (buf.isUndefined() ? -1 : buf.toInteger());

                name = obj["name"].toString();
                email = obj.take("email").toString();
                login = obj.take("login").toString();
                password = obj.take("password").toString().toLatin1();
                imagestr= obj.take("image").toString();
                if (imagestr.length()) {
                        image = QByteArray::fromBase64(imagestr.toLatin1());
                }

                // check values
                if (!name.length() ||
                                !login.length() ||
                                !password.length() ||
                                role < 0)
                {
                        return CmdError(InvalidParam, "Some parapeter not passed or is NULL");
                }

                // check password
                if (passwordQuality(password) == PasswordQuality::UNAVALIBLE) {
                        return CmdError(InvalidParam, "Unavalible password"); // add more informative message aha
                }

                QByteArray salt = saltGen(passWeaknesses(password));
                QByteArray passHash =
                        encryptPassword(password, salt);

                QSqlQuery q;
                q.prepare("INSERT INTO Users"
                                "(name,   entryDate,  role_id,  wapon_id,  email,  login,  password,  salt,  image) "
                                "VALUES (:name, :entryDate, :role_id, :wapon_id, :email, :login, :password, :salt, :image)");
                q.bindValue(":name", name);
                q.bindValue(":entryDate", entryDate);
                q.bindValue(":role_id", role);
                q.bindValue(":wapon_id", (wapon == -1 ? QVariant() : wapon));
                q.bindValue(":email", (email.length() > 0 ? email : QVariant()));
                q.bindValue(":login", login);
                q.bindValue(":password", passHash);
                q.bindValue(":salt", salt);
                q.bindValue(":image", (image.length() > 0 ? image: QVariant() ));

                if (!q.exec()) {
                        return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                }
                obj["id"] = q.lastInsertId().toInt();

                return CmdError();
        }

        /*
         * name: string,
         * email: string,
         * login: string,
         * password: string,
         * image: string, < optional
         */
        CmdError exec_register_customer(QJsonObject& obj)
        {
                return exec_register_employee(obj); //TODO add some checks maybe
        }

        CmdError exec_register_recrut(QJsonObject& obj)
        {
                return exec_register_employee(obj);
        }

        /*
         * name: string,
         * price: double,
         */
        CmdError exec_register_object_type(QJsonObject& obj)
        {
                QString name;
                double price;

                if (auto val = obj["name"]; val.isString()) {
                        name = val.toString();
                } else {
                        return CmdError(InvalidParam, "");
                }

                if (auto val = obj["price"]; val.isDouble()) {
                        price = val.toDouble();
                } else {
                        return CmdError(InvalidParam, "");
                }

                QSqlQuery q;
                q.prepare("INSERT INTO objectType (name, price) "
                                "VALUES (:name, :price)");
                q.bindValue(":name", name);
                q.bindValue(":price", price);
                if (!q.exec()) {
                        return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                }

                return CmdError();
        }

        /*
         * name: string,
         * ammo: int,
         * price: double,
         * ammoPrice: double,
         * image: string,
         */
        CmdError exec_register_wapon(QJsonObject& obj)
        {
                QString name;
                int ammo;
                double price;
                double ammoPrice;
                QString imagestr;
                QByteArray image;

                QJsonValue buf;

                name = obj.take("name").toString();
                buf = obj.take("ammo");
                ammo = (buf.isDouble() ? buf.toInteger() : -1);
                buf = obj.take("ammoPrice");
                ammoPrice = (buf.isDouble() ? buf.toInteger() : -1);
                imagestr = obj.take("image").toString();
                if (imagestr.length()) {
                        image = QByteArray::fromBase64(imagestr.toLatin1());
                }

                if (!name.length() ||
                                ammo < 0 ||
                                ammoPrice < 0)
                {
                        return CmdError(InvalidParam, "Empty or not exists parameter passed");
                }

                QSqlQuery q;
                q.prepare("INSERT INTO Wapons (name, price, ammo, ammoPrice, image)"
                                "VALUES (:name, :price, :ammo, :ammoPrice, :image)");
                q.bindValue(":name", name);
                q.bindValue(":price", price);
                q.bindValue(":ammo", ammo);
                q.bindValue(":ammoPrice", ammoPrice);
                q.bindValue(":image", (image.length() > 0 ? image : QVariant()));

                if (!q.exec()) {
                        return CmdError(SQLError, QString(q.lastQuery() + " " + q.lastError().text()));
                }

                return CmdError();
        }

        /*
         * wapon: int,
         * employee: int,
         */
        CmdError exec_assign_wapon(QJsonObject& obj)
        {
                if (!obj["wapon"].isDouble()) {
                        return CmdError(InvalidParam, "No wapon id passed");
                }

                if (!obj["employee"].isDouble()) {
                        return CmdError(InvalidParam, "No employee id passed");
                }

                int wapon_id = obj.take("wapon").toInt();
                int emploee_id = obj.take("employee").toInt();

                QSqlQuery q;
                q.prepare("UPDATE Wapons SET employee_id = :employee WHERE id = :wapon");
                q.bindValue(":employee", emploee_id);
                q.bindValue(":wapon", wapon_id);
                if (!q.exec()) {
                        return CmdError(SQLError, q.lastError().text());
                }

                q.prepare("UPDATE Users SET wapon_id = :wapon WHERE id = :employee");
                q.bindValue(":wapon", wapon_id);
                q.bindValue(":employee", emploee_id);
                if (!q.exec()) {
                        return CmdError(SQLError, q.lastError().text());
                }

                return CmdError();
        }

        /*
         * wapon: int,
         * count: int,
         */
        CmdError exec_pay_ammo(QJsonObject& obj)
        {
                return CmdError();
        }

        /*
         * employee: int
         */
        CmdError exec_pay_employee(QJsonObject& obj)
        {
                return CmdError();
        }

        /*
         * accident: int
         */
        CmdError exec_pay_accident(QJsonObject& obj)
        {
                return CmdError();
        }

        /*
         * id: int,
         * name: string, <- optional
         * price: double, <- optional
         */
        CmdError exec_edit_object_type(QJsonObject& obj)
        {
                return CmdError();
        }

        /*
         * role: int,
         * payMult: double, <- optional
         * payPeriod: optional, <- optional
         */
        CmdError exec_update_role(QJsonObject& obj)
        {
                return CmdError();
        }

        /*
         * id: int OR id: "*" (for all)
         *            where: string <- optional
         * takeImage: boolean <- optional (default false)
         */
        CmdError exec_get_user_info(QJsonObject& obj)
        {
                QSqlQuery q;

                int id;
                bool takeImage = obj.take("takeImage").toBool(false);
                QJsonValue buf = obj.take("id");
                if (buf.toString() == "*") // Multi select
                {
                        QJsonArray empls;
                        QString where = obj.take("where").toString();
                        q.prepare("SELECT id, name, entryDate, role_id, wapon_id, email, login" +
                                        QString(takeImage ? ", image" : "") +
                                        " FROM Users " + where);

                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }

                        while (q.next()) {
                                empls.append(QJsonObject{
                                                { "id", q.record().value("id").toInt() },
                                                { "name", q.record().value("name").toString() },
                                                { "role", q.record().value("role_id").toInt() },
                                                { "email", q.record().value("email").toString() },
                                                { "login", q.record().value("login").toString() },
                                                { "entryDate", q.record().value("entryDate").toInt() },
                                                { "wapon", q.record().value("wapon_id").toInt() },
                                                { "image", (takeImage ? extractImg(q.record().value("image")) : "") } // remove to another? use a model?
                                                });
                        }

                        obj["users"] = empls;
                }
                else if ((id = buf.toInteger(-1)) != -1) // Single selection
                {
                        q.prepare("SELECT id, name, entryDate, role_id, wapon_id, email, login" +
                                        QString(takeImage ? ", image" : "") +
                                        " FROM Users WHERE id = :id");
                        q.bindValue(":id", id);
                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }
                        q.last();
                        obj = {
                                { "id", q.record().value(0).toInt() },
                                { "name", q.record().value(1).toString() },
                                { "role", q.record().value(3).toInt() },
                                { "entryDate", q.record().value(2).toInt() },
                                { "wapon", q.record().value(4).toInt() },
                                { "email", q.record().value(5).toString() },
                                { "login", q.record().value(6).toString() },
                                { "image", (takeImage ? extractImg(q.record().value(7)) : "") } // remove to another? use a model?
                        };
                }
                else
                {
                        return CmdError(InvalidParam, "Unknown employee paramentr passed");
                }

                return CmdError();
        }

        /*
         * accident: int OR accident: "*" (for all)
         */
        CmdError exec_get_accident_details(QJsonObject& obj)
        {
                return CmdError();
        }

        /*
         * id: num OR "*"
         */
        CmdError exec_get_contract_details(QJsonObject& obj)
        {
                QSqlQuery q;

                int id;
                QJsonValue buf = obj.take("id");
                if (buf.toString() == "*") // Multi select
                {
                        QJsonArray contracts;
                        QString where = obj.take("where").toString();
                        q.prepare("SELECT *"
                                        " FROM Contracts " + where);

                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }

                        while (q.next()) {
                                contracts.append(QJsonObject{
                                                { "id", q.record().value("id").toInt() },
                                                { "assignedEmployees_id", q.record().value("assignedEmployees_id").toString() },
                                                { "customer_id", q.record().value("customer_id").toString() },
                                                { "objectType_id", q.record().value("objectType_id").toString() },
                                                { "objectAddress", q.record().value("objectAddress").toString() },
                                                { "objectWayPoint", q.record().value("objectWayPoint").toString() },
                                                { "date", q.record().value("date").toString() },
                                                { "experation", q.record().value("experationDate").toInt() },
                                                { "weekends", q.record().value("weekends").toInt() },
                                                });
                        }

                        obj["contracts"] = contracts;
                }
                else if ((id = buf.toInteger(-1)) != -1) // Single selection
                {
                        q.prepare("SELECT * "
                                        " FROM Contracts WHERE id = :id");
                        q.bindValue(":id", id);
                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }
                        q.last();
                        obj = {
                                { "id", q.record().value("id").toInt() },
                                { "name", q.record().value("name").toString() },
                                { "price", q.record().value("price").toDouble() },
                        };
                }
                else
                {
                        return CmdError(InvalidParam, "Unknown employee paramentr passed");
                }


                return CmdError();
        }

        /*
         * accounting: int OR date: int OR user: int
         */
        CmdError exec_get_accounting_entry(QJsonObject& obj)
        {
                QSqlQuery q;

                int id;
                QJsonValue buf = obj.take("id");
                if (buf.toString() == "*") // Multi select
                {
                        QJsonArray objcts;
                        QString where = obj.take("where").toString();
                        q.prepare("SELECT id, name, price"
                                        " FROM objectType " + where);

                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }

                        while (q.next()) {
                                objcts.append(QJsonObject{
                                                { "id", q.record().value("id").toInt() },
                                                { "name", q.record().value("name").toString() },
                                                { "price", q.record().value("price").toDouble() },
                                                });
                        }

                        obj["objects"] = objcts;
                }
                else if ((id = buf.toInteger(-1)) != -1) // Single selection
                {
                        q.prepare("SELECT id, name, price"
                                        " FROM objectType WHERE id = :id");
                        q.bindValue(":id", id);
                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }
                        q.last();
                        obj = {
                                { "id", q.record().value("id").toInt() },
                                { "name", q.record().value("name").toString() },
                                { "price", q.record().value("price").toDouble() },
                        };
                }
                else
                {
                        return CmdError(InvalidParam, "Unknown employee paramentr passed");
                }

                return CmdError();
        }

        /*
         * object: int OR "*"
         */
        CmdError exec_get_object_detalils(QJsonObject& obj)
        {
                QSqlQuery q;

                int id;
                QJsonValue buf = obj.take("id");
                if (buf.toString() == "*") // Multi select
                {
                        QJsonArray objcts;
                        QString where = obj.take("where").toString();
                        q.prepare("SELECT id, name, price"
                                        " FROM objectType " + where);

                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }

                        while (q.next()) {
                                objcts.append(QJsonObject{
                                                { "id", q.record().value("id").toInt() },
                                                { "name", q.record().value("name").toString() },
                                                { "price", q.record().value("price").toDouble() },
                                                });
                        }

                        obj["objects"] = objcts;
                }
                else if ((id = buf.toInteger(-1)) != -1) // Single selection
                {
                        q.prepare("SELECT id, name, price"
                                        " FROM objectType WHERE id = :id");
                        q.bindValue(":id", id);
                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }
                        q.last();
                        obj = {
                                { "id", q.record().value("id").toInt() },
                                { "name", q.record().value("name").toString() },
                                { "price", q.record().value("price").toDouble() },
                        };
                }
                else
                {
                        return CmdError(InvalidParam, "Unknown employee paramentr passed");
                }

                return CmdError();
        }

        /*
         * role: int or "*"
         */
        CmdError exec_get_role_details(QJsonObject& obj)
        {
                QSqlQuery q;

                int id;
                QJsonValue buf = obj.take("id");
                if (buf.toString() == "*") // Multi select
                {
                        QJsonArray roles;
                        QString where = obj.take("where").toString();
                        q.prepare("SELECT id, name, payMultipler, payPeriod"
                                        " FROM Roles " + where);

                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }

                        while (q.next()) {
                                roles.append(QJsonObject{
                                                { "id", q.record().value("id").toInt() },
                                                { "name", q.record().value("name").toString() },
                                                { "mult", q.record().value("payMultipler").toDouble() },
                                                { "payPeriod", q.record().value("payPeriod").toInt() },
                                                });
                        }

                        obj["roles"] = roles;
                }
                else if ((id = buf.toInteger(-1)) != -1) // Single selection
                {
                        q.prepare("SELECT id, name, payMultipler, payPeriod"
                                        " FROM Roles WHERE id = :id");
                        q.bindValue(":id", id);
                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }
                        q.last();
                        obj = {
                                { "id", q.record().value("id").toInt() },
                                { "name", q.record().value("name").toString() },
                                { "mult", q.record().value("payMultipler").toDouble() },
                                { "payPeriod", q.record().value("payPeriod").toInt() },
                        };
                }
                else
                {
                        return CmdError(InvalidParam, "Unknown employee paramentr passed");
                }
                return CmdError();
        }

        /*
         * wapon: int OR wapon: "*",
         *               where: string <- optional
         * takeImage: boolean <- optional (default false)
         */
        CmdError exec_get_wapon_details(QJsonObject& obj)
        {
                QSqlQuery q;

                bool takeImage = obj.take("takeImage").toBool(false);
                auto post = [takeImage](QSqlQuery& q) -> QJsonObject {
                        return QJsonObject{ //may currapt( add checks )
                                { "id", q.value("id").toInt() },
                                        { "name", q.value("name").toString() },
                                        { "price", q.value("price").toDouble() },
                                        { "ammoPrice", q.value("ammoPrice").toDouble() },
                                        { "image", (takeImage ? extractImg(q.value("image")) : "") }
                        };
                };

                QJsonValue buf = obj.take("wapon");
                if (buf.isString()) // Multi select
                {
                        if (buf.toString() == "*") {
                                QJsonArray qjwapons;
                                QString where = obj.take("where").toString();
                                if (conntainJoinRequest(where) && !obj["acceptJoin"].toBool(false)) {
                                        return CmdError(AccessDenied, "Only built-in logic can you JOIN sequences");
                                }
                                q.prepare("SELECT * FROM Wapons " + where);
                                if (!q.exec()) {
                                        return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                                }
                                while (q.next()) {
                                        qjwapons.append(post(q));
                                }
                                obj["wapons"] = qjwapons;
                        } else {
                                return CmdError(InvalidParam, "Unknown wapon fetch parameter");
                        }
                }
                else if (buf.isDouble()) // Single select
                {
                        int id = buf.toInteger();
                        q.prepare("SELECT * FROM Wapons WHERE id = :id");
                        q.bindValue(":id", id);
                        if (!q.exec()) {
                                return CmdError(SQLError, q.lastQuery() + " " + q.lastError().text());
                        }
                        q.last();
                        obj = post(q);
                }
                else
                {
                        return CmdError(InvalidParam, "No wapon ID passed");
                }

                return CmdError();
        }

        /*
         * user: int
         * dateStart: int <- optional
         * dateEnd: int <- optional
         */
        CmdError exec_get_duty_schedule(QJsonObject& obj)
        {
                return CmdError();
        }


} /* API */ 
