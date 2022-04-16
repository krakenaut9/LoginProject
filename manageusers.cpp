#include "manageusers.h"

bool ManageUsers::changeProperty(const QString& user, const QString& key, const QJsonValue& newValue)
{
    QFile file(USERS_FILE);
    file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();
    QJsonObject RootObject = JsonDocument.object();
    QJsonObject::Iterator it;
    if((it = RootObject.find(user)) == RootObject.end())
    {
        qDebug() << "Change property error : didn't find user "<<user;
        return false;
    }
    //Set new value
    qDebug()<<user<<" sets " <<key<<" to "<< newValue.toString();
    QJsonValueRef valueRef = it.value();
    QJsonObject userObj = valueRef.toObject();
    userObj[key] = newValue;
    valueRef = userObj;
    qDebug() << key <<" after changing is "<< it.value().toObject()[key];
    JsonDocument.setObject(RootObject); // set to json document
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    qDebug() << "Change property writes : "<< JsonDocument.toJson();
    file.write(JsonDocument.toJson());
    file.close();
    return true;
}

bool ManageUsers::isPasswordValid(const QString& password)
{
    if(password.contains(" "))
    {
        qDebug("Contains space");
        return false;
    }
    QRegExp re("*[1-9]*");
    re.setPatternSyntax(QRegExp::Wildcard);
    if(re.exactMatch(password) == false)
    {
        qDebug() << "Password doesn't contain numbers";
        return false;
    }

    re.setPattern("*[+-*/=%()^:]*");

    if(re.exactMatch(password) == false)
    {
        qDebug() << "No such chars : +-*/=%()^:";
        return false;
    }
    return true;
}
