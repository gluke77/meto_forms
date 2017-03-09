#ifndef _LED_INCLUDED
#define _LED_INCLUDED

//#include <QtGui>
#include <QWidget>
#include <QFrame>

class QVBoxLayout;
class QColor;

class Led : public QFrame
{
	Q_OBJECT
	
	bool	_on;
	
	QColor _onColor;
	QColor _offColor;
	QVBoxLayout * _layout;
	QFrame * _innerFrame;	

public:

	Led(QWidget * parent = 0, bool on = false, QColor onColor = Qt::red, QColor offColor = Qt::green);
	
	bool isOn() const;
	
	void setOnColor(const QColor &);
	void setOffColor(const QColor &);
	
public slots:

	void on();
	void off();
	void set(bool on = true);
	void toggle();
	
signals:

	void toggled();
	void setOn();
	void setOff();
	void setTo(bool);

};


#endif /* _LED_INCLUDED */
