#include "log.h"

Log::Log(QString lf, bool ls, QObject *parent) :
    QObject(parent)         {
    m_lf = lf;
    m_ls = ls;
}

void Log::tambahLog(QString msg)   {
    QFile file(m_lf);
    QDateTime a = QDateTime::currentDateTime();
    QDateTime b = a.addMSecs(1000);
    qDebug( "%d", a.time().msecsTo( b.time() ) );

    //file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.open(QIODevice::Append);
    QTextStream out(&file);

    out << a.toString() << endl;
    out << msg << endl;
    out << "-----------------------------" << endl;
    file.close();
}
