#ifndef ROBOT_H
#define ROBOT_H

#include <QString>

typedef struct
{
    QString ip;
    QString hostName;
    bool naoQiRunning;
    int batteryLevel;
    int maxTemperature;
    int wifiSignal;
    bool wifiConnected;
}Robot;

#endif // ROBOT_H
