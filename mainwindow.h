#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include "qcustomplot.h"
#include "process.h"
#include "public.h"

#define OPENSTATE   0
#define CLOSESTATE  1

#define BASETIME    500

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    process *processor;
    QTimer *timer = new QTimer(this);
    QList<QString> serialport_list;
    bool serial_state;
    QString  serial_choice;
    QSerialPort *serialport = new QSerialPort();
    QList<int> id_list;
    int max_line_id;
    typedef struct line
    {
        QVector<double> x;
        QVector<double> y;
    }line;
    QList<line> line_list;
    bool reply_flag,autosend_flag;
    QCPCurve *trajectory;
    void widgetInit();
    bool openSerialport();
    void closeSerialport();

private slots:
   void changeSerialState();
   void searchSerialport();
   void recordSerialChoice(int choice);
   void readSerialport();
   void updateMapSize();
   void updateGraph();
};
#endif // MAINWINDOW_H
