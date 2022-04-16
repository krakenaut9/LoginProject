#ifndef MANAGEUSERS_H
#define MANAGEUSERS_H
#include <pch.h>
#include <QRegExp>

namespace ManageUsers {
    bool changeProperty(const QString& user, const QString& key, const QJsonValue& newValue);
    bool isPasswordValid(const QString& password);
    bool addUser(const QString& userName, QJsonObject);
}

#endif // MANAGEUSERS_H
