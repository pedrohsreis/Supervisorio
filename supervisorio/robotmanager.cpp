#include "robotmanager.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QProcess>
#include <QHostInfo>

RobotManagerWorker::RobotManagerWorker(QList<Robot> *robots)
{
    this->robots = robots;
    this->keepRunning = true;
    this->search = false;
}

void RobotManagerWorker::run()
{
    qDebug() << "doWork";
    while(keepRunning)
    {
        QThread::sleep(1);
        if(search)
        {
            findAddress();
            clearRobots();
            emit resultReady();
        }
        qDebug() << "Work";
    }
}

void RobotManagerWorker::findAddress()
{
    QProcess process;
#ifdef Q_OS_WIN32
    process.start("arp", QStringList() << "-a");
#else
    process.start("arp", QStringList() << "-e");
#endif
    process.waitForFinished();
    QString output(process.readAllStandardOutput());
    QStringList lines = output.split("\n", QString::SplitBehavior::SkipEmptyParts);
    for (int i = 0; i < lines.size(); i++)
    {
        QString line = lines[i];
        if(line.at(0).isDigit())
        {
            QString ip = line.left(line.indexOf(' '));
            checkRobot(ip);
        }
    }
}

void RobotManagerWorker::checkRobot(QString ip)
{
    QHostInfo info = QHostInfo::fromName(ip);
    createRobot(info.hostName(), ip);
}

void RobotManagerWorker::createRobot(QString hostname, QString ip)
{
    int index = findRobotIndex(ip);
    if(index == -1)
    {
        Robot robot;
        robot.ip = ip;
        robot.hostName = hostname;
        robot.updated = true;
        robots->append(robot);
    }
    else
    {
        (*robots)[index].updated = true;
    }
}

int RobotManagerWorker::findRobotIndex(QString ip)
{
    for(int i = 0; i < robots->size(); i++)
    {
        if((*robots)[i].ip == ip)
            return i;
    }
    return -1;
}

void RobotManagerWorker::clearRobots()
{
    QList<Robot>::iterator it = robots->begin();
    while (it != robots->end())
    {
        if(!it->updated)
            it = robots->erase(it);
        else
            it++;
    }
}

bool RobotManagerWorker::isSearching()
{
    return search;
}

void RobotManagerWorker::startSearch()
{
    search = true;
}

void RobotManagerWorker::stopSearch()
{
    search = false;
}

void RobotManagerWorker::stopRun()
{
    keepRunning = false;
}

RobotManager::RobotManager()
{
    worker = new RobotManagerWorker(&robots);
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &RobotManagerWorker::resultReady, this, &RobotManager::handleResults);
    worker->start();
    listWidget = nullptr;
}

RobotManager::~RobotManager()
{
    worker->stopSearch();
    worker->stopRun();
    worker->quit();
    worker->wait();
}

bool RobotManager::isRunning()
{
    return worker->isSearching();
}

void RobotManager::setWidget(QTreeWidget *listWidget)
{
    this->listWidget = listWidget;
}

void RobotManager::startSearch()
{
    if(!isRunning())
    {
        worker->startSearch();
    }
}

void RobotManager::stopSearch()
{
    if(isRunning())
    {
        worker->stopSearch();
    }
}

void RobotManager::handleResults()
{
    if(listWidget != nullptr)
    {
        listWidget->clear();
        for(int i = 0; i < robots.size(); i++)
        {
            QTreeWidgetItem *robot = new QTreeWidgetItem();
            robot->setText(0, robots[i].hostName);
            listWidget->addTopLevelItem(robot);
        }
        qDebug() << "Add";
    }
}
