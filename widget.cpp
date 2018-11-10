#include "widget.h"
#include "ui_widget.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QtDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    foreach(const QSerialPortInfo &Info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "PortName   :" << Info.portName();  //调试时可以看串口信息
        qDebug() << "Description    :" << Info.description();
        qDebug() << "Manufacturer:" << Info.manufacturer();

        QSerialPort serial;
        serial.setPort(Info);

        if(serial.open(QIODevice::ReadWrite))//如果串口时可以读写方式打开的
        {
            ui->comboBox_portName->addItem(Info.portName());//在comboBox那添加串口号
            serial.close();//然后自动关闭等待人为开启（通过那个打开串口的PushButton）
        }
    }

    my_serialPort = new QSerialPort(this);
    timer = new QTimer(this);
    ui->pushButton_comopen->QAbstractButton::setText("Open");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_send_clicked()
{
    if(my_serialPort->isOpen())
    {
        my_serialPort->write(ui->lineEdit_write->text().toUtf8());
    }
}

void Widget::readMyCom()//读取缓冲的数据，每秒读一次
{
    requestData = my_serialPort->readAll();//用requestData存储从串口那读取的数据
    if(!requestData.isNull())//如果读取到有数据，就显示出来。
    {
        ui->textEdit_read->append(requestData);
    }
    requestData.clear();    //清除缓冲区
}

void Widget::on_pushButton_comopen_clicked()
{
    if(!my_serialPort->isOpen())
    {
        my_serialPort->setPortName(ui->comboBox_portName->currentText());
        my_serialPort->open(QIODevice::ReadWrite);

        qDebug() << ui->comboBox_portName->currentText();

        setBaudRate();
        setDataBits();
        setParity();
        setFlowCtrl();
        setStopBits();

        connect(timer, SIGNAL(timeout()), this, SLOT(readMyCom()));
        timer->start(1000);//每秒读一次

        ui->pushButton_comopen->QAbstractButton::setText("Close");
    }
    else
    {
        my_serialPort->close();
        timer->stop();

        ui->pushButton_comopen->QAbstractButton::setText("Open");
    }
}

void Widget::setBaudRate()
{
#define BAUD_RATE_CFG (ui->comboBox_baudRate->currentText())

    if(BAUD_RATE_CFG == "2400")
        my_serialPort->setBaudRate(QSerialPort::Baud2400);
    else if(BAUD_RATE_CFG == "4800")
        my_serialPort->setBaudRate(QSerialPort::Baud4800);
    else if(BAUD_RATE_CFG == "9600")
        my_serialPort->setBaudRate(QSerialPort::Baud9600);
    else if(BAUD_RATE_CFG == "19200")
        my_serialPort->setBaudRate(QSerialPort::Baud19200);
    else if(BAUD_RATE_CFG == "38400")
        my_serialPort->setBaudRate(QSerialPort::Baud38400);
    else if(BAUD_RATE_CFG == "57600")
        my_serialPort->setBaudRate(QSerialPort::Baud57600);
    else if(BAUD_RATE_CFG == "115200")
        my_serialPort->setBaudRate(QSerialPort::Baud115200);
    else
        my_serialPort->setBaudRate(QSerialPort::Baud115200);
}

void Widget::setParity()
{
#define PARITY_CFG   (ui->comboBox_parity->currentText())

    if(PARITY_CFG == "None")
        my_serialPort->setParity(QSerialPort::NoParity);
    else if(PARITY_CFG == "Odd")
        my_serialPort->setParity(QSerialPort::OddParity);
    else if(PARITY_CFG == "Even")
        my_serialPort->setParity(QSerialPort::EvenParity);
    else if(PARITY_CFG == "Mark")
        my_serialPort->setParity(QSerialPort::MarkParity);
    else if(PARITY_CFG == "Space")
        my_serialPort->setParity(QSerialPort::SpaceParity);
    else
        my_serialPort->setParity(QSerialPort::NoParity);
}

void Widget::setDataBits()
{
#define DATA_BITS_CFG  (ui->comboBox_dataBits->currentText())

    if(DATA_BITS_CFG == "8")
        my_serialPort->setDataBits(QSerialPort::Data8);
    else if(DATA_BITS_CFG == "7")
        my_serialPort->setDataBits(QSerialPort::Data7);
    else if(DATA_BITS_CFG == "6")
        my_serialPort->setDataBits(QSerialPort::Data6);
    else if(DATA_BITS_CFG == "5")
        my_serialPort->setDataBits(QSerialPort::Data5);
    else
        my_serialPort->setDataBits(QSerialPort::Data8);
}

void Widget::setStopBits()
{
#define STOP_BITS_CFG  (ui->comboBox_stopBits->currentText())

    if(STOP_BITS_CFG == "1")
        my_serialPort->setStopBits(QSerialPort::OneStop);
    else if(STOP_BITS_CFG == "1.5")
        my_serialPort->setStopBits(QSerialPort::OneAndHalfStop);
    else if(STOP_BITS_CFG == "2")
        my_serialPort->setStopBits(QSerialPort::TwoStop);
    else
        my_serialPort->setStopBits(QSerialPort::OneStop);
}

void Widget::setFlowCtrl()
{
    my_serialPort->setFlowControl(QSerialPort::NoFlowControl);//数据流控制，无
}
