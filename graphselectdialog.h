#ifndef _GRAPHSELECTDIALOG_INCLUDED
#define _GRAPHSELECTDIALOG_INCLUDED

#include <QtGui>
#include "ui_GraphSelectDialog.h"
#include "graph.h"

class GraphSelectDialog : public QDialog, Ui_GraphSelectDialog
{
	Q_OBJECT

	int	_lastGraph;
	
	QMap<int, QString>	_graphNames;

public:
	GraphSelectDialog(QWidget * parent = 0, Qt::WFlags f = 0);

	QString	graphName();	

public slots:

	void on_cancelButton_clicked();

	void on_e2pZone1Pv_clicked();
	void on_e2pZone2Pv_clicked();
	void on_e2pZone3Pv_clicked();
	void on_e2pZone4Pv_clicked();
	void on_e2pTracePv_clicked();

	void on_e2kZone1Pv_clicked();
	void on_e2kZone2Pv_clicked();
	void on_e2kZone3Pv_clicked();
	void on_e2kTracePv_clicked();

	void on_e3kZone1Pv_clicked();
	void on_e3kZone2Pv_clicked();
	void on_e3kZone3Pv_clicked();
	void on_e3kTracePv_clicked();

	void on_e3pZone1Pv_clicked();
	void on_e3pZone2Pv_clicked();
	void on_e3pZone3Pv_clicked();
	void on_e3pZone4Pv_clicked();
	void on_e3pTracePv_clicked();

	void on_e3mZone1Pv_clicked();
	void on_e3mZone2Pv_clicked();
	void on_e3mZone3Pv_clicked();
	void on_e3mTracePv_clicked();

	void on_sgZone1Pv_clicked();
	void on_sgZone2Pv_clicked();

	void on_e2pRotate_clicked();
	void on_e2kRotate_clicked();
	void on_e3kRotate_clicked();
	void on_e3pRotate_clicked();
	void on_e3mRotate_clicked();
	
	void on_tuSpeed_clicked();
	void on_sgSpeed_clicked();
	void on_sgRotate_clicked();

	void on_e2pPressure_clicked();
	void on_e2kPressure_clicked();
	void on_e3kPressure_clicked();
	void on_e3pPressure_clicked();
	void on_e3mPressure_clicked();

	void on_e2pCurrent_clicked();
	void on_e2kCurrent_clicked();
	void on_e3kCurrent_clicked();
	void on_e3pCurrent_clicked();
	void on_e3mCurrent_clicked();
	
	void on_tuCurrent_clicked();
	void on_sgCurrent_clicked();

	void on_uzgFreq_clicked();
	void on_uzgCurrent_clicked();
	void on_uzgPower_clicked();
	void on_uzgTemp_clicked();
};


#endif /* _GRAPHSELECTDIALOG_INCLUDED */

