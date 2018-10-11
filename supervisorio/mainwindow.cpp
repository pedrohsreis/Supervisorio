#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QProcess>
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

    selectedRobot = "127.0.0.1";
    findToolchains();
    load();
    loadModules();

    robotManager.setWidget(ui->treeRobots);
    robotManager.startSearch();

}

MainWindow::~MainWindow()
{
    robotManager.stopSearch();
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

void MainWindow::on_treeRobots_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current != nullptr)
        selectedRobot = current->text(0);
    else
        selectedRobot = "127.0.0.1";
    qDebug() << "Current robot:" << selectedRobot;
}

void MainWindow::findToolchains()
{
    ui->comboToolchain->clear();

    QProcess process;
    process.start("qitoolchain", QStringList() << "list");
    process.waitForFinished();
    QString output(process.readAllStandardOutput());
#ifdef Q_OS_WIN32
    QStringList lines = output.split("\r\n", QString::SplitBehavior::SkipEmptyParts);
#else
    QStringList lines = output.split("\n", QString::SplitBehavior::SkipEmptyParts);
#endif
    for (int i = 0; i < lines.size(); i++)
    {
        QString line = lines[i].trimmed();
        if(line.at(0) == '*')
        {
            QString item = line.mid(line.indexOf('*')+1).trimmed();
            ui->comboToolchain->addItem(item);
        }
    }
    if(ui->comboToolchain->count() == 0)
    {
        ui->comboToolchain->addItem("Cannot find a toolchain");
        ui->btnCompile->setEnabled(false);
        ui->btnInstall->setEnabled(false);
        ui->btnConfigure->setEnabled(false);
        ui->btnClear->setEnabled(false);
    }
    ui->comboToolchain->setCurrentIndex(0);
    selectedToolchain = ui->comboToolchain->currentText();
    if(selectedToolchain.contains(' '))
        selectedToolchain = "any";
}

void MainWindow::on_comboToolchain_activated(const QString &arg1)
{
    selectedToolchain = arg1;
    if(selectedToolchain.contains(' '))
        selectedToolchain = "any";
    qDebug() << "Current toolchain: " << selectedToolchain;
}

void MainWindow::load()
{
    QFile readfile("settings.json");
    if (!readfile.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray saveData = readfile.readAll();
    readfile.close();


    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject json = loadDoc.object();

    codeReleasePath = loadSetting(json, "codeReleasePath", "");
    selectedToolchain = loadSetting(json, "toolchain", "any");
    ui->comboToolchain->setCurrentText(selectedToolchain);
    ui->teCodeRelease->setText(codeReleasePath);
}

QString MainWindow::loadSetting(QJsonObject &json, QString name, QString defaltValue)
{
    if(json.contains(name))
        return json[name].toString();
    return defaltValue;
}

void MainWindow::save()
{
    QJsonObject json;
    QFile readfile("settings.json");
    if (readfile.open(QIODevice::ReadOnly))
    {
        QByteArray saveData = readfile.readAll();
        readfile.close();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        json = loadDoc.object();
    }



    saveSetting(json, "codeReleasePath", codeReleasePath);
    saveSetting(json, "toolchain", selectedToolchain);

    QFile writefile("settings.json");
    if (!writefile.open(QIODevice::WriteOnly))
    {
        return;
    }
    QJsonDocument saveDoc(json);
    writefile.write(saveDoc.toJson());
    writefile.close();
}

void MainWindow::saveSetting(QJsonObject &json, QString name, QString value)
{
    if(!json.contains(name))
        json.insert(name, value);
    else
        json[name] = value;
}

void MainWindow::closeEvent(QCloseEvent *bar)
{
    save();
}

void MainWindow::on_btnAddRobot_clicked()
{
    QString ip = ui->edRobotIp->text();
    if(!robotManager.addRobot(ip))
    {

    }
}

void MainWindow::on_btnSelectCodeRelease_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select path", "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(path != NULL && path.length() > 0){
        ui->teCodeRelease->setText(path);
        codeReleasePath = path;
    }
}

void MainWindow::on_btnCompile_clicked()
{
    QString program = "bash";
    QStringList arguments;
    arguments << "-c" << "cd "+ codeReleasePath + "; ./build.sh " + selectedToolchain;
    executeProcess(program, arguments);
}
