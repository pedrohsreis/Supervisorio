#include "message.h"

void Message::initId()
{
    QDateTime UTC(QDateTime::currentDateTimeUtc());
    id = UTC.toMSecsSinceEpoch();
}

void Message::initDateTime()
{
    QDateTime UTC(QDateTime::currentDateTimeUtc());
    dateTime = UTC.toLocalTime();
}

void Message::initMessage()
{
    message = "";
}

void Message::initLevel()
{
    level = LEVEL_INFO;
}

Message::Message()
{
    initId();
    initDateTime();
    initMessage();
    initLevel();
}

Message::Message(long id)
{
    this->id = id;
    initLevel();
}

Message::Message(QString &message)
{
    initId();
    initDateTime();
    this->message = message;
    initLevel();
}

Message::Message(long id, QString &message)
{
    this->id = id;
    this->message = message;
    initDateTime();
    initLevel();
}

Message::Message(QDateTime &dateTime, QString &message)
{
    initId();
    this->dateTime = dateTime;
    this->message = message;
    initLevel();
}

Message::Message(long id, QDateTime &dateTime, QString &message)
{
    this->id = id;
    this->dateTime = dateTime;
    this->message = message;
    initLevel();
}

Message::~Message()
{

}

void Message::setLevel(int level)
{
    this->level = level;
}

int Message::getLevel()
{
    return level;
}

void Message::setDateTime(QDateTime& dateTime)
{
    this->dateTime = dateTime;
}

QDateTime &Message::getDateTime()
{
    return dateTime;
}

void Message::setMessage(QString &message)
{
    this->message = message;
}

QString &Message::getMessage()
{
    return message;
}

QString Message::toString()
{
    return message;
}
