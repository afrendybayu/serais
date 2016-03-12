#include "serialportreader.h"

#include <QCoreApplication>

QT_USE_NAMESPACE

SerialPortReader::SerialPortReader(QSerialPort *serialPort, QObject *parent)
    : QObject(parent)
    , m_serialPort(serialPort)
    , m_standardOutput(stdout)
{
    connect(m_serialPort, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
    connect(&m_timer, SIGNAL(timeout()), SLOT(handleTimeout()));

    // gak bisa klo bikin signal slot network disini !!
    //connect(nm, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleResponsePost(QNetworkReply*)));
    m_timer.start(5000);
}

SerialPortReader::~SerialPortReader()   {
    m_standardOutput << QObject::tr("Keluar aplikasi, destructor") << endl;
    QCoreApplication::quit();
}

void SerialPortReader::handleReadyRead()    {
    m_readData.append(m_serialPort->readAll());

    if (!m_timer.isActive())
        m_timer.start(5000);
}

void SerialPortReader::handleResponsePost(QNetworkReply *res) {
    qDebug() << "handleResponsePost Success" << res->readAll();
}

void SerialPortReader::sendRequest()  {
    QEventLoop eventLoop;
    QNetworkRequest request = QNetworkRequest(QUrl("http://localhost/ais/post.php"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postDatax;
    postDatax.append("string=");
    postDatax.append(m_readData);

    //*
    //QNetworkAccessManager *nm = new QNetworkAccessManager(this);
    //connect(nm, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleResponsePost(QNetworkReply*)));

    if(!m_readData.isEmpty())    {
        nm->post(request, postDatax);
    }
    else {
        qDebug() << "tidak ada data";
    }

    //eventLoop.exec(); // blocks stack until "finished()" has been called

    /*
    if (reply->error() == QNetworkReply::NoError) {
        //success
        qDebug() << "Success" <<reply->readAll();
        delete reply;
    }
    //*/
}

void SerialPortReader::handleTimeout()  {
    if (m_readData.isEmpty()) {
        //m_standardOutput << QObject::tr("No data was currently available for reading from port %1").arg(m_serialPort->portName()) << endl;
    } else {
        //m_standardOutput << QObject::tr("Data successfully received from port %1").arg(m_serialPort->portName()) << endl;
        m_standardOutput << m_readData << endl;
        sendRequest();
        m_readData.clear();
    }

    //QCoreApplication::quit();
}

void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)    {
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}
