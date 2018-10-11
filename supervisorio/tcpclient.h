#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QThread>

class TCPClient : public QThread
{
    Q_OBJECT

    private:
        bool keepRunning;
        QString imageType;

        void processImage(QString imageType);
    public:
        TCPClient();
        ~TCPClient();

        void stop();
        void setImageType(QString name);

        void run() override;

    signals:
        void updateImage();
        void addImageType(QString name);
};

#endif // TCPCLIENT_H
