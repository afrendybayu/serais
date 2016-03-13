#include "serialportreader.h"

#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QSettings>

QT_USE_NAMESPACE

struct setting {
    QString url;
    QString serial;
};


struct setting cekSetting();
//void cekSetting();

int main(int argc, char *argv[])    {
    QCoreApplication coreApplication(argc, argv);
    QSerialPort serialPort;
    QTextStream standardOutput(stdout);

    setting conf;
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

    conf = cekSetting();
    //cekSetting();

    serialPort.setPortName(conf.serial);
    //serialPort.setPortName(m_conf.serial);
    //serialPort.setBaudRate(QSerialPort::Baud9600, QSerialPort::Input);

    if (!serialPort.open(QIODevice::ReadOnly)) {        //  | QIODevice::Unbuffered
        standardOutput << QObject::tr("Failed to open port tnt0, error: %1").arg(serialPort.errorString()) << endl;
        return 1;
    }

    qDebug() << "berhasil buka serial /dev/tnt0" << endl;
    SerialPortReader serialPortReader(&serialPort, conf.url);

    return coreApplication.exec();
}

struct setting cekSetting() {
//void cekSetting() {
    QString pth = "setting.ini";
    QSettings sett(pth, QSettings::IniFormat);

    setting m_conf;

    m_conf.url = sett.value("url").toString();
    m_conf.serial = sett.value("serial").toString();

    m_conf.url = m_conf.url.length()>0?m_conf.url:"http://localhost/ais/post.php";
    m_conf.serial = m_conf.serial.length()>0?m_conf.serial:"/dev/COM1";

    qDebug() << "Cek Setting";
    qDebug() << " URL :"<< m_conf.url;
    qDebug() << " ser :"<< m_conf.serial;

    return m_conf;
}
