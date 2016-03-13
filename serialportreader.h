
#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>


QT_USE_NAMESPACE

class SerialPortReader : public QObject {
    Q_OBJECT

public:
    SerialPortReader(QSerialPort *serialPort, QString url, QObject *parent = 0);
    ~SerialPortReader();

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);
    void handleResponsePost(QNetworkReply *);


private:
    QSerialPort *m_serialPort;
    QByteArray  m_readData;
    QTextStream m_standardOutput;
    QTimer      m_timer;
    QString     m_url;

    //QNetworkAccessManager *nm;      // definisi disini supaya no memory leakage

    void sendRequest();
};

#endif
