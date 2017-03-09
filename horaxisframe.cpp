#include "horaxisframe.h"
#include <QFrame>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QDateTime>

#include <QMessageBox>

HorAxisFrame::HorAxisFrame(QWidget * parent, Qt::WFlags f) : QFrame(parent, f)
{
	_begin = QDateTime::currentDateTime();
	_end = QDateTime::currentDateTime();
}

void HorAxisFrame::setAxis(const QDateTime & begin, const QDateTime & end)
{
	_begin = begin;
	_end = end;

	update();
}

void HorAxisFrame::paintEvent(QPaintEvent *)
{
	QPainter	p(this);
		
	int bottom = p.window().y();
	int height = p.window().height();
	int top = height + bottom;
	int left = p.window().x();
	int width = p.window().width();
	int right = left + width;

	QPen pen;
	pen.setWidth(2);
	p.setPen(pen);
	p.drawLine(left, bottom + 1, right, bottom + 1);
	pen.setWidth(1);
	p.setPen(pen);

	int totalSec = _begin.secsTo(_end);
	int totalMin = totalSec / 60;
	int minPerMark = totalMin / 6;
	int sec;
	int pos;
	int counter = 0;

	QDateTime	minBegin = _begin.addSecs(60 - _begin.time().second());
	
	for (QDateTime t = minBegin; t < _end; t = t.addSecs(60))
	{
		sec = _begin.secsTo(t);
		pos = left + width * sec / totalSec;
		
		if (0 == counter)
		{
			p.drawLine(pos, bottom, pos, bottom + 10);
			p.drawText(pos + 5, bottom + 20, t.time().toString("hh:mm"));
		}	
		else
			p.drawLine(pos, bottom, pos, bottom + 5);
			
		counter++;
		if (counter >= minPerMark)
			counter = 0;
	}

/*	
	if (_end > _begin)
	{
		int		pos;
		double	mark;
		
		for (int i = 0; i < 10; i++)
		{
			pos = bottom + (top - bottom) * i / 10;
			mark = _graph1Min + (_graph1Max - _graph1Min) * i / 10;
			
			p.drawLine(redPos, pos - 1, redPos + 10, pos - 1);
			p.drawText(redPos - 60, pos - 20, 55, 20, Qt::AlignRight, QString::number(mark, 'f', 2));
		}
	}
*/
}
	
