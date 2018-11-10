#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_send_clicked();

    void readMyCom();//用于读取数据

    void on_pushButton_comopen_clicked();

private:
    void setBaudRate();

    void setParity();

    void setDataBits();

    void setStopBits();

    void setFlowCtrl();

    Ui::Widget *ui;
    QSerialPort *my_serialPort; //(实例化一个指向串口的指针，用于访问串口)
    QByteArray  requestData;    //(用于存储从串口那读取的数据)
    QTimer  *timer;             //(用于计时)
};

#endif // WIDGET_H
