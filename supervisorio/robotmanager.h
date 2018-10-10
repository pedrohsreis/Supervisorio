#ifndef ROBOTMANAGER_H
#define ROBOTMANAGER_H

#include <QList>
#include <QTreeWidget>
#include <QThread>
#include "robot.h"

class RobotManagerWorker : public QObject
{
    Q_OBJECT

    private:
        QList<Robot> *robots;
    public:
        RobotManagerWorker(QList<Robot> *robots);

    public slots:
        void doWork();

    signals:
        void resultReady();
};

class RobotManager : public QObject
{
    Q_OBJECT
    private:
        QList<Robot> robots;
        QThread thread;
        bool running;
        QTreeWidget *listWidget;
    public:
        RobotManager();
        ~RobotManager();

        bool isRunning();

        void setWidget(QTreeWidget *listWidget);
        void startSearch();
        void stopSearch();

    public slots:
        void handleResults();
    signals:
        void operate();
};

#endif // ROBOTMANAGER_H
