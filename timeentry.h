#ifndef _TIMEENTRY_INCLUDED
#define _TIMEENTRY_INCLUDED

#include <QLineEdit>
#include <QEvent>
#include <QTime>


#include "keypad.h"

class QPalette;

class TimeEntry : public QLineEdit
{
	Q_OBJECT

	QTime		_value;
	KeyPad		* _keypad;
	bool		_editMode;
	QPalette	_palette;
	QPalette	_editPalette;
	int			_cursorPos;

public:
	TimeEntry(QWidget * parent = 0);

	QTime value() const;
	QTime time() const;
	void setValue(QTime &);
	void setValue(QTime);
	void setTime(QTime &);
	void setTime(QTime);
	
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

#endif /* _TIMEENTRY_INCLUDED */

