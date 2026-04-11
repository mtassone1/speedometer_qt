#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButtonConectar_clicked();
    void on_pushButtonEnviar_clicked();
    void Recibir_info();

private:
    QSerialPort *port;
    void Conectar_puerto();
    Ui::Dialog *ui;
};

#endif // DIALOG_H
