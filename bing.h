//#pragma execution_character_set("utf-8")

#ifndef BING_H
#define BING_H
#include "Windows.h"

#include <QtCore>
#include <QtNetwork>
//#include <QUrlQuery>

class Bing : public QObject
{
    Q_OBJECT
public:
    Bing();

private:
    QNetworkAccessManager manager;
    QVector<QNetworkReply *> currentReplys;
    void setWallPaper();
private slots:

    void doGetJson(QNetworkReply *reply);
    void downloadFinished(QNetworkReply *reply);
    void saveImages(QNetworkReply *reply);

signals:

public slots:
    void execute();

};

#endif // BING_H
