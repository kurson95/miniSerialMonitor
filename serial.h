#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
class serial:public QObject
{
    Q_OBJECT
private:
    QSerialPort *serialdevice;
    QByteArray buffer;


signals:
    void dataReceived(const QString &data);
    void deviceDisconnected();
    void deviceConnected();
    void connectionFailed();
private slots:
    void onReadyRead();
    void handleError(QSerialPort::SerialPortError error);
public:
    explicit serial(QObject *parent = nullptr);
    bool connectDevice(const QString &port, qint32 baudRate,QSerialPort::DataBits,QSerialPort::StopBits,QSerialPort::Parity);
    void disconnect();
    void writeData(const QString&,const QString&);

};

#endif // SERIAL_H
