/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#ifndef TSBPHEADERS_H
#define TSBPHEADERS_H
#include <QString>

#include <QPair>


class scalarclass
{
public:
	QString type;
	QString unit;
	unsigned int offset;
	unsigned int size;
	bool signedval;
	float scale;
	float translate;
};

class arrayclass
{
public:
	QString type;
	QString label;
	QString unit;
	unsigned int offset;
	unsigned int size;
	bool is2d;
	unsigned int xdim;
	unsigned int ydim;
	bool isSigned;
	bool isfloat;
	unsigned int elementSize;
	float scale;
	float translate;
	float min;
	float max;

};

class Table
{
public:
	QString title;
	QString mapid;
	QString xbin;
	QString ybin;
	QString zbin;
	QPair<QString,QString> upDownLabel;
	float gridHeight;
	int page;

};
class Curve
{
public:
	QString title;
	QPair<QString,QString> columnLabels;
	QString xbin;
	QString ybin;
	QString gauge;
	QString variable;

};

#endif // TSBPHEADERS_H
