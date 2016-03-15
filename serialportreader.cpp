#include "serialportreader.h"

#include <QCoreApplication>

QT_USE_NAMESPACE

SerialPortReader::SerialPortReader(QSerialPort *serialPort, QString url, QObject *parent)
    : QObject(parent)
    , m_serialPort(serialPort)
    , m_standardOutput(stdout)
{
    connect(m_serialPort, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
    connect(&m_timer, SIGNAL(timeout()), SLOT(handleTimeout()));

    m_url = url;
    m_timer.start(1000);
}

SerialPortReader::~SerialPortReader()   {
    m_standardOutput << QObject::tr("Keluar aplikasi, destructor") << endl;
    QCoreApplication::quit();
}

void SerialPortReader::handleReadyRead()    {
    QByteArray ba = m_serialPort->readAll();
    ba.replace(QByteArray("\r"), QByteArray(""));
    ba.replace(QByteArray("\n"), QByteArray(""));
    m_readData.append(ba);

    if (!m_timer.isActive())
        m_timer.start(5000);
}

// dipakai untuk asyncronous request
void SerialPortReader::handleResponsePost(QNetworkReply *res) {
    if (res->error() == QNetworkReply::NoError) {
        //success
        qDebug() << "handleResponsePost Success" << res->readAll();
        //delete reply;
    }

}

// QNetworkRequest pakai cara syncrounous, hati2 dengan growing thread
void SerialPortReader::sendRequest()  {
    QEventLoop ev;
    //QNetworkRequest request = QNetworkRequest(QUrl("http://localhost/ais/post.php"));
    QNetworkRequest request = QNetworkRequest(QUrl(m_url));
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    /*
    request.setRawHeader("User-Agent", "My app name v0.1");
    request.setRawHeader("X-Custom-User-Agent", "My app name v0.1");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", postDataSize);
    //*/

    /*
    postDatax.append("string=");
    postDatax.append(m_readData.toBase64());
    //*/
    m_postU = 0;

    //QString jsonStr = "{\"ais\":\""+m_readData.toBase64()+"\",\"postU\":\""+m_postU+"\"}";
    QString jsonStr = "{\"ais\":\""+m_readData.toBase64()+"\",\"postU\":\""+QString::number(m_postU)+"\"}";
    //QByteArray jsonStr;// = "{\"method\":\"AuthenticatePlain\",\"loginName\":\"username@domain.com\",\"password\":\"mypass\"}";
    QByteArray jsonBA;
    jsonBA.append(jsonStr);

    QByteArray postDataSize = QByteArray::number(jsonBA.size());
    qDebug() << "isi json: " << jsonStr;

    // For your "Content-Length" header
    request.setHeader(QNetworkRequest::UserAgentHeader,"ABAdhy");
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, postDataSize);


    QNetworkAccessManager nm;// = new QNetworkAccessManager(this);
    connect(&nm, SIGNAL(finished(QNetworkReply*)), &ev, SLOT(quit()));

    if(m_readData.isEmpty())    {
        qDebug() << "tidak ada data";
        return;
    }

    QNetworkReply *reply = nm.post(request, jsonBA);
    ev.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Success" <<reply->readAll();
    }
    else {      //failure
        qDebug() << "Failure" <<reply->errorString();
    }

    //delete nm;
    delete reply;
    //qDebug() << "selesai";
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
