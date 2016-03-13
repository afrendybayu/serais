#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>

class Log : public QObject  {
    Q_OBJECT
public:
    explicit Log(QString logfile, bool st, QObject *parent = 0);
    void tambahLog(QString msg);
signals:

public slots:

private:
    QString m_lf;
    QString m_ls;
};

#endif // LOG_H
