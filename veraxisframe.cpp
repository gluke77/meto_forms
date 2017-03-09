
#include "veraxisframe.h"
#include <QFrame>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>

VerAxisFrame::VerAxisFrame(QWidget * parent, Qt::WFlags f) : QFrame(parent, f)
{
	_graph1Min = 0;
	_graph1Max = 0;
	_graph2Min = 0;
	_graph2Max = 0;
	_graph3Min = 0;
	_graph3Max = 0;
		
	_green = QColor(0, 85, 0);
	_red = QColor(170, 0, 0);
	_blue = QColor(0, 0, 127);
}

void VerAxisFrame::setAxis(double min1, double max1, double min2, double max2, double min3, double max3)
{
	_graph1Min = min1;
	_graph2Min = min2;
	_graph3Min = min3;
		
	_graph1Max = max1;
	_graph2Max = max2;
	_graph3Max = max3;
		
	update();
}

void VerAxisFrame::paintEvent(QPaintEvent *)
{
	QPainter	p(this);
		
//	p.setWindow(0, 0, 300, 1000);
//	p.scale(1, -1);
//	p.translate(0, -1000);
	
	int top = p.window().y();
	int height = p.window().height();
	int bottom = height + top;
	int left = p.window().x();
	int width = p.window().width();
	int right = left + width;

	int redPos = left + 70 * width / 300;
	int greenPos = left + 170 * width / 300;
	int bluePos = left + 270 * width / 300;
		
	if (_graph1Max > _graph1Min)
	{
		QPen pen;
		pen.setCapStyle(Qt::RoundCap);
		pen.setBrush(QBrush(_red));
		pen.setWidth(2);

		p.setPen(pen);
		
		p.drawLine(redPos, bottom, redPos, top);
		p.drawLine(redPos, top, redPos - 5, top + 10);
		p.drawLine(redPos, top, redPos + 5, top + 10);
		
		pen.setCapStyle(Qt::SquareCap);
		p.setPen(pen);
		
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
		
	if (_graph2Max > _graph2Min)
	{
		QPen pen;
		pen.setCapStyle(Qt::RoundCap);
		pen.setBrush(QBrush(_green));
		pen.setWidth(2);

		p.setPen(pen);
		
		p.drawLine(greenPos, bottom, greenPos, top);
		p.drawLine(greenPos, top, greenPos - 5, top + 10);
		p.drawLine(greenPos, top, greenPos + 5, top + 10);
		
		pen.setCapStyle(Qt::SquareCap);
		p.setPen(pen);
		
		int		pos;
		double	mark;
		
		for (int i = 0; i < 10; i++)
		{
			pos = bottom + (top - bottom) * i / 10;
			mark = _graph2Min + (_graph2Max - _graph2Min) * i / 10;
			
			p.drawLine(greenPos, pos - 1, greenPos + 10, pos - 1);
			p.drawText(greenPos - 60, pos - 20, 55, 20, Qt::AlignRight, QString::number(mark, 'f', 2));
		}
	}

	if (_graph3Max > _graph3Min)
	{
		QPen pen;
		pen.setCapStyle(Qt::RoundCap);
		pen.setBrush(QBrush(_blue));
		pen.setWidth(2);

		p.setPen(pen);
		
		p.drawLine(bluePos, bottom, bluePos, top);
		p.drawLine(bluePos, top, bluePos - 5, top + 10);
		p.drawLine(bluePos, top, bluePos + 5, top + 10);
		
		pen.setCapStyle(Qt::SquareCap);
		p.setPen(pen);
		
		int		pos;
		double	mark;
		
		for (int i = 0; i < 10; i++)
		{
			pos = bottom + (top - bottom) * i / 10;
			mark = _graph3Min + (_graph3Max - _graph3Min) * i / 10;
			
			p.drawLine(bluePos, pos - 1, bluePos + 10, pos - 1);
			p.drawText(bluePos - 60, pos - 20, 55, 20, Qt::AlignRight, QString::number(mark, 'f', 2));
		}
	}
}
	
