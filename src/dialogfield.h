#ifndef DIALOGFIELD_H
#define DIALOGFIELD_H
#include <QString>

class DialogField
{
public:
	QString title; //Display name
	QString variable; //Ram variable it edits
	QString condition; //Condition on it being editable/displayed
};

#endif // DIALOGFIELD_H
