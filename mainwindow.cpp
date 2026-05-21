#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    serialConnection = new serial(this);
    ui->setupUi(this);
    ui->baudrate_select->addItem("9600", QSerialPort::Baud9600);
    ui->baudrate_select->addItem("19200", QSerialPort::Baud19200);
    ui->baudrate_select->addItem("38400", QSerialPort::Baud38400);
    ui->baudrate_select->addItem("57600", QSerialPort::Baud57600);
    ui->baudrate_select->addItem("115200", QSerialPort::Baud115200); // Standard dla firmware AT ESP
    ui->stopbits_select->addItem("1",QSerialPort::StopBits::OneStop);
    ui->stopbits_select->addItem("2",QSerialPort::StopBits::TwoStop);
    ui->databits_select->addItem("5",QSerialPort::DataBits::Data5);
    ui->databits_select->addItem("6",QSerialPort::DataBits::Data6);
    ui->databits_select->addItem("7",QSerialPort::DataBits::Data7);
    ui->databits_select->addItem("8",QSerialPort::DataBits::Data8);
    ui->databits_select->setCurrentIndex(3);
    ui->parity_select->addItem("No parity",QSerialPort::Parity::NoParity);
    ui->parity_select->addItem("Even parity",QSerialPort::Parity::EvenParity);
    ui->parity_select->addItem("Mark parity",QSerialPort::Parity::MarkParity);
    ui->parity_select->addItem("Odd parity",QSerialPort::Parity::OddParity);
    ui->parity_select->addItem("Space parity",QSerialPort::Parity::SpaceParity);
    ui->suffix_select->addItem("CRLF (\\r\\n)", "\r\n");
    ui->suffix_select->addItem("CR (\\r)", "\r");
    ui->suffix_select->addItem("LF (\\n)", "\n");
    ui->suffix_select->addItem("none","");
    refreshPort(ui->port_select);
    save = new csv_save(this);
    ui->send_btn->setDisabled(true);
    ui->connect_btn->setDisabled(false);
    ui->disconnect_btn->setDisabled(true);
    ui->baudrate_select->setCurrentIndex(4);

    connect(serialConnection,&serial::dataReceived,this,&MainWindow::updateTerminal);
    connect(serialConnection,&serial::deviceConnected,this,&MainWindow::onDeviceConnect);
    connect(serialConnection,&serial::deviceDisconnected,this,&MainWindow::onDeviceDisconnect);
    connect(serialConnection,&serial::connectionFailed,this,&MainWindow::onConnectionFail);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::refreshPort(QComboBox *comboBox){

    comboBox->clear();
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : availablePorts) {
        // Extract port details
        QString portName = portInfo.portName();       // e.g., "ttyUSB0"
        QString description = portInfo.description(); // e.g., "CP2102 USB to UART Bridge Controller"
        QString manufacturer = portInfo.manufacturer();

        // Create a user-friendly display string
        QString displayString = portName;
        if (!description.isEmpty()) {
            displayString += " - " + description;
        }

        // Add the item to the combo box.
        // We store the raw portName as UserData for easy retrieval later.
        comboBox->addItem(displayString, portName);
        qDebug()<<"Found port:"<<displayString;
    }

}

void MainWindow::on_connect_btn_clicked()
{   qint32 baud;
    QString port;
    baud = ui->baudrate_select->currentData().toInt();
    port = ui->port_select->currentData().toString();
    QSerialPort::DataBits databits = static_cast<QSerialPort::DataBits>(ui->databits_select->currentData().toInt());
    QSerialPort::StopBits stopbits = static_cast<QSerialPort::StopBits>(ui->stopbits_select->currentData().toInt());
    QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(ui->parity_select->currentData().toInt());
    qDebug()<<"Connecting to device on port"<<port;
    if(serialConnection->connectDevice(port,baud,databits,stopbits,parity)){
        emit serialConnection->deviceConnected();
    }else{
        emit serialConnection->connectionFailed();
    }
}


void MainWindow::updateTerminal(const QString &data){
    qDebug()<<data;
    QString out;
    if(enabletimestamp){
    QString _timestamp = timestamp.time().currentTime().toString();
        out = QString("%1,%2").arg(_timestamp).arg(data);
    }else{
        out = data;
    }
    ui->Serial_output->moveCursor(QTextCursor::End);
    ui->Serial_output->insertPlainText(out);
    ui->Serial_output->ensureCursorVisible();
}
void MainWindow::onDeviceConnect(){
    qDebug()<<"Device connected";
    ui->connect_btn->setDisabled(true);
    ui->disconnect_btn->setDisabled(false);
    ui->send_btn->setDisabled(false);
    ui->Serial_output->setPlainText("");
    ui->Serial_output->update();
}

void MainWindow::onDeviceDisconnect(){
    qDebug()<<"Device disconnected";
    ui->connect_btn->setDisabled(false);
    ui->disconnect_btn->setDisabled(true);
    ui->send_btn->setDisabled(true);
}
void MainWindow::onConnectionFail(){
    QMessageBox::critical(this, "Error","Connection failed");
    qDebug()<<"Conection failed";
}


void MainWindow::on_send_btn_clicked()
{
    const QString _data = ui->Serial_input->text();
    const QString _suffix = ui->suffix_select->currentData().toString();
    serialConnection->writeData(_suffix,_data);
    if(enableEcho){
    ui->Serial_output->moveCursor(QTextCursor::End);
    ui->Serial_output->insertPlainText(_data+_suffix);
    ui->Serial_output->ensureCursorVisible();
    ui->Serial_input->clear();
    }
}


void MainWindow::on_disconnect_btn_clicked()
{
    serialConnection->disconnect();
}


void MainWindow::on_echoena_toggled(bool checked)
{
    enableEcho = checked;
}


void MainWindow::on_actionRefresh_ports_triggered()
{
    qDebug()<<"Refreshing ports...";
    MainWindow::refreshPort(ui->port_select);
}




void MainWindow::on_Serial_input_returnPressed()
{
    on_send_btn_clicked();
}


void MainWindow::on_timestamp_ena_toggled(bool checked)
{
    enabletimestamp = checked;
}


void MainWindow::on_save_csv_btn_clicked()
{
    QString split = ui->frame_split->text();
    QString stop = ui->frame_stop->text();
    QString _filePath = QDir::homePath();
    QString _initialDir = QDir::homePath();
    _filePath = QFileDialog::getSaveFileName(this,tr("Save file"),_initialDir,tr("CSV files (*.csv)"));
    if(!_filePath.isEmpty()){
        QString _fileContent = ui->Serial_output->toPlainText();
        if(stop.isEmpty()) stop = "\n";
        if(split.isEmpty()) split = ", ";
        bool succes = save->saveFile(_filePath,_fileContent,stop,split);
        if(succes)
        {            QMessageBox succesMsg;
            succesMsg.setText("File saved succesfully");
            succesMsg.exec();
        }else{
            QErrorMessage errorMsg;
            errorMsg.showMessage("Error saving file");
        }
    }}

