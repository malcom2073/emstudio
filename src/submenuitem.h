#ifndef SUBMENUITEM_H
#define SUBMENUITEM_H

#include <QString>

class SubMenuItem
{
public:
	QString variable;
	QString title;
	QString parent;
	QString var;
	QString param;
	QString condition;
	bool is_seperator;
};


#endif // SUBMENUITEM_H
