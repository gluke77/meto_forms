#ifndef _NUMERICENTRY_INCLUDED
#define _NUMERICENTRY_INCLUDED

#include <QLineEdit>
#include <QEvent>
#include <QTimer>

#include "keypad.h"

class QPalette;

class NumericEntry : public QLineEdit
{
	Q_OBJECT

	double		_value;
	KeyPad 		* _keypad;
	int			_digitsAfterDot;
	int			_precision;
	bool		_editMode;
	QPalette	_palette;
	QPalette	_editPalette;
	
	QPalette	_blinkPalette;
	
	int			_timerId;
	bool		_blink;
	bool		_blinkLight;
	bool		_blinkable;
	
	double		_alterValue;
	double		_cmdValue;	
	double		_maxValue;	

protected:

	void timerEvent(QTimerEvent *event);

public:
	NumericEntry(QWidget * parent = 0);
	
	virtual ~NumericEntry()
	{	
		if (_timerId) 
			killTimer(_timerId);
	};

	double value() const;
	
	void textToValue();
	void valueToText();
	
	int precision() const;
	void setPrecision(int);
	
	void emitValueChanged()
	{
		emit valueChanged(value());
	};
	
	bool isBlink()
	{
		return _blink;
	};
	
	bool isBlinkable()
	{
		return _blinkable;
	};
	
	double alterValue()
	{
		return _alterValue;
	};	
	
	void setMaxValue(double m)
	{
		_maxValue = m;
		if (_value > _maxValue)
		{	
			if (_editMode)
				return;
		
			_value = _maxValue;
			
			valueToText();
			emit valueChanged();
			emit valueChanged(_value);
		}
	};
	
	double maxValue()
	{
		return _maxValue;
	};
	
protected:

	virtual void focusInEvent(QFocusEvent *);

protected slots:
	
	void beginEdit();

public slots:

	void setValue(double value, bool emitSignals = true, bool emitWithValue = false);
	void enterDigit();
	void enterDot();
	void clear();
	void back();
	void rejectChanges();
	void acceptChanges();
	
	void startBlink();
	void startBlink(double);
	void stopBlink();
	void setBlink(bool blink = true);
	void setBlinkable(bool blinkable = true);
	void setAlterValue(double);	
	
signals:

	void valueChanged();
	void valueChanged(double);

};

#endif /* _NUMERICENTRY_INCLUDED */

