// #pragma once
//
// #include "API/CommandInput.hpp"
// #include <QString>
//
// namespace API {
//
// class AddJobInput : public CommandInput {
// public:
//         AddJobInput();
//         ~AddJobInput();
//
//         QString title;
//         QString iconBase64;
//         QString description;
//         QString instruction;
//         int minimumGradeValue;
//         QString requiredCheckAttachments; // stringified JSON TODO add class for it
//         int autoCheckScript_id;
//         double tokenPrice;
//         qint64 tokenDeadlineHours;
//         qint64 experationDate;
//         int preRunScript_id;
//         QString autoCreationTokenSource;
//         int autoCreationTokenSourceType;
//         int suspend; // boolean
// };
//
// class SuspendJobInput : public CommandInput {
// public:
//         SuspendJobInput();
//         ~SuspendJobInput();
//
//         int job_id;
// };
//
// class RemoveJobInput : public CommandInput {
// public:
//         RemoveJobInput();
//         ~RemoveJobInput();
//
//         int job_id;
// };
//
// class EditJobInput : public CommandInput {
// public:
//         EditJobInput();
//         ~EditJobInput();
//
//         QString title;
//         QString iconBase64;
//         QString description;
//         QString instruction;
//         int minimumGradeValue;
//         QString requiredCheckAttachments; // stringified JSON TODO add class for it
//         int autoCheckScript_id;
//         double tokenPrice;
//         qint64 tokenDeadlineHours;
//         qint64 experationDate;
//         int preRunScript_id;
//         QString autoCreationTokenSource;
//         int autoCreationTokenSourceType;
//         int suspend; // boolean
// };
//
// class GetJobInfoInput : public CommandInput {
// public:
//         GetJobInfoInput();
//         ~GetJobInfoInput();
//
//         int job_id;
// };
//
// CmdError exec_AddJob(const AddJobInput&);
//         // add new job
// CmdError exec_SuspendJob(const SuspendJobInput&);
//         // suspend job and all job tokens
// CmdError exec_RemoveJob(const RemoveJobInput&);
//         // remove job and remove all job tokens
// CmdError exec_EditJob(const EditJobInput&);
//         // edit job info
// CmdError exec_GetJobInfo(const GetJobInfoInput&);
//         // get job info
//         
// }
