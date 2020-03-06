#ifndef DIALOGITEM_H
#define DIALOGITEM_H

#include "dialogfield.h"
#include <QString>
#include <QList>
class DialogItem
{
public:
	QString title; //Display name
	QString variable; //What a menu item points to
	QList<DialogField> fieldList;
	QList<QPair<QString,QString> > panelList;
	QMap<QString,DialogItem> panelMap;
};


#endif // DIALOGITEM_H
