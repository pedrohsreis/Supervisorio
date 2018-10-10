#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>

class Message
{
    private:
        long id;
        QDateTime dateTime;
        QString message;
        void initId();
        void initDateTime();
        void initMessage();

    public:
        Message();
        Message(long id);
        Message(QString &message);
        Message(long id, QString &message);
        Message(QDateTime &dateTime, QString &message);
        Message(long id, QDateTime &dateTime, QString &message);
        virtual ~Message();
        void setDateTime(QDateTime& dateTime);
        QDateTime &getDateTime();
        void setMessage(QString &message);
        QString &getMessage();

        virtual QString toString();

};

#endif // MESSAGE_H
