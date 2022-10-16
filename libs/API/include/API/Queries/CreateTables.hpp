#ifndef CREATETABLES_HPP_Y3XIBLZH
#define CREATETABLES_HPP_Y3XIBLZH

static const auto CREATE_USER_TABLE =
R"(CREATE TABLE Users (
        id INTEGER NOT NULL UNIQUE PRIMARY KEY,
        name TEXT NOT NULL,
        role INTEGER NOT NULL,
        grade_id INTEGER REFERENCES UserGrades (id) ON DELETE RESTRICT ON UPDATE RESTRICT NOT NULL DEFAULT (0),
        entryDate TEXT NOT NULL,
        email TEXT UNIQUE,
        telegram TEXT UNIQUE,
        vk TEXT UNIQUE,
        tokensCompleted INTEGER NOT NULL DEFAULT (0),
        tokensTotal INTEGER NOT NULL DEFAULT (0),
        login TEXT NOT NULL UNIQUE,
        password BLOB NOT NULL UNIQUE,
        salt BLOB NOT NULL,
        image BLOB,
        bannedDue INTEGER NOT NULL DEFAULT (0)
);)";

static const auto CREATE_SCRIPTS_TABLE =
R"(CREATE TABLE Scripts (
        id INTEGER NOT NULL UNIQUE,
        type INTEGER NOT NULL,
        link TEXT NOT NULL,
        PRIMARY KEY (id)
);)";

static const auto CREATE_TOKENS_TABLE =
R"(CREATE TABLE Tokens (
        id TEXT NOT NULL UNIQUE,
        job_id INTEGER NOT NULL,
        attachedUser_id REFERENCES Users (id) ON DELETE RESTRICT ON UPDATE RESTRICT,
        doneByWorker_id INTEGER REFERENCES Users (id) ON DELETE RESTRICT ON UPDATE RESTRICT,
        releaseDate INTEGER NOT NULL,
        completeDate INTEGER,
        isOnCheck INTEGER NOT NULL DEFAULT (0),
        suspended INTEGER NOT NULL DEFAULT (0),
        firstWorkerStartDate INTEGER,
        lastWorkerStartDate INTEGER,
        attachments BLOB NOT NULL DEFAULT ('{"version": 1}'),
        paid INTEGER NOT NULL DEFAULT (0),
        PRIMARY KEY (id)
);)";

static const auto CREATE_JOB_TABLE =
R"(CREATE TABLE Job (
        id INTEGER NOT NULL UNIQUE,
        title TEXT NOT NULL,
        icon BLOB,
        description TEXT NOT NULL,
        instruction TEXT NOT NULL,
        minimumGradeValue INTEGER NOT NULL DEFAULT (0),
        requireCheckAttachments TEXT NOT NULL DEFAULT ('{"version": 1}'),
        autoCheckScript_id INTEGER NOT NULL REFERENCES Scripts (id) ON DELETE RESTRICT ON UPDATE RESTRICT DEFAULT (-1),
        tokenPrice REAL NOT NULL,
        tokenDeadlineHours INTEGER NOT NULL,
        experationDate INTEGER NOT NULL DEFAULT (-1),
        preRunScript_id INTEGER NOT NULL REFERENCES Scripts (id) ON DELETE RESTRICT ON UPDATE RESTRICT DEFAULT (-1),
        autoCreationTokenSource TEXT,
        autoCreationTokenSourceType INTEGER,
        suspended INTEGER NOT NULL DEFAULT (0)
);)";

static const auto CREATE_USER_GRADES_TABLE =
R"(CREATE TABLE UserGrades (
        id INTEGER PRIMARY KEY UNIQUE NOT NULL,
        title TEXT NOT NULL,
        description TEXT NOT NULL DEFAULT ('Common grade'),
        value INTEGER NOT NULL UNIQUE
);)";

#endif /* end of include guard: CREATETABLES_HPP_Y3XIBLZH */
