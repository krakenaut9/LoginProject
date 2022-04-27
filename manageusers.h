#ifndef MANAGEUSERS_H
#define MANAGEUSERS_H
#include <pch.h>
#include <QRegExp>
#include <QtMath>


static QString s_questionsArray[QUESTIONS_COUNT] = {
    "What was your childhood nickname?",
    "What was the name of your childhood best friend?",
    "What city was your first job in?",
    "What was the name of your school?",
    "What was the name of the company you first worked for?",
    "What was the name of your favorite teacher?",
    "What was your house number as a child?",
    "What is your mother's middle name?",
    "What was the name of the school you attended for the first time?",
    "What is your animal's name?",
    "What year was your father born?",
    "What is your mother's maiden name?",
    "What is your favorite animal?",
    "What is your favorite sport?",
    "What is the name of your hometown?",
    "What color are your father's eyes?",
    "What color are your mother's eyes?",
    "What was the name of your first animal?",
    "What is your favorite color?",
    "What school did you attend in sixth grade?"
};

namespace ManageUsers {
    bool changeProperty(const QString& user, const QString& key, const QJsonValue& newValue);
    bool isPasswordValid(const QString& password);
    bool addUser(const QString& userName, QJsonObject userProperties);
    bool deleteUser(const QString& userName);
    QFile::FileError initUsersFile(const QString& fileName);
    QFile::FileError initQuestionsFile(const QString fileName);
    QString encryptPassword(const QString& password, const quint64 randNum);
    bool addAnswers(const QString& userName, const QVector<QString> answers);
}

#endif // MANAGEUSERS_H
