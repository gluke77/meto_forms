#ifndef _DATEENTRY_INCLUDED
#define _DATEENTRY_INCLUDED

#include <QLineEdit>
#include <QEvent>
#include <QDate>


#include "keypad.h"

class QPalette;

class DateEntry : public QLineEdit
{
	Q_OBJECT

	QDate		_value;
	KeyPad		* _keypad;
	bool		_editMode;
	QPalette	_palette;
	QPalette	_editPalette;
	int			_cursorPos;

public:
	DateEntry(QWidget * parent = 0);

	QDate value() const;
	QDate date() const;
	void setValue(QDate &);
	void setValue(QDate);
	void setDate(QDate &);
	void setDate(QDate);
	
protected:

	virtual void focusInEvent(QFocusEvent *);

protected slots:
	
	void beginEdit();

public slots:

	void enterDigit();
	void enterDot();
	void rejectChanges();
	void acceptChanges();
	void back();
	void clear();
	
signals:

	void valueChanged();
};

#endif /* _DATEENTRY_INCLUDED */

