#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

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


void MainWindow::on_btnInstall_clicked()
{
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
