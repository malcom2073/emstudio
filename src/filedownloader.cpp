#include "filedownloader.h"
#include <QTemporaryFile>
#include <QDebug>
#include <QSslSocket>
#include <QStandardPaths>
#include <QDir>
FileDownloader::FileDownloader()
{
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();
    qDebug() << QSslSocket::sslLibraryVersionString();
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));

    connect(&m_networkAccessManager, SIGNAL (finished(QNetworkReply*)),this, SLOT (replyReady(QNetworkReply*)));
}
void FileDownloader::replyReady(QNetworkReply* reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (!status_code.isValid())
    {
        emit fileDownloadFailed();
        return;
    }
    if (status_code.toInt() != 200)
    {
        //Error getting file
        qDebug() << "Error downloading file! Return code:" << status_code.toInt();
        emit fileDownloadFailed();
        return;
    }
    QStringList strsplit = m_destPath.split("/");
    QString rawfilename = strsplit[strsplit.length()-1];
    qDebug() << "File Downloaded!" <<QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/" + rawfilename;
    QFile *tempfile = new QFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/" + rawfilename);
    tempfile->open(QIODevice::ReadWrite);
    tempfile->write(reply->readAll());
    tempfile->close();
    emit fileDownloaded(tempfile);
}
void FileDownloader::downloadFile(QString url,QString destpath)
{
    m_destPath = url;
    qDebug() << "Downloading:" << url;
    QStringList strsplit = url.split("/");
    QString rawfilename = strsplit[strsplit.length()-1];
    if (QFile::exists(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/" + rawfilename))
    {
        QFile *file = new QFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/" + rawfilename);
        qDebug() << "File exists:" << QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/" + rawfilename;
        emit fileDownloaded(file);
        return;
    }
    QNetworkRequest request(url);
    m_networkAccessManager.get(request);
    //m_destPath = destpath;
}
