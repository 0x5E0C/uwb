#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    widgetInit();
    connect(timer,SIGNAL(timeout()),this,SLOT(searchSerialport()));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(changeSerialState()));
    connect(ui->serialportBox,SIGNAL(activated(int)),this,SLOT(recordSerialChoice(int)));
    connect(serialport,SIGNAL(readyRead()),this,SLOT(readSerialport()));
    connect(ui->updateMapButton,SIGNAL(clicked()),this,SLOT(updateMapSize()));
    connect(processor,SIGNAL(processed()),this,SLOT(updateGraph()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::widgetInit()
{
    serial_state=CLOSESTATE;
    serial_choice="";
    max_line_id=0;
    reply_flag=false;
    autosend_flag=false;
    processor = new process();
    processor->setReplyFlag(&reply_flag);
    ui->openButton->setIcon(QPixmap(":/state/close.png"));
    ui->openButton->setText("打开串口");
    ui->widget->setInteractions(QCP::iSelectAxes|QCP::iSelectLegend|QCP::iSelectPlottables);
    timer->start(BASETIME);
}

void MainWindow::changeSerialState()
{
    if(serial_state==CLOSESTATE && openSerialport())
    {
        ui->openButton->setIcon(QPixmap(":/state/open.png"));
        ui->openButton->setText("关闭串口");
        serial_state=OPENSTATE;
    }
    else if(serial_state==OPENSTATE)
    {
        ui->openButton->setIcon(QPixmap(":/state/close.png"));
        ui->openButton->setText("打开串口");
        serial_state=CLOSESTATE;
        closeSerialport();
    }
}

void MainWindow::recordSerialChoice(int choice)
{
    serial_choice=ui->serialportBox->itemText(choice);
    if(serial_state==OPENSTATE)
    {
        closeSerialport();
        openSerialport();
    }
}

void MainWindow::searchSerialport()
{
    QList<QString> temp_list;
    if(QSerialPortInfo::availablePorts().size()!=0)
    {
        if(ui->serialportBox->itemText(0)=="")
        {
            ui->serialportBox->removeItem(0);
        }
        if(serialport_list.isEmpty())
        {
            foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
            {
                serialport_list.append(info.portName());
            }
            for(int i=0;i<serialport_list.size();i++)
            {
                ui->serialportBox->addItem(serialport_list[i]);
            }
        }
        else
        {
            foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
            {
                temp_list.append(info.portName());
            }
            if(temp_list!=serialport_list)
            {
                ui->serialportBox->clear();
                for(int i=0;i<temp_list.size();i++)
                {
                    ui->serialportBox->addItem(temp_list[i]);
                }
                serialport_list=temp_list;
            }
        }
        ui->serialportBox->setCurrentText(serial_choice);
    }
    else
    {
        ui->serialportBox->clear();
        ui->serialportBox->addItem("");
        serialport_list.clear();
    }
}

bool MainWindow::openSerialport()
{
    serialport->setPortName(ui->serialportBox->currentText());
    serialport->setBaudRate(ui->baudrateBox->currentText().toInt());
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setFlowControl(QSerialPort::NoFlowControl);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setParity(QSerialPort::NoParity);
    QSerialPortInfo serial_info(ui->serialportBox->currentText());
    if(serial_info.isBusy()==true)
    {
        QString dlgTitle="错误";
        QString strInfo="打开串口失败!串口已被占用!";
        QMessageBox::critical(this,dlgTitle,strInfo);
        serial_state=OPENSTATE;
        changeSerialState();
        return false;
    }
    if(!serialport->open(QIODevice::ReadWrite))
    {
        QString dlgTitle="错误";
        QString strInfo="打开串口失败!";
        QMessageBox::critical(this,dlgTitle,strInfo);
        return false;
    }
    return true;
}

void MainWindow::closeSerialport()
{
    serialport->close();
}

void MainWindow::readSerialport()
{
    QByteArray src_data=serialport->readAll();
    processor->setCacheData(src_data);
    if(!processor->isbusy)
    {
        processor->start();
    }
}

void MainWindow::updateMapSize()
{
    if(ui->editMapX->text().isEmpty() || ui->editMapY->text().isEmpty())
    {
        QString dlgTitle="错误";
        QString strInfo="请先填写地图信息!";
        QMessageBox::critical(this,dlgTitle,strInfo);
        return;
    }
    int x=ui->editMapX->text().toInt();
    int y=ui->editMapY->text().toInt();
    ui->widget->xAxis->setRange(0,x);//设置x轴长度
    ui->widget->yAxis->setRange(0,y);//设置y轴长度
    trajectory=new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
    trajectory->setPen(QPen(Qt::blue));
    ui->widget->replot();
}

void MainWindow::updateGraph()
{
    trajectory->data()->set(processor->trajectory_data);
    ui->widget->replot();
}
