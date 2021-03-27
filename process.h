#ifndef PROCESS_H
#define PROCESS_H

#include <QThread>
#include "public.h"
#include <QDebug>
#include "qcustomplot.h"

class process : public QThread
{
    Q_OBJECT
signals:
    void processed();

public:
    process();
    bool isbusy;
    QVector<QCPCurveData> trajectory_data;
    void setCacheData(QByteArray d);
    void setReplyFlag(bool *flag);
    void processData();
    int getDataIndex(QByteArray data);

private:
    QByteArray cachedata;
    QByteArray rec_data;
    bool *reply_flag;

protected:
    void run();
};

#endif // PROCESS_H
