#ifndef CREATETABLES_HPP_Y3XIBLZH
#define CREATETABLES_HPP_Y3XIBLZH

static const auto CREATE_ACCIDENTS_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "Accidents" (
        "id" INTEGER NOT NULL UNIQUE,
        "name" TEXT NOT NULL,
        "contract_id" INTEGER NOT NULL,
        "usedAmmoCount" INTEGER,
        "damagePrice" DECIMAL(10, 3),
        "assignedEmployees_id" INTEGER,
    FOREIGN KEY("contract_id") REFERENCES "Contracts"("id") ON DELETE RESTRICT,
    FOREIGN KEY("assignedEmployees_id") REFERENCES "AssignedEmployees"("id") ON DELETE RESTRICT,
    PRIMARY KEY("id") ))";

static const auto CREATE_ACCOUNTING_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "Accounting" (
        "id" INTEGER NOT NULL UNIQUE,
        "accountingType_id" INTEGER NOT NULL,
        "pay" DECIMAL(10, 3) NOT NULL,
        "date" TEXT NOT NULL,
        "user_id" INTEGER NOT NULL,
    FOREIGN KEY("accountingType_id") REFERENCES "AccountingType"("id") ON DELETE RESTRICT,
    FOREIGN KEY("user_id") REFERENCES "Users"("id") ON DELETE RESTRICT,
    PRIMARY KEY("id") ))";

static const auto CREATE_ACCOUNTING_TYPE_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "AccountingType" (
        "id" INTEGER NOT NULL UNIQUE,
        "name" TEXT NOT NULL UNIQUE,
    PRIMARY KEY("id","name") ))";

static const auto CREATE_ASSIGNED_EMPLOYEES_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "AssignedEmployees" (
        "id" INTEGER NOT NULL,
        "employee_id" INTEGER NOT NULL,
        "guiltyPercent" DECIMAL(10, 3) NOT NULL,
        "usedAmmo" INTEGER,
    PRIMARY KEY("id") ))";

static const auto CREATE_CONTRACTS_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "Contracts" (
        "id" INTEGER NOT NULL UNIQUE,
        "assignedEmployees_id" INTEGER NOT NULL,
        "customer_id" INTEGER NOT NULL,
        "objectType_id" INTEGER NOT NULL,
        "objectAddress" TEXT NOT NULL,
        "objectWayPoint" TEXT,
        "date" INTEGER NOT NULL,
        "expirationDate" INTEGER NOT NULL,
        "weekends" INTEGER NOT NULL,
    FOREIGN KEY("customer_id") REFERENCES "Users"("id") ON DELETE RESTRICT,
    PRIMARY KEY("id"),
    FOREIGN KEY("assignedEmployees_id") REFERENCES "AssignedEmployees"("employee_id") ON DELETE RESTRICT,
    FOREIGN KEY("objectType_id") REFERENCES "objectType"("id") ON DELETE RESTRICT ))";

static const auto CREATE_DUTY_SCHEDULE_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "DutySchedule" (
        "emploee_id" INTEGER NOT NULL,
        "day" INTEGER NOT NULL,
    FOREIGN KEY("emploee_id") REFERENCES "Users"("id") ON DELETE RESTRICT ))";

static const auto CREATE_USERS_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "Users" (
        "id" INTEGER NOT NULL UNIQUE,
        "name" TEXT NOT NULL,
        "entryDate" TEXT NOT NULL,
        "role_id" INTEGER NOT NULL,
        "wapon_id" INTEGER,
        "email" TEXT UNIQUE,
        "login" TEXT NOT NULL UNIQUE,
        "password" BLOB NOT NULL UNIQUE,
        "salt" BLOB NOT NULL,
        "image" BLOB,
    FOREIGN KEY("wapon_id") REFERENCES "Wapons"("id") ON DELETE RESTRICT,
    FOREIGN KEY("role_id") REFERENCES "Roles"("id") ON DELETE RESTRICT,
    PRIMARY KEY("id" AUTOINCREMENT) ))";

static const auto CREATE_ROLES_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "Roles" (
        "id" INTEGER NOT NULL UNIQUE,
        "name" TEXT NOT NULL UNIQUE,
        "commands_id" INTEGER NOT NULL,
        "payMultipler" DECIMAL(10, 3) NOT NULL,
        "payPeriod" INTEGER NOT NULL,
    FOREIGN KEY("commands_id") REFERENCES "roleCommands"("role_id") ON DELETE RESTRICT,
    PRIMARY KEY("id") ))";

static const auto CREATE_WAPONS_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "Wapons" (
        "id" INTEGER NOT NULL UNIQUE,
        "employee_id" INTEGER UNIQUE,
        "name" TEXT NOT NULL,
        "ammo" INTEGER NOT NULL,
        "price" DECIMAL(10, 3) NOT NULL,
        "ammoPrice" DECIMAL(10, 3) NOT NULL,
        "image" BLOB,
    FOREIGN KEY("employee_id") REFERENCES "Users"("id") ON DELETE RESTRICT,
    PRIMARY KEY("id") ))";

static const auto CREATE_OBJECT_TYPE_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "objectType" (
        "id" INTEGER NOT NULL UNIQUE,
        "name" TEXT NOT NULL UNIQUE,
        "price" DECIMAL(10, 3) NOT NULL,
    PRIMARY KEY("id") )
    )";

static const auto CREATE_ROLE_COMMANDS_TABLE =
R"( CREATE TABLE IF NOT EXISTS
    "roleCommands" (
        "role_id" INTEGER NOT NULL,
        "command_id" INTEGER NOT NULL,
    FOREIGN KEY("role_id") REFERENCES "Roles"("id") ON DELETE RESTRICT ))";

#endif /* end of include guard: CREATETABLES_HPP_Y3XIBLZH */
