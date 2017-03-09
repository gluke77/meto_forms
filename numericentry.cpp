#include <QLineEdit>
#include <QString>
#include <QPushButton>
#include <QEvent>
#include <QPalette>
#include <QPoint>
#include <QDesktopWidget>
#include <QColor>

#include "numericentry.h"
#include "keypad.h"

NumericEntry::NumericEntry(QWidget * parent) : QLineEdit(parent)
{
	_keypad = 0;
	_value = 0;
	_digitsAfterDot = -1;
	_editMode = false;
	_precision = 1;
	_maxValue = 999999.9;

	
	_palette = palette();
	_palette.setColor(QPalette::Base, Qt::white);
	_editPalette = _palette;
	_editPalette.setColor(QPalette::Base, Qt::cyan);
	setPalette(_palette);
	_blinkPalette = _palette;
	_blinkPalette.setColor(QPalette::Base, Qt::yellow);
	
	_blinkable = false;
	_blink = false;
	_blinkLight = false;
	_timerId = 0;

	valueToText();
}

double NumericEntry::value() const
{
	return _value;
}

void NumericEntry::setValue(double value, bool emitSignals, bool emitWithValue)
{
	if (_editMode)
		return;
		
	_value = value;
	
	if (_value > _maxValue)
		_value = _maxValue;
	
	valueToText();
	
	if (emitSignals)
		emit valueChanged();
		
	if (emitWithValue)
		emit valueChanged(_value);
}
	
void NumericEntry::textToValue()
{
	bool ok;

	_value = text().toDouble(&ok);
	
	if (!ok)
	{
		_value = 0;
		valueToText();
	}
}

void NumericEntry::valueToText()
{
	if (_value > 0.)
		setText(QString::number(_value, 'f', _precision));
	else
		setText(QString::number(_value, 'f', 0));
}

void NumericEntry::beginEdit()
{
	if (_editMode)
		return;
	
	if (isBlink())
		stopBlink();
	
	_keypad = new KeyPad(this);
	
	connect(_keypad->button0, SIGNAL(clicked()), this, SLOT(enterDigit()));
	connect(_keypad->button1, SIGNAL(clicked()), this, SLOT(enterDigit()));
	connect(_keypad->button2, SIGNAL(clicked()), this, SLOT(enterDigit()));
	connect(_keypad->button3, SIGNAL(clicked()), this, SLOT(enterDigit()));
	connect(_keypad->button4, SIGNAL(clicked()), this, SLOT(enterDigit()));
	connect(_keypad->button5, SIGNAL(clicked()), this, SLOT(enterDigit()));
	connect(_keypad->button6, SIGNAL(clicked()), this, SLOT(enterDigit()));
	connect(_keypad->button7, SIGNAL(clicked()), this, SLOT(enterDigit()));
	connect(_keypad->button8, SIGNAL(clicked()), this, SLOT(enterDigit()));
	connect(_keypad->button9, SIGNAL(clicked()), this, SLOT(enterDigit()));
	
	connect(_keypad->buttonDot, SIGNAL(clicked()), this, SLOT(enterDot()));
	connect(_keypad->buttonBack, SIGNAL(clicked()), this, SLOT(back()));
	connect(_keypad->buttonClear, SIGNAL(clicked()), this, SLOT(clear()));
	
	connect(_keypad, SIGNAL(accepted()), this, SLOT(acceptChanges()));
	connect(_keypad, SIGNAL(rejected()), this, SLOT(rejectChanges()));
	
	QPoint topLeft = mapToGlobal(QPoint(0, 0));
	
	int screenWidth = QApplication::desktop()->width();
	int screenHeight = QApplication::desktop()->height();

	int keypadWidth = _keypad->frameGeometry().width();
	int keypadHeight = _keypad->frameGeometry().height();

	int newx = topLeft.x() + width();
	int newy = topLeft.y();

	if (newx + keypadWidth > screenWidth)
		newx = newx - keypadWidth - width();

	if (newy + keypadHeight > screenHeight)
		newy = screenHeight - keypadHeight;

	_keypad->move(newx, newy);
	_keypad->show();	
	
	_editMode = true;
	
	setPalette(_editPalette);
	clear();
	update();
}

void NumericEntry::enterDigit()
{
	QPushButton * button = qobject_cast<QPushButton *>(sender());
	
	QString digit = button->text();
	
	if ((QString("0") == text()) && (QString("0") == digit))
		return;
	
	if ((QString("0") == text()) && (QString("0") != digit))
	{
		setText(digit);
		_digitsAfterDot = -1;
	}
	else
		if (_digitsAfterDot < _precision)
		{
			setText(text() + digit);
			
			if (-1 < _digitsAfterDot)
				_digitsAfterDot++;
		}
			
	update();
}

void NumericEntry::enterDot()
{
	if (_digitsAfterDot < 0)
	{
		setText(text() + QString("."));
		_digitsAfterDot = 0;
	}
	update();
}

void NumericEntry::clear()
{
	setText(QString("0"));
	_digitsAfterDot = -1;
	update();
}

void NumericEntry::back()
{
	backspace();
	
	if (0 <= _digitsAfterDot)
		_digitsAfterDot--;
	
	if (text().isEmpty())
	{
		setText(QString("0"));
			_digitsAfterDot = -1;
	}
	update();
}

void NumericEntry::rejectChanges()
{
	valueToText();
	_editMode = false;
	clearFocus();
	setPalette(_palette);
	update();
}

void NumericEntry::acceptChanges()
{
	double oldValue = _value;

	textToValue();
	
	if (_value > _maxValue)
		_value = _maxValue;
	
	valueToText();
	_editMode = false;
	clearFocus();
	setPalette(_palette);

	if (oldValue != _value)
	{
		emit valueChanged();
		emit valueChanged(_value);
	}
	
	update();
}

void NumericEntry::focusInEvent(QFocusEvent * ev)
{
	if (ev->gotFocus())
		beginEdit();
}

void NumericEntry::setPrecision(int prec)
{
	_precision = (prec >= 0) ? prec : 0;
}

int NumericEntry::precision() const
{
	return _precision;
}

void NumericEntry::startBlink()
{
	setBlink(true);
	_cmdValue = value();
}
	
void NumericEntry::startBlink(double dumb)
{
	startBlink();
}
	
void NumericEntry::stopBlink()
{
	setBlink(false);
}
	
void NumericEntry::setBlink(bool blink)
{
	if (_blink)
	{
		killTimer(_timerId);
		_timerId = 0;
		_blinkLight = false;
		setPalette(_palette);
	}
		
	_blink = blink;
	
	if (_blink)
	{
		_timerId = startTimer(500);
		_blinkLight = true;
		setPalette(_blinkPalette);
	}
	update();
}
	
void NumericEntry::setBlinkable(bool blinkable)
{
	_blinkable = blinkable;
	
	if (_blinkable)
		connect(this, SIGNAL(valueChanged(double)), this, SLOT(startBlink(double)));
	else
	{
		disconnect(this, SIGNAL(valueChanged(double)), this, SLOT(startBlink(double)));
		stopBlink();
	}
}

#include <math.h>
#define EPS		(0.1)
	
void NumericEntry::setAlterValue(double aValue)
{
	_alterValue = aValue;
	
	if (isBlinkable())
	{
		if (fabs(_cmdValue - _alterValue) < EPS)
			stopBlink();
	}
	
	setValue(_alterValue);
}

void NumericEntry::timerEvent(QTimerEvent *event)
{
	if (_timerId != event->timerId())
		return;
		
	if (!isBlink())
		return;

	if (_blinkLight)
	{
		_blinkLight = false;
		setPalette(_palette);
	}
	else
	{
		_blinkLight = true;
		setPalette(_blinkPalette);
	}
	update();
}
