#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QCheckBox>
#include "robotmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void 	processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void 	processReadyReadStandardError();
    void 	processReadyReadStandardOutput();
    void 	processStarted();

    void on_btnInstall_clicked();

    void on_btnUninstall_clicked();

    void on_treeRobots_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::MainWindow *ui;
    QProcess process;
    QString codeReleasePath;
    QString selectedRobot;
    QString selectedToolchain;

    RobotManager robotManager;

    void loadModules();
    void loadModule(QJsonObject &modules, QString moduleName, QCheckBox *checkbox);
    void saveModules();
    void saveModule(QJsonObject &modules, QString moduleName, QCheckBox *checkbox);
    void executeProcess(QString program, QStringList arguments);
};

#endif // MAINWINDOW_H
