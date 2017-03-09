#ifndef _TERMOFRAME_INCLUDED
#define _TERMOFRAME_INCLUDED

#include <QtGui>
#include <QColor>

#include "ui_TermoFrame.h"
#include "togglebutton.h"

#include "termo.h"

class TermoFrame : public QWidget, public Ui_TermoFrame
{
	Q_OBJECT

	Termo * _termo;
	
public:

	TermoFrame(QWidget * parent = 0);
	void connectTermo(Termo * t = 0);

public slots:

	void showDelta();
	void setLoCritBound(double);
	void setHiCritBound(double);

signals:

	void loCritBoundChanged(double);
	void hiCritBoundChanged(double);

};

#endif /* _TERMOFRAME_INCLUDED */
