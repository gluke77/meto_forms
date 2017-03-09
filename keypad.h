#ifndef _KEYPAD_INCLUDED
#define _KEYPAD_INCLUDED


#include <QtGui>

#include "ui_KeypadDialog.h"

class KeyPad : public QDialog, public Ui_KeypadDialog
{
	Q_OBJECT
	
public:
	KeyPad(QWidget * parent = 0, Qt::WFlags f = 0) :
		QDialog(parent, f)
	{
		setupUi(this);
	}
};

#endif /* _KEYPAD_INCLUDED */

