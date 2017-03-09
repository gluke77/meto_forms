#ifndef _NUMERICEDIT_INCLUDED
#define _NUMERICEDIT_INCLUDED

#include <QLineEdit>
#include <QPalette>

class NumericEdit : public QLineEdit
{
	Q_OBJECT

	double		_value;
	int			_precision;

	QPalette	_basePalette;
	QPalette	_redPalette;
	QPalette	_greenPalette;

public:
	NumericEdit(QWidget * parent = 0);

	double value() const;
	
	void textToValue();
	void valueToText();
	
	int precision() const;
	void setPrecision(int);
	
public slots:

	void setValue(double);
	
	void setGreen();
	void setRed();
	void setWhite();
	
signals:

	void valueChanged();
	void valueChanged(double);

};

#endif /* _NUMERICEDIT_INCLUDED */

