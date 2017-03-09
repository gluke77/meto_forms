#ifndef _VERAXISFRAME_INCLUDED
#define _VERAXISFRAME_INCLUDED

#include <QFrame>
#include <QColor>
#include <QPaintEvent>

class VerAxisFrame : public QFrame
{
	Q_OBJECT
	
	double _graph1Min;
	double _graph1Max;
	double _graph2Min;
	double _graph2Max;
	double _graph3Min;
	double _graph3Max;
	
	QColor	_green;
	QColor	_red;
	QColor	_blue;

public:
	VerAxisFrame(QWidget * parent = 0, Qt::WFlags f = 0); 

public slots:
	void setAxis(double min1 = 0, double max1 = 0, double min2 = 0, double max2 = 0, double min3 = 0, double max3 = 0);

protected:

	void paintEvent(QPaintEvent *);
};


#endif /* _VERAXISFRAME_INCLUDED */
