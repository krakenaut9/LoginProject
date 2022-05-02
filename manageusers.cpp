#include "manageusers.h"

static std::string engAlphabet = " abcdefghijklmnopqrstuvwxyz";

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
        qDebug() << "Change property : Change property error : didn't find user "<<user;
        return false;
    }
    //Set new value
    PLOGI << "Change property : " << user << " changes " << key;

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
    QRegExp re("*[0-9]*");
    re.setPatternSyntax(QRegExp::Wildcard);
    if(re.exactMatch(password) == false)
    {
        qDebug() << "Password doesn't contain numbers";
        return false;
    }

    re.setPattern("*[-+*/=%()^:]*");

    if(re.exactMatch(password) == false)
    {
        qDebug() << "No such chars : +-*/=%()^:";
        return false;
    }
    return true;
}

bool ManageUsers::addUser(const QString& userName, QJsonObject userProperties)
{
    QString newUserName = userName.toLower();
    QFile file(USERS_FILE);
    file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();
    QJsonObject RootObject = JsonDocument.object();
    if(userName.isEmpty())
    {
        qDebug() << "Empty user name";
        QMessageBox::warning(nullptr, "Incorrect name", "Empty user name");
        return false;
    }
    if(RootObject.find(userName) != RootObject.constEnd())
    {
        qDebug() << "This user already exists";
        QMessageBox::warning(nullptr, "User exists", "This user already exists");
        return false;
    }
    RootObject.insert(newUserName, userProperties);
    JsonDocument.setObject(RootObject); // set to json document
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(JsonDocument.toJson());
    file.close();
    return true;
}

bool ManageUsers::deleteUser(const QString& userName)
{
    QFile file(USERS_FILE);
    file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();
    QJsonObject RootObject = JsonDocument.object();

    auto it = RootObject.find(userName);
    if(it == RootObject.end())
    {
        qDebug() << "Didn't find user " << userName << " to delete";
        return false;
    }
    RootObject.erase(it);

    JsonDocument.setObject(RootObject); // set to json document
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(JsonDocument.toJson());
    file.close();
    return true;
}

QFile::FileError ManageUsers::initUsersFile(const QString& fileName)
{
    if(!QFile(fileName).exists())
    {
        QFile usersFile(fileName);
        if(!usersFile.open(QIODevice::NewOnly | QIODevice::ReadWrite | QIODevice::Text))
        {
            PLOG_ERROR<<"New users file creation failed";
            qDebug()<<"New usersfile creation failed";
            return QFile::FatalError;
        }
        QJsonObject mainObject;
        QJsonObject adminUser;
        adminUser.insert(PASSWORD, "");
        adminUser.insert(IS_BLOCKED, false);
        adminUser.insert(RESTRICTED_PASSWORD, false);
        adminUser.insert(FIRST_LOGIN, true);
        adminUser.insert(ACCESS_LEVEL, ACCESS_LEVEL_ADMIN);
        adminUser.insert(ACTIVATED, false);
        mainObject.insert(ADMIN, adminUser);
        QJsonDocument jsonDoc(mainObject);
        qDebug() << jsonDoc.toJson(QJsonDocument::Indented);
        usersFile.write(jsonDoc.toJson(QJsonDocument::Indented));
        usersFile.close();
    }
    return QFile::NoError;
}

QFile::FileError ManageUsers::initQuestionsFile(const QString fileName)
{
    if(!QFile(fileName).exists())
    {
        QFile usersFile(fileName);
        if(!usersFile.open(QIODevice::NewOnly | QIODevice::ReadWrite | QIODevice::Text))
        {
            PLOG_ERROR<<"New questions file creation failed";
            qDebug()<<"New questions file creation failed";
            return QFile::FatalError;
        }
        QJsonObject mainObject;

        QJsonArray questionsArray;
        for(auto& question : s_questionsArray)
        {
            questionsArray.push_back(question);
        }
        mainObject.insert("Questions", questionsArray);
        QJsonDocument jsonDoc(mainObject);
        qDebug() << jsonDoc.toJson(QJsonDocument::Indented);
        usersFile.write(jsonDoc.toJson(QJsonDocument::Indented));
        usersFile.close();
    }
    return QFile::NoError;
}

QString ManageUsers::encryptPassword(const QString& password, const quint64 randNum)
{
    return QString::number(randNum / qSin(qHash(password)));
}

bool ManageUsers::addAnswers(const QString& userName, const QVector<QString> answers)
{
    QFile file(USERS_FILE);
    file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();
    QJsonObject RootObject = JsonDocument.object();
    if(userName.isEmpty())
    {
        qDebug() << "Empty user name";
        QMessageBox::warning(nullptr, "Incorrect name", "Empty user name");
        return false;
    }
    auto userIt = RootObject.find(userName);
    QJsonArray answersArray;
    for(auto& answer : answers)
    {
        answersArray.push_back(answer);
    }
    QJsonValueRef valueRef = userIt.value();
    QJsonObject userObj = valueRef.toObject();
    userObj[ANSWERS] = answersArray;
    valueRef = userObj;
    JsonDocument.setObject(RootObject); // set to json document
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    qDebug() << "Change property writes : "<< JsonDocument.toJson();
    file.write(JsonDocument.toJson());
    file.close();
    return true;
}

bool ManageUsers::areAnswersCorrect(const QString& userName, const QVector<QString> answers, const QVector<int> questions)
{
    QFile file(USERS_FILE);
    file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();
    QJsonObject RootObject = JsonDocument.object();
    if(userName.isEmpty())
    {
        qDebug() << "Empty user name";
        QMessageBox::warning(nullptr, "Incorrect name", "Empty user name");
        return false;
    }
    auto userIt = RootObject.find(userName);
    auto answersArray = userIt.value().toObject()[ANSWERS].toArray();

    for(size_t i  = 0; i < answers.size(); ++i)
    {
        if(answers[i] != answersArray[questions[i]].toString())
        {
            PLOGW    << "Incorrect " << i+1<<" answer to " << questions[i] << " question : " << s_questionsArray[questions[i]];
            qDebug() << "Incorrect " << i+1<<" answer to " << questions[i] << " question : " << s_questionsArray[questions[i]];
            qDebug() << answers[i] << " != " <<answersArray[questions[i]].toString();
            return false;
        }
    }
    return true;
}

QString ManageUsers::caesarCipher(std::string src, qint8 key)
{
    for(auto& ch : src)
    {
        if(QChar(ch).isLetter() || QChar(ch).isSpace())
        {
            auto index = engAlphabet.find(tolower(ch));
            ch = engAlphabet[(index + key) % 27];
        }
    }
    return QString::fromStdString(src);
}
