#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_pushButtonEnviar_clicked();
    void on_pushButtonConectar_clicked();
    void Recibir_info();
    void on_dial_temp_valueChanged(int value);

private:
    QSerialPort *port;
    void Conectar_puerto();
};

#endif // MAINWINDOW_H
