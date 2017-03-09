#include <QLineEdit>
#include <QString>
#include <QColor>
#include "numericedit.h"


NumericEdit::NumericEdit(QWidget * parent) : QLineEdit(parent)
{
	_value = 0;
	_precision = 1;
	
	_basePalette = palette();
	_greenPalette = _basePalette;
	_greenPalette.setColor(QPalette::Base, Qt::green);
	_redPalette = _basePalette;
	_redPalette.setColor(QPalette::Base, Qt::red);
	
	valueToText();
}

void NumericEdit::setRed()
{
	setPalette(_redPalette);
	update();
}

void NumericEdit::setGreen()
{
	setPalette(_greenPalette);
	update();
}

void NumericEdit::setWhite()
{
	setPalette(_basePalette);
	update();
}

double NumericEdit::value() const
{
	return _value;
}

void NumericEdit::setValue(double value)
{
	_value = value;
	valueToText();
	emit valueChanged();
	emit valueChanged(value);
}
	
void NumericEdit::textToValue()
{
	bool ok;

	_value = text().toDouble(&ok);
	
	if (!ok)
	{
		_value = 0;
		valueToText();
	}
}

void NumericEdit::valueToText()
{
	setText(QString::number(_value, 'f', _precision));
}

void NumericEdit::setPrecision(int prec)
{
	_precision = (prec >= 0) ? prec : 0;
}

int NumericEdit::precision() const
{
	return _precision;
}
