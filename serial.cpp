#include "serial.h"
#include <qdebug.h>

serial::serial(QObject *parent): QObject(parent) {
    serialdevice = new QSerialPort(this);
    QSerialPort::connect(serialdevice,&QSerialPort::errorOccurred,this,&serial::handleError);
}
bool serial::connectDevice(const QString &port, qint32 baudRate,QSerialPort::DataBits _databits,QSerialPort::StopBits _stopbits,QSerialPort::Parity _parity){
    serialdevice->setDataBits(_databits);
    serialdevice->setStopBits(_stopbits);
    serialdevice->setParity(_parity);
    serialdevice->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    serialdevice->setBaudRate(baudRate);
    serialdevice->setPortName(port);
    QSerialPort::connect(serialdevice,&QSerialPort::readyRead,this,&serial::onReadyRead);
    QSerialPort::connect(serialdevice,&QSerialPort::errorOccurred,this,&serial::handleError);
    if (!serialdevice->isOpen()) {
        QObject::connect(serialdevice, &QSerialPort::errorOccurred,
                         this, &serial::handleError);
    }

    return serialdevice->open(QIODevice::ReadWrite);

}
void serial::disconnect(){
    if(serialdevice->isOpen()){
        serialdevice->close();
    }
    emit deviceDisconnected();
}
void serial::onReadyRead(){
    QByteArray newData = serialdevice->readAll();
    if (newData.isEmpty()) return;
    buffer.append(newData);
    while (buffer.contains('\n')) {
        int newlinePos = buffer.indexOf('\n');

        QByteArray line = buffer.left(newlinePos + 1);

        buffer.remove(0, newlinePos + 1);


        QString cleanLine = QString::fromUtf8(line).trimmed();

        if (!cleanLine.isEmpty()) {
            emit dataReceived(cleanLine+"\r");
            qDebug()<<"Got data: "<<cleanLine;

        }
    }
}

void serial::handleError(QSerialPort::SerialPortError error){
    if(error == QSerialPort::ResourceError){
        if(serialdevice->isOpen()){
            serialdevice->close();
        }
    }
    emit deviceDisconnected();
}
void serial::writeData(const QString &suffix,const QString &data){
    serialdevice->write(data.toUtf8() + suffix.toUtf8());
}
