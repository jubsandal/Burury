// #pragma once
//
// #include "API/Commands/baseInclude.hpp"
//
// namespace API {
//
// class SendMoneyInput : public CommandInput {
// public:
//         SendMoneyInput();
//         ~SendMoneyInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         int user_id;
//         double amount;
//         double currency;
// };
//
// // TODO create messaging system
// class SendMessageInput : public CommandInput {
// public:
//         SendMessageInput();
//         ~SendMessageInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         int user_id;
//         QString msg;
//         int urgency;
// };
//
// CmdError exec_SendMoney(const SendMoneyInput&);
//         // send money to worker, crypto/real world money
// CmdError exec_SendMessage(const SendMessageInput&);
//         // send message to user
//         
// }
