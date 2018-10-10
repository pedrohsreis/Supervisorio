#include "robotmanager.h"
#include <QDebug>

RobotManagerWorker::RobotManagerWorker(QList<Robot> *robots)
{
    this->robots = robots;
}

void RobotManagerWorker::doWork()
{
    while(true)
    {
        QThread::sleep(1000);
        qDebug() << "Work";
        emit resultReady();
    }
}

RobotManager::RobotManager()
{
    RobotManagerWorker *worker = new RobotManagerWorker(&robots);
    worker->moveToThread(&thread);
    connect(&thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &RobotManager::operate, worker, &RobotManagerWorker::doWork);
    connect(worker, &RobotManagerWorker::resultReady, this, &RobotManager::handleResults);
    listWidget = nullptr;
}

RobotManager::~RobotManager()
{
    if(isRunning())
    {
        thread.quit();
        thread.wait();
    }
}

bool RobotManager::isRunning()
{
    return thread.isRunning();
}

void RobotManager::setWidget(QTreeWidget *listWidget)
{
    this->listWidget = listWidget;
}

void RobotManager::startSearch()
{
    if(!isRunning())
    {
        thread.start();
    }
}

void RobotManager::stopSearch()
{
    if(isRunning())
    {
        thread.quit();
    }
}

void RobotManager::handleResults()
{
    if(listWidget != nullptr)
    {
        qDebug() << "Add";
    }
}
