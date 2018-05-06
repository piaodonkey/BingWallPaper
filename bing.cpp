#include "bing.h"
#pragma comment(lib, "user32.lib")

Bing::Bing()
{
    connect(&manager,&QNetworkAccessManager::finished,this,&Bing::downloadFinished);

}

void Bing::execute()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(execute()));
    timer->setInterval(86400000);
    timer->start();

    QUrl url("https://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=en-US");

    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);
    currentReplys.append(reply);
    qDebug() <<  "execute ";
}
void Bing::doGetJson(QNetworkReply *reply)
{

    QByteArray byteArray = reply->readAll();

    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(byteArray,&jsonError);

    if (!json.isNull() && (jsonError.error == QJsonParseError::NoError))
    {

        if(json.isObject())
        {

            QJsonObject obj =json.object();
            if(obj.contains("images"))
            {

                QJsonValue value = obj.value("images");
                if(value.isArray())
                {

                    QJsonArray jsonArray = value.toArray();
                    QJsonValue imageValue = jsonArray.at(0);
                    if(imageValue.isObject())
                    {
                        QString image = imageValue["url"].toString();
                        QString baseUrl = "https://www.bing.com";
                        baseUrl.append(image);
                        QUrl url(baseUrl);
                        QNetworkRequest request(url);
                        QNetworkReply *replyImage = manager.get(request);
                        currentReplys.append(replyImage);
                    }
                }
            }
        }
    }

}

void Bing::downloadFinished(QNetworkReply *reply)
{

    if(reply->url() == QUrl("https://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=en-US"))
    {
        doGetJson(reply);
    }
    else
    {
        saveImages(reply);
    }

    currentReplys.removeAll(reply);
    reply->deleteLater();
}

void Bing::saveImages(QNetworkReply *reply)
{

    QDate date = QDate::currentDate();

    QString fileName = "d:\\bingWallPaper\\bing";
    fileName.append(date.toString("yyyyMMdd")).append(".jpg");
    QFile file(fileName);


    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "open failed";
        return;
    }
    file.write(reply->readAll());
    file.close();
    setWallPaper();
}
void Bing::setWallPaper()
{
    //const wchar_t *wimFile =reinterpret_cast<const wchar_t *>(pszWimFile.utf16());
    QDate date = QDate::currentDate();
    QString fileName = "d:\\bingWallPaper\\bing";
    fileName.append(date.toString("yyyyMMdd")).append(".jpg");
    const wchar_t *image =reinterpret_cast<const wchar_t *>(fileName.utf16());
    if( !SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)image, SPIF_UPDATEINIFILE) )//调用windows的API函

        return;

}

