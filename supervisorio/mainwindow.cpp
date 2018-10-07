#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(processReadyReadStandardOutput()));
    connect(&process,SIGNAL(readyReadStandardError()),this,SLOT(processReadyReadStandardError()));
    connect(&process, SIGNAL(started()), this, SLOT(processStarted()));

    selectedRobot = "nao01.local";
    selectedToolchain = "robo";
    codeReleasePath = "/home/alexander/Development/Rinobot/Mari";

    loadModules();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::executeProcess(QString program, QStringList arguments)
{
    process.start(program,arguments);
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Finished: " << exitCode;
}

void MainWindow::processReadyReadStandardError()
{
    qDebug() << "ReadyError";
}

void MainWindow::processReadyReadStandardOutput()
{
    QProcess *p = (QProcess *)sender();
    QByteArray buf = p->readAllStandardOutput();

    qDebug() << buf;
}

void MainWindow::processStarted()
{
    qDebug() << "Proc Started";
}

void MainWindow::loadModules()
{
    QFile readfile(codeReleasePath + "/root/home/nao/data/config.json");
    if (!readfile.open(QIODevice::ReadOnly))
    {
        qWarning("Não foi possível atualizar os módulos");
        return;
    }
    QByteArray saveData = readfile.readAll();
    readfile.close();


    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject json = loadDoc.object();

    if(!json.contains("Modules"))
        json.insert("Modules", QJsonObject());

    QJsonObject modules = json["Modules"].toObject();

    loadModule(modules, "Motion", ui->chkMotion);
    loadModule(modules, "Perception", ui->chkPerception);
    loadModule(modules, "Strategy", ui->chkStrategy);
    loadModule(modules, "Remote", ui->chkRemoteControl);
    loadModule(modules, "Network", ui->chkNetwork);
}

void MainWindow::loadModule(QJsonObject &modules, QString moduleName, QCheckBox *checkbox)
{
    if(modules.contains(moduleName))
    {
        QJsonObject module = modules[moduleName].toObject();
        if(module.contains("Enabled"))
            checkbox->setChecked(module["Enabled"].toBool());
        else
            checkbox->setChecked(true);
    }
    else
        checkbox->setChecked(true);
}

void MainWindow::saveModules()
{
    QFile readfile(codeReleasePath + "/root/home/nao/data/config.json");
    if (!readfile.open(QIODevice::ReadOnly))
    {
        qWarning("Não foi possível atualizar os módulos");
        return;
    }
    QByteArray saveData = readfile.readAll();
    readfile.close();


    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject json = loadDoc.object();

    if(!json.contains("Modules"))
        json.insert("Modules", QJsonObject());

    QJsonObject modules = json["Modules"].toObject();

    // Motion
    saveModule(modules, "Motion", ui->chkMotion);
    saveModule(modules, "Perception", ui->chkPerception);
    saveModule(modules, "Strategy", ui->chkStrategy);
    saveModule(modules, "Remote", ui->chkRemoteControl);
    saveModule(modules, "Network", ui->chkNetwork);

    json["Modules"] = modules;

    QFile writefile(codeReleasePath + "/root/home/nao/data/config.json");
    if (!writefile.open(QIODevice::WriteOnly))
    {
        qWarning("Não foi possível atualizar os módulos");
        return;
    }
    QJsonDocument saveDoc(json);
    writefile.write(saveDoc.toJson());
    writefile.close();
}

void MainWindow::saveModule(QJsonObject &modules, QString moduleName, QCheckBox *checkbox)
{
    if(!modules.contains(moduleName))
        modules.insert(moduleName, QJsonObject());
    QJsonObject module = modules[moduleName].toObject();
    module["Enabled"] = checkbox->isChecked();
    modules[moduleName] = module;
}

void MainWindow::on_btnInstall_clicked()
{
    saveModules();
    QString program = "bash";
    QStringList arguments;
    arguments << "-c" << "cd "+ codeReleasePath + "; ./sync.sh " + selectedRobot + " " + selectedToolchain;
    executeProcess(program, arguments);
}

void MainWindow::on_btnUninstall_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Desinstalar");
    msgBox.setInformativeText("Você realmente deseja efetuar a desinstalação em " + selectedRobot + "?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Yes)
    {
        QString program = "bash";
        QStringList arguments;
        arguments << "-c" << "cd "+ codeReleasePath + "; ./sync.sh " + selectedRobot + " " + selectedToolchain + "-uninstall";
        executeProcess(program, arguments);
    }
}
