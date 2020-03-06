/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef TSBPTable3DData_H
#define TSBPTable3DData_H
#include "table3ddata.h"
#include <QByteArray>

class TSBPTable3DData : public Table3DData
{
	Q_OBJECT
public:
	explicit TSBPTable3DData();

	void setXAxis(QByteArray data);
	void setYAxis(QByteArray data);
	void setValues(QByteArray data);
	void setXAxisMetaData(int pagenum,unsigned int offset, unsigned int totalsize,int elementsize,float scale,float translate,bool issigned,bool isfloat,float min, float max);
	void setYAxisMetaData(int pagenum,unsigned int offset, unsigned int totalsize,int elementsize,float scale,float translate,bool issigned,bool isfloat,float min, float max);
	void setValueMetaData(int pagenum,unsigned int offset, unsigned int totalsize,int elementsize,float scale,float translate,bool issigned,bool isfloat,unsigned short xdim,unsigned short ydim,float min, float max);
	void setMetaData(TableMeta metadata,FieldMeta xMeta,FieldMeta yMeta,FieldMeta zMeta) { }
	//void setData(unsigned short locationid,bool isflashonly,QByteArray payload) { }
	void setData(unsigned short locationid,bool isflashonly,QByteArray payload,Table3DMetaData metadata);
	void setData(unsigned short locationid,bool isflashonly,QByteArray payload);
	QByteArray data();
	QList<double> xAxis();
	QList<double> yAxis();
	QList<QList<double> > values();
	int columns();
	int rows();
	void setCell(int row, int column,double val);
	void setXAxis(int column,double val);
	void setYAxis(int row,double val);
	//double maxXAxis();
	//double maxYAxis();
	//double maxZAxis();
	//double minXAxis();
	//double minYAxis();
	//double minZAxis();
	double maxActualXAxis();
	double maxActualYAxis();
	double maxActualValue();
	double minActualYAxis();
	double minActualXAxis();
	double minActualValue();
	double maxCalcedXAxis();
	double maxCalcedYAxis();
	double maxCalcedValue();
	double minCalcedXAxis();
	double minCalcedYAxis();
	double minCalcedValue();
	void setWritesEnabled(bool enabled);
	void writeWholeLocation(bool ram);
	bool isRam();
	unsigned int xAxisPage() { return m_xAxisPage; }
	unsigned int yAxisPage() { return m_yAxisPage; }
	unsigned int valuePage() { return m_valuePage; }
private:
	QList<double> m_xAxis;
	QList<double> m_yAxis;
	QList<QList<double> > m_values;
	unsigned int m_xAxisPage;
	unsigned int m_yAxisPage;
	unsigned int m_valuePage;
	float m_xAxisScale;
	float m_yAxisScale;
	float m_valueScale;
	float m_xAxisTranslate;
	float m_yAxisTranslate;
	float m_valueTranslate;
	unsigned int m_xDim;
	unsigned int m_yDim;
	int m_xAxisElementSize;
	int m_yAxisElementSize;
	int m_valueElementSize;
	bool m_xAxisSigned;
	bool m_yAxisSigned;
	bool m_valueSigned;
	bool m_xAxisFloat;
	bool m_yAxisFloat;
	bool m_valueFloat;
	unsigned int m_yAxisOffset;
	unsigned int m_yAxisSize;
	unsigned int m_xAxisOffset;
	unsigned int m_xAxisSize;
	unsigned int m_valueOffset;
	unsigned int m_valueSize;

	double m_maxCalcedXAxis;
	double m_maxCalcedYAxis;
	double m_maxCalcedValue;

	double m_minCalcedXAxis;
	double m_minCalcedYAxis;
	double m_minCalcedValue;

	double m_maxActualXAxis;
	double m_maxActualYAxis;
	double m_maxActualValue;

	double m_minActualXAxis;
	double m_minActualYAxis;
	double m_minActualValue;
signals:
//	void saveSingleDataToRam(unsigned short locationid,unsigned short offset, unsigned short size,QByteArray data);
//	void saveSingleDataToFlash(unsigned short locationid,unsigned short offset, unsigned short size,QByteArray data);
	void saveSingleData(unsigned short page, QByteArray bytes,unsigned short offset,unsigned short size);
	void burnPage(unsigned int page);
	void update();
public slots:
	void saveRamToFlash();
	void updateFromFlash();
	void updateFromRam();
	
};

#endif // TSBPTable3DData_H
