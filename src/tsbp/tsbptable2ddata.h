/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef TSBPTABLE2DDATA_H
#define TSBPTABLE2DDATA_H
#include "table2ddata.h"

class TSBPTable2DData : public Table2DData
{
	Q_OBJECT
public:
	explicit TSBPTable2DData();
	//void setAxis(QByteArray data,int elementsize,float scale,float translate);
	//void setValues(QByteArray data, int elementsize,float scale, float translate);
	void setAxis(QByteArray data);
	void setValues(QByteArray data);
	void setData(unsigned short locationid,bool isflashonly,QByteArray payload){}
	void setMetaData(TableMeta metadata,FieldMeta xMeta,FieldMeta yMeta){}
	void setAxisMetaData(int pagenum,unsigned int offset, unsigned int totalsize,int elementsize,float scale,float translate,bool issigned);
	void setValueMetaData(int pagenum,unsigned int offset, unsigned int totalsize,int elementsize,float scale,float translate,bool issigned);
	QList<double> axis();
	QList<double> values();
	int columns() { return m_axis.size(); }
	int rows() { return 2; }
	int axisPage() { return m_axisPage; }
	int valuePage() { return m_valuePage; }

	void setData(unsigned short locationid,bool isflashonly,QByteArray payload,Table2DMetaData metadata,bool signedData);
	QByteArray data();
	double maxActualXAxis();
	double maxActualYAxis();
	double minActualYAxis();
	double minActualXAxis();
	double maxCalcedXAxis();
	double maxCalcedYAxis();
	double minCalcedXAxis();
	double minCalcedYAxis();
	void setAxisLabel(QString label) { m_axisLabel = label; }
	void setValueLabel(QString label) { m_valueLabel = label; }
	void setAxisVariable(QString label) { m_axisVariable = label; }
	void setValueVariable(QString label) { m_valueVariable = label; }

	QString axisLabel() { return m_axisLabel; }
	QString valueLabel() { return m_valueLabel; }

	void setCell(int row, int column,double newval);
	void setWritesEnabled(bool enabled);
	void writeWholeLocation(bool ram);
	bool isRam();
signals:
	void saveSingleData(unsigned short locationid,QByteArray data, unsigned short offset, unsigned short size);
	void burnPage(unsigned int page);
public slots:
	void saveRamToFlash();
	void updateFromFlash();
	void updateFromRam();
private:
	QString m_axisLabel;
	QString m_valueLabel;
	QString m_axisVariable;
	QString m_valueVariable;
	QByteArray axisdata;
	QByteArray valuedata;
	QList<double> m_values;
	QList<double> m_axis;
	float m_axisScale;
	float m_valueScale;
	float m_axisTranslate;
	float m_valueTranslate;
	int m_axisElementSize;
	int m_valueElementSize;
	bool m_axisSigned;
	bool m_valueSigned;
	int m_axisPage;
	int m_valuePage;
	unsigned int m_axisOffset;
	unsigned int m_axisSize;
	unsigned int m_valueOffset;
	unsigned int m_valueSize;

};

#endif // TSBPTABLE2DDATA_H
