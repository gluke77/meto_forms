#ifndef _TOGGLEBUTTON_INCLUDED 
#define _TOGGLEBUTTON_INCLUDED

#include "QtGui"

class ToggleButton : public QPushButton
{
	Q_OBJECT
	
private:
	
	QPalette	_basePalette;
	QPalette	_onPalette;
	QPalette	_offPalette;
	QPalette	_blinkPalette;
	QString		_onText;
	QString		_offText;
	
	int			_timerId;
	bool		_blink;
	bool		_blinkLight;
	bool		_blinkable;
	
	bool		_alterChecked;
	bool		_cmdChecked;
	
protected:

	void timerEvent(QTimerEvent *event)
	{
		if (_timerId != event->timerId())
			return;
			
		if (!isBlink())
			return;
	
		if (_blinkLight)
		{
			_blinkLight = false;
			setup();
		}
		else
		{
			_blinkLight = true;
			setPalette(_blinkPalette);
			update();
		}
	};

	
public:

	ToggleButton(QWidget * parent = 0) : QPushButton(parent)
	{
		setCheckable(true);
		
		_basePalette = palette();
		_basePalette.setBrush(QPalette::Light, Qt::SolidPattern);
		setOffColor(Qt::green);
		setOnColor(Qt::red);
		setBlinkColor(Qt::yellow);
		setOffText(QString("Вкл"));
		setOnText(QString("Выкл"));
		
		_blink = false;
		_blinkLight = false;
		_timerId = 0;
		
		_alterChecked = isChecked();

		setup();

		connect(this, SIGNAL(toggled(bool)), this, SLOT(setup(bool)));
	};
	
	virtual ~ToggleButton()
	{
		if (_timerId) 
			killTimer(_timerId);
	};
	
	QString onText() const
	{
		return _onText;
	};
	
	QString offText() const
	{
		return _offText;
	};
	
	void setOnText(const QString & text)
	{
		_onText = text;
		setup();
	};
	
	void setOffText(const QString & text)
	{
		_offText = text;
		setup();
	};
	
	QColor onColor() const
	{
		return _onPalette.color(QPalette::Light);
	};
	
	QColor blinkColor() const
	{
		return _blinkPalette.color(QPalette::Button);
	};

	QColor offColor() const
	{
		return _offPalette.color(QPalette::Button);
	};
	
	void setOnColor(const QColor & color)
	{
		_onPalette = _basePalette;
		_onPalette.setColor(QPalette::Light, color);
		setup();
	};
	
	void setOffColor(const QColor & color)
	{
		_offPalette = _basePalette;
		_offPalette.setColor(QPalette::Button, color);
		setup();
	};
	
	void setBlinkColor(const QColor & color)
	{
		_blinkPalette = _basePalette;
		_blinkPalette.setColor(QPalette::Button, color);
		_blinkPalette.setColor(QPalette::Light, color);
	};
	
	bool isBlink()
	{
		return _blink;
	};
	
	bool isBlinkable()
	{
		return _blinkable;
	};
	
	bool isAlterChecked()
	{
		return _alterChecked;
	};
	
public slots:

	void setup(bool check)
	{
		if (check)
		{
			setText(_onText);
			setPalette(_onPalette);
		}
		else
		{
			setText(_offText);
			setPalette(_offPalette);
		}
		update();
	};
	
	void setup()
	{
		setup(isChecked());
	};
	
	void startBlink()
	{
		setBlink(true);
		_cmdChecked = isChecked();
	};
	
	void stopBlink()
	{
		setBlink(false);
	};
	
	void setBlink(bool blink = true)
	{
		if (_blink)
		{
			killTimer(_timerId);
			_timerId = 0;
			_blinkLight = false;
			setup();
		}
		
		_blink = blink;
		
		if (_blink)
		{
			_timerId = startTimer(500);
			_blinkLight = true;
			setPalette(_blinkPalette);
			update();
		}
	};
	
	void setBlinkable(bool blinkable = true)
	{
		_blinkable = blinkable;
		
		if (_blinkable)
			connect(this, SIGNAL(clicked()), this, SLOT(startBlink()));
		else
		{
			disconnect(this, SIGNAL(clicked()), this, SLOT(startBlink()));
			stopBlink();
		};
	
	};
	
	void setAlterChecked(bool alterChecked = true)
	{
		_alterChecked = alterChecked;
		
		if (isBlinkable())
		{
			if (_cmdChecked == _alterChecked)
				stopBlink();
		}
		
		setChecked(_alterChecked);
	};
/*	
signals:

	void toggledOn();
	void toggledOff();
*/
};

#endif /* _TOGGLEBUTTON_INCLUDED */
