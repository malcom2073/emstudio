/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef EBLPARSER_H
#define EBLPARSER_H
#include <QThread>
#include <QVariantMap>
class EBLParser : public QThread
{
	Q_OBJECT
public:
	EBLParser(QObject *parent=0);
	void loadLog(QString log);
signals:
	void loadProgress(quint64 pos,quint64 total);
	void done();
	void payloadDecoded(QVariantMap payload);
private:
	void parseEblPacket(QByteArray origpacket);
	QString m_fileName;
protected:
	void run();
};

#endif // EBLPARSER_H
