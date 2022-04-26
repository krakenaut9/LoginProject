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

#include <plog/Log.h>

#define USERS_FILE  "users.json"
#define USER_NAME
#define PASSWORD    "Password"
#define FIRST_LOGIN "FirstLogin"
#define ADMIN       "admin"
#define RESTRICTED_PASSWORD "Restricted password"
#define IS_BLOCKED "Is blocked"
#define ACCESS_LEVEL "Access level"

#define ADD_NEW     "Add new"
#define EDIT        "Edit"

#define ACCESS_LEVEL_DEFAULT    "Default"
#define ACCESS_LEVEL_ADMIN      "Admin"

#define MAX_USERS_COUNT 4
#endif // PCH_H
