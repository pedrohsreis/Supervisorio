#include "tcpclient.h"
#include "logger.h"

TCPClient::TCPClient()
{
    keepRunning = true;
    imageType = "";
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

void TCPClient::processImage(QString imageType)
{
    if(imageType.isEmpty())
        emit updateImage();
    else if(this->imageType == imageType)
        emit updateImage();
}

void TCPClient::run()
{
    emit addImageType("Raw YUV");
    emit addImageType("Raw RGB");
    while(keepRunning)
    {
        processImage("Raw RGB");
        QThread::msleep(30);
    }
}


