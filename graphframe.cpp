#include "graphframe.h"
#include <QFrame>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QDateTime>

#include <QMessageBox>

void GraphFrame::setScalePos(int vScale, int vPos, int hScale, int hPos)
{
	_vScale = vScale;
	_vPos = vPos;
	_hScale = hScale;
	_hPos = hPos;
	
	update();
}
	
void GraphFrame::setGraph(int graph_id, const QList< QPair<QDateTime, double> > & data,
		const QDateTime & begin, const QDateTime & end, double min, double max)
{
	graphs[graph_id].data = data;
	graphs[graph_id].begin = begin;
	graphs[graph_id].end = end;
	graphs[graph_id].max = max;
	graphs[graph_id].min = min;

	update();
}
	
void GraphFrame::clearGraph(int graph_id)
{
	graphs[graph_id].data.clear();
	graphs[graph_id].begin = QDateTime::currentDateTime();
	graphs[graph_id].end = QDateTime::currentDateTime();
	graphs[graph_id].min = 0;
	graphs[graph_id].max = 0;
	
	update();
}

void GraphFrame::clearGraphs()
{
	clearGraph(RedGraph);
	clearGraph(GreenGraph);
	clearGraph(BlueGraph);
}
	
GraphFrame::GraphFrame(QWidget * parent, Qt::WFlags f) : QFrame(parent, f)
{
	bottom = 0;
	height = 1000;
	top = height + bottom;
	left = 0;
	width = 1440;
	right = left + width;

	_vScale = 1;
	_vPos = 0;
	_hScale = 1;
	_hPos = 0;
	
	for (int i = RedGraph; i < GraphCount; i++)
	{
		graphs[i].data.clear();
		graphs[i].begin = QDateTime::currentDateTime();
		graphs[i].end = QDateTime::currentDateTime();
		graphs[i].min = 0;
		graphs[i].max = 0;
	}
	
	graphColors[RedGraph] = QColor(170, 0, 0);
	graphColors[GreenGraph] = QColor(0, 85, 0);
	graphColors[BlueGraph] = QColor(0, 0, 127);
}

int	GraphFrame::xForDt(int graph_id, const QDateTime & dt)
{
	QDateTime dtBegin = graphs[graph_id].begin;
	int totalSec = dtBegin.secsTo(graphs[graph_id].end);
	int x = left + width * (dtBegin.secsTo(dt)) / totalSec;
	
	return x;
}

int	GraphFrame::yForValue(int graph_id, double value)
{
	double	totalAmp = graphs[graph_id].max - graphs[graph_id].min;
	int y =	(int)(bottom + height * value / totalAmp);

	return y;
}

void GraphFrame::paintEvent(QPaintEvent *)
{
	QPainter	p(this);
		
	QDateTime	dtBegin;

//	p.setWindow(left, bottom, width, height);
	
	int b = height * _vPos / _vScale / 2;
	int l = width * _hPos / _hScale / 2;
	int h = height /_vScale;
	int w = width / _hScale;

	double _penWidth = 2.;
	
	_penWidth /= (_vScale + _hScale) / 2;

	p.setWindow(l, b, w, h);
	p.scale(1, -1);
	p.translate(0, -2*b - h);

	p.setRenderHint(QPainter::Antialiasing);
//	p.setBackground(QBrush(QColor(255, 255, 255)));
	p.fillRect(p.window(), QBrush(QColor(255, 255, 255)));

	for (int i = RedGraph; i < GraphCount; i++)
	{
		if (graphs[i].data.empty())
			continue;
		
		if (graphs[i].max <= graphs[i].min)
			continue;

		if (graphs[i].end <= graphs[i].begin)
			continue;

		QPen pen;
		pen.setWidthF(_penWidth);
		pen.setColor(graphColors[i]);
		p.setPen(pen);
		
		QList< QPair<QDateTime, double> >::iterator b = graphs[i].data.begin();
		QList< QPair<QDateTime, double> >::iterator e = graphs[i].data.end();
		QList< QPair<QDateTime, double> >::iterator it;
		QList< QPair<QDateTime, double> >::iterator prev;
	
		for (prev = b, it = b, it++; it != e; it++, prev++)
		{
			if ((prev->first).secsTo(it->first) < 120)
				p.drawLine(xForDt(i, prev->first), yForValue(i, prev->second),
					xForDt(i, it->first), yForValue(i, it->second));
		}
	}

}
	
