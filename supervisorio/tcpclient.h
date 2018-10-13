#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QThread>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "message.h"
#include "imagemessage.h"

class TCPClient : public QThread
{
    Q_OBJECT

    private:
        bool keepRunning;
        int port;
        QString address;
        QString imageType;
        QTcpSocket *socket;

        void processImage(ImageMessage &imageMessage);
    public:
        TCPClient();
        ~TCPClient();

        void stop();
        void setImageType(QString name);
        void setPort(int port);

        bool connectToHost(QString address);
        bool disconnectFromHost();
        bool isConnected();
        bool send(Message *message);

        void run() override;

    public slots:
        void connected();
        void disconnected();
        void bytesWritten(qint64 bytes);
        void readyRead();

    signals:
        void updateImage(ImageMessage imageMessage);
        void addImageType(QString name);
};

#endif // TCPCLIENT_H
