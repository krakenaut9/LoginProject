#ifndef MANAGEUSERS_H
#define MANAGEUSERS_H
#include <pch.h>
#include <QRegExp>
#include <QtMath>

namespace ManageUsers {
    bool changeProperty(const QString& user, const QString& key, const QJsonValue& newValue);
    bool isPasswordValid(const QString& password);
    bool addUser(const QString& userName, QJsonObject userProperties);
    bool deleteUser(const QString& userName);
    QString encryptPassword(const QString& password, const quint64 randNum);
}

#endif // MANAGEUSERS_H
