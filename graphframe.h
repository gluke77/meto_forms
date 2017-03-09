#ifndef _GRAPHFRAME_INCLUDED
#define _GRAPHFRAME_INCLUDED

#include <QFrame>
#include <QPaintEvent>
#include <QDateTime>
#include <QList>
#include <QPair>

class GraphFrame : public QFrame
{
	Q_OBJECT
	
	int bottom;
	int height;
	int top;
	int left;
	int width;
	int right;
	
	int _vScale;
	int _vPos;
	int _hScale;
	int _hPos;

public:
	enum
	{
		RedGraph = 0,
		GreenGraph,
		BlueGraph,
		GraphCount
	};	

	struct Graph
	{
		QList< QPair<QDateTime, double> >	data;
		QDateTime							begin;
		QDateTime							end;
		double								min;
		double								max;
	} graphs[GraphCount];

	QColor graphColors[GraphCount];

	GraphFrame(QWidget * parent = 0, Qt::WFlags f = 0); 
	
	int	xForDt(int, const QDateTime &);
	int	yForValue(int, double);

public slots:
	void setScalePos(int vScale = 1, int vPos = 0, int hScale = 1, int hPos = 0);
	
	void setGraph(int graph_id, const QList< QPair<QDateTime, double> > & data,
		const QDateTime & begin, const QDateTime & end, double min, double max);
	
	void clearGraph(int graph_id);

	void clearGraphs();
	
protected:
	void paintEvent(QPaintEvent *);
};




#endif /* _GRAPHFRAME_INCLUDED */
