// #pragma once
//
// #include <QString>
// #include "API/CmdError.hpp"
// #include "API/CommandInput.hpp"
//
// namespace API {
//
// class CreateTableInput : public CommandInput {
// public:
//         QString query;
// };
//
// class IdentifyInput : public CommandInput {
// public:
//         QString login;
//         QString password;
// };
//
// class ValidateConfCodeInput : public CommandInput {
//         QString code;
// };
//
// CmdError exec_CreateTable(CreateTableInput*);
//         // create database table
// CmdError exec_Identify(IdentifyInput*);
//         // login identify procedure
// CmdError exec_ValidateConfCode(ValidateConfCodeInput*);
//         // send confirmation code from user to server for check
//
// } /* API */
