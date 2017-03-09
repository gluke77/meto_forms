#include <QWidget>
#include <QColor>
#include <QFrame>
#include <QVBoxLayout>
#include <QMessageBox>

#include "led.h"

Led::Led(QWidget * parent, bool on, QColor onColor, QColor offColor)
	: QFrame(parent)
{
	_on = on;
	_onColor = onColor;
	_offColor = offColor;
	
//	setMinimumSize(32, 32);
	setFrameStyle(StyledPanel);
	setFrameShadow(Raised);
	
	_layout = new QVBoxLayout(this);
	_layout->setMargin(3);

	_innerFrame = new QFrame();
	_innerFrame->setFrameStyle(StyledPanel);
	_innerFrame->setFrameShadow(Sunken);
	_innerFrame->setAutoFillBackground(true);
	_layout->addWidget(_innerFrame);

	off();
}

bool Led::isOn() const
{
	return _on;
}
	
void Led::on()
{
	set(true);
}

void Led::off()
{
	set(false);
}

void Led::toggle()
{
	if (_on)
		set(false);
	else
		set(true);
}

void Led::set(bool on)
{
	bool oldOn;
	
	oldOn = on;
	_on = on;
	
	if (oldOn != _on)
	{
		emit setTo(_on);
		emit toggled();
	
		if (_on)
			emit setOn();
		else
			emit setOff();
	}
	
	QPalette basePalette = palette();

	if (on)
		basePalette.setColor(QPalette::Window, _onColor);
	else
		basePalette.setColor(QPalette::Window, _offColor);

	_innerFrame->setPalette(basePalette);
	
//	if ((width() < 32) || (height() < 32))
	{
		_layout->setMargin(0);
		_layout->setSpacing(0);
	}
//	else
//	{
//		_layout->setMargin(9);
//		_layout->setSpacing(6);
//	}
	
	update();
}		

void Led::setOnColor(const QColor & c)
{
	_onColor = c;
}

void Led::setOffColor(const QColor & c)
{
	_offColor = c;
}
