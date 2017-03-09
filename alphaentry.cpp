#include <QDesktopWidget>
#include <QLineEdit>
#include <QString>
#include <QPushButton>
#include <QEvent>
#include <QPalette>
#include <QColor>
#include <QPoint>

#include "alphaentry.h"
#include "keyboard.h"

AlphaEntry::AlphaEntry(QWidget * parent) : QLineEdit(parent)
{
	_keyboard = 0;
	_value = QString("");
	_editMode = false;
	
	_palette = palette();
	_palette.setColor(QPalette::Base, Qt::white);
	_editPalette = _palette;
	_editPalette.setColor(QPalette::Base, Qt::cyan);
	setPalette(_palette);
}

QString AlphaEntry::value() const
{
	return _value;
}

void AlphaEntry::setValue(QString & value)
{
	_value = value;
}
	
void AlphaEntry::beginEdit()
{
	if (_editMode)
		return;
		
	_keyboard = new KeyBoard(this);
	
	connect(_keyboard->buttonA, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonB, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonC, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonD, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonE, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonF, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonG, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonH, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonI, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonJ, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonK, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonL, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonM, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonN, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonO, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonP, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonQ, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonR, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonS, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonT, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonU, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonV, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonW, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonX, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonY, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonZ, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->button0, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->button1, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->button2, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->button3, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->button4, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->button5, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->button6, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->button7, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->button8, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->button9, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonMinus, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonPlus, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonSlash, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonBackslash, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonColon, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonSemicolon, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonDot, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonComma, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonUnderbar, SIGNAL(clicked()), this, SLOT(enterChar()));
	connect(_keyboard->buttonSpace, SIGNAL(clicked()), this, SLOT(enterChar()));

	connect(_keyboard->buttonBack, SIGNAL(clicked()), this, SLOT(back()));
	connect(_keyboard->buttonClear, SIGNAL(clicked()), this, SLOT(clear()));
	
	connect(_keyboard, SIGNAL(accepted()), this, SLOT(acceptChanges()));
	connect(_keyboard, SIGNAL(rejected()), this, SLOT(rejectChanges()));
	
	QPoint topLeft = mapToGlobal(QPoint(0, 0));
	
	int screenWidth = QApplication::desktop()->width();
	int screenHeight = QApplication::desktop()->height();

	int keyboardWidth = _keyboard->frameGeometry().width();
	int keyboardHeight = _keyboard->frameGeometry().height();

	int newx = topLeft.x() + width() / 2 - keyboardWidth / 2;
	int newy = topLeft.y() + height();


	if (newx < 0)
		newx = 0;

	if (newx + keyboardWidth > screenWidth)
		newx = screenWidth - keyboardWidth;

	if (newy + keyboardHeight > screenHeight)
		newy = newy - height() - keyboardHeight;

	
	_keyboard->move(newx, newy);
	_keyboard->show();	
	
	_editMode = true;
	
	setPalette(_editPalette);
	clear();
	update();
}

void AlphaEntry::back()
{
	backspace();
	update();
}

void AlphaEntry::enterChar()
{
	QPushButton * button = qobject_cast<QPushButton *>(sender());
	
	QString chr = button->text();
	
	if (chr.isEmpty())
		chr = QString(" ");
	
	bool shift = _keyboard->buttonLShift->isDown() || 
		_keyboard->buttonRShift->isDown();

	bool caps = _keyboard->buttonCaps->isChecked();

	if ((caps && !shift) || (!caps && shift))
		chr = chr.toUpper();

	setText(text() + chr);	
			
	update();
}

void AlphaEntry::rejectChanges()
{
	setText(_value);
	_editMode = false;
	clearFocus();
	setPalette(_palette);
	update();
}

void AlphaEntry::acceptChanges()
{
	QString oldValue = _value;

	_value = text();
	_editMode = false;
	clearFocus();
	setPalette(_palette);

	if (oldValue != _value)
	{
		emit valueChanged();
	}
	
	update();
}

void AlphaEntry::focusInEvent(QFocusEvent * ev)
{
	if (ev->gotFocus())
		beginEdit();
}

