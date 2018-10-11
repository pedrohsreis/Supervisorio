#ifndef LOGGER_H
#define LOGGER_H

#include <QTextEdit>
#include "message.h"

class Logger
{
    private:
        static QTextEdit *logWidget;
        static void output(QString &text);
    public:
        static void setLogWidget(QTextEdit *logWidget);
        static void log(Message *message);
        static void log(QString text, int level = LEVEL_INFO);

};

#endif // LOGGER_H
