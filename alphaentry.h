#ifndef _ALPHAENTRY_INCLUDED
#define _ALPHAENTRY_INCLUDED

#include <QLineEdit>
#include <QEvent>


#include "keyboard.h"

class QPalette;

class AlphaEntry : public QLineEdit
{
	Q_OBJECT

	QString		_value;
	KeyBoard	* _keyboard;
	bool		_editMode;
	QPalette	_palette;
	QPalette	_editPalette;

public:
	AlphaEntry(QWidget * parent = 0);

	QString value() const;
	void setValue(QString &);
	
protected:

	virtual void focusInEvent(QFocusEvent *);

protected slots:
	
	void beginEdit();

public slots:

	void enterChar();
	void rejectChanges();
	void acceptChanges();
	void back();
	
signals:

	void valueChanged();
};

#endif /* _ALPHAENTRY_INCLUDED */

