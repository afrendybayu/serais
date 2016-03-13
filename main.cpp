#include "serialportreader.h"
#include "log.h"

#include <QtSerialPort/QSerialPort>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QSettings>

QT_USE_NAMESPACE

struct setting {
    QString url;
    QString serial;
    bool logst;
    QString logfile;
};

struct setting cekSetting();

int main(int argc, char *argv[])    {
    QCoreApplication coreApplication(argc, argv);
    QSerialPort serialPort;
    QTextStream standardOutput(stdout);

    setting conf;
    conf = cekSetting();
    //cekSetting();

    Log ol(conf.logfile, conf.logst);

    serialPort.setPortName(conf.serial);

    //serialPort.setBaudRate(QSerialPort::Baud9600, QSerialPort::Input);

    if (!serialPort.open(QIODevice::ReadOnly)) {        //  | QIODevice::Unbuffered
        ol.tambahLog("GAGAL Buka serial "+ conf.serial);
        standardOutput << QObject::tr("Failed to open port tnt0, error: %1").arg(serialPort.errorString()) << endl;
        return 1;
    }

    ol.tambahLog("BERHASIL Buka serial "+ conf.serial);
    qDebug() << "berhasil buka serial /dev/tnt0" << endl;
    SerialPortReader serialPortReader(&serialPort, conf.url);

    return coreApplication.exec();
}

struct setting cekSetting() {
    QString pth = "setting.ini";
    QSettings sett(pth, QSettings::IniFormat);
    QString stl;
    setting m_conf;

    m_conf.url = sett.value("url").toString();
    m_conf.serial = sett.value("serial").toString();
    stl = sett.value("log").toString();
    m_conf.logfile = sett.value("logfile").toString();

    m_conf.url = m_conf.url.length()>0?m_conf.url:"http://localhost/ais/post.php";
    m_conf.serial = m_conf.serial.length()>0?m_conf.serial:"/dev/COM1";
    m_conf.logst = (stl=="true")?1:0;

    qDebug() << "Cek Setting";
    qDebug() << " URL :"<< m_conf.url;
    qDebug() << " ser :"<< m_conf.serial;

    return m_conf;
}
