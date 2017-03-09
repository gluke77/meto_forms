#include <QDesktopWidget>
#include <QLineEdit>
#include <QString>
#include <QPushButton>
#include <QEvent>
#include <QPalette>
#include <QColor>
#include <QPoint>
#include <QTime>

#include "timeentry.h"
#include "keypad.h"

TimeEntry::TimeEntry(QWidget * parent) : QLineEdit(parent)
{
	_keypad = 0;
	_editMode = false;
	_palette = palette();
	_palette.setColor(QPalette::Base, Qt::white);
	_editPalette = _palette;
	_editPalette.setColor(QPalette::Base, Qt::cyan);
	setPalette(_palette);
	
	setValue(QTime::currentTime());
}

QTime TimeEntry::value() const
{
	return _value;
}

QTime TimeEntry::time() const
{
	return _value;
}

void TimeEntry::setValue(QTime value)
{
	_value = value;
	setText(_value.toString("hh:mm:ss"));
	update();
}
	
void TimeEntry::setValue(QTime & value)
{
	_value = value;
	setText(_value.toString("hh:mm:ss"));
	update();
}

void TimeEntry::setTime(QTime value)
{
	_value = value;
	setText(_value.toString("hh:mm:ss"));
	update();
}
	
void TimeEntry::setTime(QTime & value)
{
	_value = value;
	setText(_value.toString("hh:mm:ss"));
	update();
}

void TimeEntry::beginEdit()
{
	if (_editMode)
		return;
		
	_keypad = new KeyPad(this);
	
	_keypad->buttonDot->setText(":");
	_keypad->buttonDot->update();
	
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

	int newx = topLeft.x() + width() / 2 - keypadWidth / 2;
	int newy = topLeft.y() + height();


	if (newx < 0)
		newx = 0;

	if (newx + keypadWidth > screenWidth)
		newx = screenWidth - keypadWidth;

	if (newy + keypadHeight > screenHeight)
		newy = newy - height() - keypadHeight;

	
	_keypad->move(newx, newy);
	_keypad->show();	
		
	_editMode = true;
	
	setPalette(_editPalette);
	clear();
}

void TimeEntry::clear()
{
	setText("  :  :  ");
	_cursorPos = 0;
	_keypad->buttonDot->setEnabled(false);
	update();
}

void TimeEntry::back()
{
	if (0 == _cursorPos)
		return;
	
	QString txt = text();
	
	_cursorPos--;

	if (':' == txt[_cursorPos])
		_cursorPos--;
	
	if (5 < _cursorPos)
		_keypad->buttonDot->setEnabled(false);
	else
		_keypad->buttonDot->setEnabled(true);

	txt[_cursorPos] = ' ';
	setText(txt);
	update();
}

void TimeEntry::enterDigit()
{
	if (7 < _cursorPos)
		return;
		
	QPushButton * button = qobject_cast<QPushButton *>(sender());
	QChar chr = (button->text())[0];
	QString txt = text();
	
	txt[_cursorPos] = chr;
	setText(txt);	

	_cursorPos++;
	if ((2 == _cursorPos) || (5 == _cursorPos))
		_cursorPos++;

	if (5 < _cursorPos)
		_keypad->buttonDot->setEnabled(false);
	else
		_keypad->buttonDot->setEnabled(true);
			
	update();
}

void TimeEntry::enterDot()
{
	QString txt = text();
	
	switch (_cursorPos)
	{
	case 0:
		txt[0] = '0';
		txt[1] = '0';
		_cursorPos = 3;
		break;
	case 1:
		txt[1] = txt[0];
		txt[0] = '0';
		_cursorPos = 3;
		break;
	case 2:
		_cursorPos = 3;
		break;
	case 3:
		txt[3] = '0';
		txt[4] = '0';
		_cursorPos = 6;
		_keypad->buttonDot->setEnabled(false);
		break;
	case 4:
		txt[4] = txt[3];
		txt[3] = '0';
		_keypad->buttonDot->setEnabled(false);
		_cursorPos = 6;
		break;
	case 5:
		_keypad->buttonDot->setEnabled(false);
		_cursorPos = 6;
		break;
	default:
		_keypad->buttonDot->setEnabled(false);
		break;
	}
	
	setText(txt);
	update();
}

void TimeEntry::rejectChanges()
{
	setText(_value.toString("hh:mm:ss"));
	_editMode = false;
	clearFocus();
	setPalette(_palette);
	update();
}

void TimeEntry::acceptChanges()
{
	QTime oldValue = _value;

	QString txt = text();

	switch (_cursorPos)
	{
	case 0:
		txt[0] = '0';
		txt[1] = '0';
		txt[3] = '0';
		txt[4] = '0';
		txt[6] = '0';
		txt[7] = '0';
		break;
	case 1:
		txt[1] = txt[0];
		txt[0] = '0';
		txt[3] = '0';
		txt[4] = '0';
		txt[6] = '0';
		txt[7] = '0';
		break;
	case 2:
	case 3:
		txt[3] = '0';
		txt[4] = '0';
		txt[6] = '0';
		txt[7] = '0';
		break;
	case 4:
		txt[4] = txt[3];
		txt[3] = '0';
		txt[6] = '0';
		txt[7] = '0';
		break;
	case 5:
	case 6:
		txt[6] = '0';
		txt[7] = '0';
		break;
	case 7:
		txt[7] = txt[6];
		txt[6] = '0';
		break;
	}

	_value = QTime::fromString(txt, "hh:mm:ss");
	
	if (!_value.isValid())
		_value = oldValue;
		
	setText(_value.toString("hh:mm:ss"));
		
	_editMode = false;
	clearFocus();
	setPalette(_palette);

	if (oldValue != _value)
	{
		emit valueChanged();
	}
	
	update();
}

void TimeEntry::focusInEvent(QFocusEvent * ev)
{
	if (ev->gotFocus())
		beginEdit();
}

