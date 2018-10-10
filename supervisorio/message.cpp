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

Message::Message()
{

}

Message::Message(long id)
{
    this->id = id;
}

Message::Message(QString &message)
{
    initId();
    initDateTime();
    this->message = message;
}

Message::Message(long id, QString &message)
{
    this->id = id;
    this->message = message;
    initDateTime();
}

Message::Message(QDateTime &dateTime, QString &message)
{
    initId();
    this->dateTime = dateTime;
    this->message = message;
}

Message::Message(long id, QDateTime &dateTime, QString &message)
{
    this->id = id;
    this->dateTime = dateTime;
    this->message = message;
}

Message::~Message()
{

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
