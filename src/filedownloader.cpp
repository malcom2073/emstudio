#include "filedownloader.h"
#include <QTemporaryFile>
#include <QDebug>
#include <QSslSocket>
FileDownloader::FileDownloader()
{
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();
    qDebug() << QSslSocket::sslLibraryVersionString();

    connect(&m_networkAccessManager, SIGNAL (finished(QNetworkReply*)),this, SLOT (replyReady(QNetworkReply*)));
}
void FileDownloader::replyReady(QNetworkReply* reply)
{
    qDebug() << "File Downloaded!";
    QTemporaryFile *tempfile = new QTemporaryFile();
    tempfile->open();
    tempfile->write(reply->readAll());
    tempfile->close();
    emit fileDownloaded(tempfile);
}
void FileDownloader::downloadFile(QString url,QString destpath)
{
    qDebug() << "Downloading:" << url;
    QNetworkRequest request(url);
    m_networkAccessManager.get(request);
    m_destPath = destpath;
}
