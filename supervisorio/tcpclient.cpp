#include <QAbstractSocket>
#include "tcpclient.h"
#include "logger.h"

TCPClient::TCPClient()
{
    keepRunning = true;
    imageType = "";
    port = 9572;
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

}

TCPClient::~TCPClient()
{
    keepRunning = false;
}

void TCPClient::stop()
{
    keepRunning = false;
    quit();
    wait();
}

void TCPClient::setImageType(QString name)
{
    imageType = name;
    Logger::log("Image type: "+imageType);
}

void TCPClient::setPort(int port)
{
    this->port = port;
}

void TCPClient::processImage(ImageMessage &imageMessage)
{
    if(imageType.isEmpty() || imageType == imageMessage.getName())
    {
        emit updateImage(imageMessage);
    }
}

void TCPClient::run()
{
    emit addImageType("Raw YUV");
    emit addImageType("Raw RGB");

    while(keepRunning)
    {
        ImageMessage imageMessage("Raw RGB", 320, 240, IMAGE_TYPE_RGB);
        processImage(imageMessage);
        QThread::msleep(30);
    }
}

bool TCPClient::connectToHost(QString address)
{
    if(isConnected())
        disconnectFromHost();
    this->address = address;
    Logger::log("Connecting to "+address+":"+QString::number(port));
    socket->connectToHost(address, port);
    if(!socket->waitForConnected(1000))
    {
        Logger::log("Failed connecting to "+address+":"+QString::number(port)+" -> "+socket->errorString());
        return false;
    }
    return true;
}

bool TCPClient::disconnectFromHost()
{
    if(isConnected())
    {
        Logger::log("Disconnecting from "+address+":"+QString::number(port));
        socket->disconnectFromHost();
    }
    return true;
}

bool TCPClient::isConnected()
{
    return socket->isOpen();
}

bool TCPClient::send(Message *message)
{
    QByteArray data;
    if(message->encode(data) > 0)
    {
        Logger::log("Sending: "+message->toString());
        socket->write(data);
        return true;
    }
    else
    {
        Logger::log("Cannot send: "+message->toString());
    }
    return false;
}

void TCPClient::connected()
{
    Logger::log("Connected to "+address+":"+QString::number(port));

    //socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
}

void TCPClient::disconnected()
{
    Logger::log("Disconnected from "+address+":"+QString::number(port));
}

void TCPClient::bytesWritten(qint64 bytes)
{
    Logger::log("We wrote: " + QString::number(bytes));
}

void TCPClient::readyRead()
{
    QByteArray data = socket->readAll();
    Message message;
    int skipSize = message.decode(data);
    if(skipSize == 0)
        Logger::log("Invalid message received");
    else
    {
        Logger::log("Received: " + message.toString());
    }
}


