#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QErrorMessage>
#include <QDateTime>
#include "serial.h"
#include "csv_save.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refreshPort(QComboBox *combobox);
    bool saveascsv(const QString *data);
    QDateTime timestamp;

    csv_save *save;
    serial *serialConnection;
signals:

private slots:
    void on_connect_btn_clicked();
    void updateTerminal(const QString &data);
    void onDeviceConnect();
    void onDeviceDisconnect();
    void onConnectionFail();
    void on_send_btn_clicked();
    void on_disconnect_btn_clicked();

    void on_echoena_toggled(bool checked);

    void on_actionRefresh_ports_triggered();


    void on_Serial_input_returnPressed();

    void on_timestamp_ena_toggled(bool checked);

    void on_save_csv_btn_clicked();

private:
    Ui::MainWindow *ui;
    const QString suffix;
    bool enableEcho = true;
    bool enabletimestamp = false;
};
#endif // MAINWINDOW_H
