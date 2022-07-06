#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QFile>
class FileDownloader : public QObject
{
    Q_OBJECT
public:
    FileDownloader();
    void downloadFile(QString url,QString destpath);
signals:
    void fileDownloaded(QFile *file);
    void fileDownloadFailed();
private slots:
    void replyReady(QNetworkReply* pReply);
private:
    QNetworkAccessManager m_networkAccessManager;
    QByteArray m_DownloadedData;
    QString m_destPath;
};

#endif // FILEDOWNLOADER_H
