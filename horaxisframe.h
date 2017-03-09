#ifndef _HORAXISFRAME_INCLUDED
#define _HORAXISFRAME_INCLUDED

#include <QFrame>
#include <QPaintEvent>
#include <QDateTime>

class HorAxisFrame : public QFrame
{
	Q_OBJECT
	
	QDateTime	_begin;
	QDateTime	_end;

public:
	HorAxisFrame(QWidget * parent = 0, Qt::WFlags f = 0); 

public slots:
	void setAxis(const QDateTime & begin = QDateTime::currentDateTime(), const QDateTime & end = QDateTime::currentDateTime());

protected:

	void paintEvent(QPaintEvent *);
};




#endif /* _HORAXISFRAME_INCLUDED */
