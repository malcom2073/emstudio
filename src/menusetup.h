#ifndef MENUSETUP_H
#define MENUSETUP_H

#include <QList>
#include "menuitem.h"
#include "dialogitem.h"

class MenuSetup
{
public:
	QList<MenuItem> menulist;
	//QList<QPair<QString,QList<QString> > > menulist;
	//QMap<QString,QString> menunamemap;
	//QMap<QString,QList<QPair<QString,QString> > > dialogmap;
	QList<DialogItem> dialoglist;
};


#endif // MENUSETUP_H
