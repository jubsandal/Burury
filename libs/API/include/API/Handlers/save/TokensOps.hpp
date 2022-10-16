// #pragma once
//
// #include "API/Commands/baseInclude.hpp"
//
// namespace API {
//
// class AddJobTokensInput : public CommandInput {
// public:
//         AddJobTokensInput();
//         ~AddJobTokensInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         QString query;
//
//         int job_id;
// };
//
// class AttachJobTokensInput : public CommandInput {
// public:
//         AttachJobTokensInput();
//         ~AttachJobTokensInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         QVector<int> tokens;
//         int user_id;
// };
//
// class SetTokenDeadlineInput : public CommandInput {
// public:
//         SetTokenDeadlineInput();
//         ~SetTokenDeadlineInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         QVector<int> tokens;
//         int hours;
// };
//
// class UnattachJobTokensInput : public CommandInput {
// public:
//         UnattachJobTokensInput();
//         ~UnattachJobTokensInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         QVector<int> tokens;
// };
//
// class SuspendJobTokenInput : public CommandInput {
// public:
//         SuspendJobTokenInput();
//         ~SuspendJobTokenInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         QVector<int> tokens;
// };
//
// class UnsuspendJobTokenInput : public CommandInput {
// public:
//         UnsuspendJobTokenInput();
//         ~UnsuspendJobTokenInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         QVector<int> tokens;
// };
//
// class RequestTokensInput : public CommandInput {
// public:
//         RequestTokensInput();
//         ~RequestTokensInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         int job_id;
//         int count;
// };
//
// class RefuseTokensInput : public CommandInput {
// public:
//         RefuseTokensInput();
//         ~RefuseTokensInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         QVector<int> tokens;
// };
//
// class SendTokenForCheckInput : public CommandInput {
// public:
//         SendTokenForCheckInput();
//         ~SendTokenForCheckInput();
//
//         virtual bool extract() override;
//         virtual void load(const QJsonObject&) override;
//
//         virtual bool ready() const override;
//
//         int token_id;
//         QString attachments;
// };
//
// CmdError exec_AddJobTokens(const AddJobTokensInput&);
//         // and tokens to job tokens pool
// CmdError exec_AttachJobTokens(const AttachJobTokensInput&); // ---- AUTO only ---- //
//         // attach job tokens to worker with sertain deadline to each incremently
// CmdError exec_SetTokenDeadline(const SetTokenDeadlineInput&);
//         // edit token deadline, only for attached tokens
// CmdError exec_UnattachJobTokens(const UnattachJobTokensInput&);
//         // unattach job tokens list from worker
// CmdError exec_SuspendJobToken(const SuspendJobTokenInput&);
//         // put job token to suspend state, token will be removed from worker
// CmdError exec_UnsuspendJobToken(const UnsuspendJobTokenInput&);
//         // remove job token from suspend state and can be used by worker
// CmdError exec_RequestTokens(const RequestTokensInput&);
//         // request tokens
// CmdError exec_RefuseTokens(const RefuseTokensInput&);
//         // refuse tokens, and return it to job pool
// CmdError exec_SendTokenForCheck(const SendTokenForCheckInput&);
//         // send attachments to check and this starts automatic check procedure 
//         
// }
