#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QByteArray>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    port = NULL;
    Conectar_puerto();
    ui->dial_temp->setMinimum(40);
    ui->dial_temp->setMaximum(95);
}

MainWindow::~MainWindow(){
    delete ui;
    delete port;
}

void MainWindow::Conectar_puerto(){

    ui->Ddmenu_puertos->clear();

    QList<QSerialPortInfo> puerto = QSerialPortInfo::availablePorts();

    for (int i = 0; i < puerto.size(); i++)
    {
        ui->Ddmenu_puertos->addItem(puerto.at(i).portName());
    }
}

void MainWindow::on_pushButtonConectar_clicked(){

    if(!port)
    {
        port = new QSerialPort(ui->Ddmenu_puertos->currentText());
        port->setBaudRate(QSerialPort::Baud115200);
        port->setFlowControl(QSerialPort::NoFlowControl);
        port->setParity(QSerialPort::NoParity);
        port->setDataBits(QSerialPort::Data8);
        port->setStopBits(QSerialPort::OneStop);
        if(!port->open(QIODevice::ReadWrite))
        {
            QMessageBox::critical(this,"Error","No se puede abrir el puerto " + port->portName());
            delete port;
            port = NULL;
        }
        else
        {
            ui->pushButtonConectar->setText("Desconectar");
            connect(port,SIGNAL(readyRead()),this,SLOT(Recibir_info()));
        }
    }
    else
    {
        delete port;
        port = NULL;
        ui->pushButtonConectar->setText("Conectar");
    }
}

void MainWindow::on_pushButtonEnviar_clicked(){

    if(port)
    {
        QByteArray enviar;
        enviar.append('#');
        enviar.append(ui->dial_temp->sliderPosition());
        enviar.append('&');
        port->write(enviar);
    }
    else
    {
        QMessageBox::critical(this, "Error", QString::fromUtf8("No hay ningún puerto abierto"));
    }
}

void MainWindow::on_dial_temp_valueChanged(int value)
{
    ui->lcdNumber->display(value);
}

void MainWindow::Recibir_info(){
    QByteArray info;
    static int i=0;
    info.resize(port->bytesAvailable());
    port->read(info.data(),info.size());
    if(i>5){
        ui->Text_info->clear();
        i=0;
    }
    ui->Text_info->insertPlainText(info);
    ui->Text_info->insertPlainText("\n");
    i++;
}

