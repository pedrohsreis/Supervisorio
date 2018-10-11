#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>

#define LEVEL_INFO 1
#define LEVEL_DEBUG 2
#define LEVEL_WARNING 3
#define LEVEL_ERROR 4

class Message
{
    private:
        long id;
        int level;
        QDateTime dateTime;
        QString message;
        void initId();
        void initDateTime();
        void initMessage();
        void initLevel();

    public:
        Message();
        Message(long id);
        Message(QString &message);
        Message(long id, QString &message);
        Message(QDateTime &dateTime, QString &message);
        Message(long id, QDateTime &dateTime, QString &message);
        virtual ~Message();
        int getLevel();
        void setLevel(int level);
        void setDateTime(QDateTime& dateTime);
        QDateTime &getDateTime();
        void setMessage(QString &message);
        QString &getMessage();

        virtual QString toString();

};

#endif // MESSAGE_H
