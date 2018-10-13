#include "imagemessage.h"

ImageMessage::ImageMessage() : Message ()
{

}

ImageMessage::ImageMessage(QString name, int width, int height, int imageType) : Message ()
{
    setType(TYPE_IMAGE);
    this->name = name;
    this->width = width;
    this->height = height;
    this->imageType = imageType;
    dataSize = width*height;
    step = width;
    switch (imageType)
    {
        case IMAGE_TYPE_GRAY:
            break;
        case IMAGE_TYPE_BGR:
        case IMAGE_TYPE_RGB:
        case IMAGE_TYPE_HSV:
            dataSize *= 3;
            step *= 3;
            break;
        case IMAGE_TYPE_YUV:
            dataSize *= 2;
            step *= 2;
            break;
    }
    data = new uchar[dataSize];
    for(int i = 0; i < dataSize; i++)
        data[i] = rand() % 256;
}

ImageMessage::ImageMessage(const ImageMessage &imageMessage) : Message (imageMessage)
{
    name = imageMessage.name;
    width = imageMessage.width;
    height = imageMessage.height;
    imageType = imageMessage.imageType;
    dataSize = imageMessage.dataSize;
    step = imageMessage.step;
    data = new uchar[dataSize];
    memcpy(data, imageMessage.data, dataSize);
}

ImageMessage::ImageMessage(QString name, int width, int height, int imageType, uchar *data) : ImageMessage(name, width, height, imageType)
{
    memcpy(this->data, data, dataSize);
}

ImageMessage::~ImageMessage()
{
    delete[] data;
}

int ImageMessage::getWidth()
{
    return width;
}

int ImageMessage::getHeight()
{
    return height;
}

int ImageMessage::getImageType()
{
    return imageType;
}

int ImageMessage::getStep()
{
    return step;
}

QString ImageMessage::getName()
{
    return name;
}

uint8_t *ImageMessage::getData()
{
    return data;
}

QString ImageMessage::toString()
{
    return "Image message: "+name;
}
