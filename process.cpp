#include "process.h"

process::process()
{
    isbusy=false;
    trajectory_data.resize(0);
}

void process::setCacheData(QByteArray d)
{
    cachedata.append(d);
}

void process::setReplyFlag(bool *flag)
{
    reply_flag=flag;
}

int process::getDataIndex(QByteArray data)
{
    int index=data.indexOf(0x5E);
    if(index!=(data.size()-1))
    {
        if(data.at(index+1)==0x0C)
        {
            return index;
        }
    }
    return -1;
}

void process::run()
{
    quint16 sum;
    int index;
    isbusy=true;
    while((cachedata.size()+rec_data.size())>=PACKET_LENGTH)
    {
        rec_data.append(cachedata);
        cachedata.clear();
        index=getDataIndex(rec_data);
        if(index==-1)
        {
            rec_data.clear();
            continue;
        }
        QByteArray tempbuffer=rec_data.mid(index,PACKET_LENGTH);
        quint8 *rec_buffer=(quint8*)tempbuffer.data();
        rec_data=rec_data.mid(index+1);
        sum=0;
        for(int i=0;i<8;i++)
        {
            sum+=rec_buffer[i];
        }
        if(sum!=(rec_buffer[8]<<8|rec_buffer[9]))
        {
            continue;
        }
        qint16 x=rec_buffer[4]<<8|rec_buffer[5];
        qint16 y=rec_buffer[6]<<8|rec_buffer[7];
        qDebug()<<x<<y;
        if(rec_buffer[3]==REPLY_CMD)
        {
            *reply_flag=true;
        }
        else if(rec_buffer[3]==SENDPOS_CMD)
        {
            trajectory_data.append(QCPCurveData(trajectory_data.size(),x,y));
            emit processed();
        }
    }
    isbusy=false;
}
