#include "serialportreader.h"

#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QCoreApplication coreApplication(argc, argv);
    QSerialPort serialPort;
    QTextStream standardOutput(stdout);

    /*
    int argumentCount = QCoreApplication::arguments().size();
    QStringList argumentList = QCoreApplication::arguments();

    if (argumentCount == 1) {
        standardOutput << QObject::tr("Usage: %1 <serialportname> [baudrate]").arg(argumentList.first()) << endl;
        return 1;
    }


    QString serialPortName = argumentList.at(1);
    serialPort.setPortName(serialPortName);

    int serialPortBaudRate = (argumentCount > 2) ? argumentList.at(2).toInt() : QSerialPort::Baud9600;
    serialPort.setBaudRate(serialPortBaudRate);

    if (!serialPort.open(QIODevice::ReadOnly)) {
        standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
        return 1;
    }
    //*/

    serialPort.setPortName("/dev/tnt0");
    //serialPort.setBaudRate(QSerialPort::Baud9600, QSerialPort::Input);

    qDebug() << "mau buka serial" << endl;
    if (!serialPort.open(QIODevice::ReadOnly)) {        //  | QIODevice::Unbuffered
        standardOutput << QObject::tr("Failed to open port tnt0, error: %1").arg(serialPort.errorString()) << endl;
        return 1;
    }

    qDebug() << "berhasil buka serial /dev/tnt0" << endl;
    SerialPortReader serialPortReader(&serialPort);

    return coreApplication.exec();
}
