#ifndef MANAGEUSERS_H
#define MANAGEUSERS_H
#include <pch.h>
namespace ManageUsers {
    bool changeProperty(const QString& user, const QString& key, const QJsonValue& newValue);
}

#endif // MANAGEUSERS_H
