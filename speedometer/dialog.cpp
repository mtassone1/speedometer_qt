/*
 *   Copyright (C) 2016 by P. Sereno
 *   http://www.qt-italia.net
 */

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    port = NULL;
    Conectar_puerto();
    ui->meterWidget->setForeground(Qt::white);
    connect(ui->pushButtonConectar,SIGNAL(released()),this,SLOT(on_pushButtonConectar_clicked()));
    connect(ui->pushButtonEnviar,SIGNAL(released()),this,SLOT(on_pushButtonEnviar_clicked()));
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),ui->meterWidget,SLOT(setValue(int)));
    connect(ui->Slider_Vel,SIGNAL(sliderMoved(int)),ui->gaugeWidget,SLOT(setValue(int)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::Conectar_puerto(){

    ui->Ddmenu_puertos->clear();

    QList<QSerialPortInfo> puerto = QSerialPortInfo::availablePorts();

    for (int i = 0; i < puerto.size(); i++)
    {
        ui->Ddmenu_puertos->addItem(puerto.at(i).portName());
    }
}

void Dialog::on_pushButtonConectar_clicked(){

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
            //QMessageBox::critical(this,"Error","No se puede abrir el puerto " + port->portName());
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

void Dialog::on_pushButtonEnviar_clicked(){

    if(port)
    {
        QByteArray enviar;
        enviar.append('#');
        enviar.append(ui->Slider_Vel->sliderPosition());
        enviar.append('&');
        port->write(enviar);
    }
    else
    {
        ui->lineEdit->setText("No hay puerto abierto");
    }
}

void Dialog::Recibir_info(){
    QByteArray info;
    //static int i=0;
    int vel;

    info.resize(port->bytesAvailable());
    port->read(info.data(),info.size());
    vel = info.toInt();

    if(vel>1 && vel<100){
        ui->meterWidget->setValue(vel);
    }
}
