#ifndef MENUITEM_H
#define MENUITEM_H
#include <QString>
#include <QList>
#include "submenuitem.h"

class MenuItem
{
public:
	QString title;
	QString condition;
	QList<SubMenuItem> subMenuList;
};


#endif // MENUITEM_H
