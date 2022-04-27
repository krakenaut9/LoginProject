#ifndef PCH_H
#define PCH_H
#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTimer>
#include <QVector>

#include <plog/Log.h>

#define USERS_FILE  "users.json"
#define PASSWORD    "Password"
#define FIRST_LOGIN "FirstLogin"
#define ADMIN       "admin"
#define RESTRICTED_PASSWORD "Restricted password"
#define IS_BLOCKED "Is blocked"
#define ACCESS_LEVEL "Access level"
#define ANSWERS "Answers"
#define TIME_INTERVAL 5 *  1000
#define QUESTIONS_FILE "questions.json"

#define ADD_NEW     "Add new"
#define EDIT        "Edit"


#define QUESTIONS_COUNT 20
#define QUESTIONS_TO_CHECK 3

#define ACCESS_LEVEL_DEFAULT    "Default"
#define ACCESS_LEVEL_ADMIN      "Admin"

#define MAX_USERS_COUNT 4
#endif // PCH_H
