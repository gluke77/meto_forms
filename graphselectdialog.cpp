#include <QtGui>
#include "graphselectdialog.h"
#include "graph.h"


GraphSelectDialog::GraphSelectDialog(QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);
	
	_lastGraph = Graph::Select;
	
	_graphNames[Graph::Select] = QString("Выбрать");
	
	_graphNames[Graph::E2pZone1Pv] = QString("ЭП внутр.\nзона1\nТ, °С");
	_graphNames[Graph::E2pZone2Pv] = QString("ЭП внутр.\nзона2\nТ, °С");
	_graphNames[Graph::E2pZone3Pv] = QString("ЭП внутр.\nзона3\nТ, °С");
	_graphNames[Graph::E2pZone4Pv] = QString("ЭП внутр.\nзона4\nТ, °С");
	_graphNames[Graph::E2pTracePv] = QString("ЭП внутр.\nтрасса\nТ, °С");

	_graphNames[Graph::E2kZone1Pv] = QString("ЭК внутр.\nзона1\nТ, °С");
	_graphNames[Graph::E2kZone2Pv] = QString("ЭК внутр.\nзона2\nТ, °С");
	_graphNames[Graph::E2kZone3Pv] = QString("ЭК внутр.\nзона3\nТ, °С");
	_graphNames[Graph::E2kTracePv] = QString("ЭК внутр.\nтрасса\nТ, °С");

	_graphNames[Graph::E3kZone1Pv] = QString("ЭК нар.\nзона1\nТ, °С");
	_graphNames[Graph::E3kZone2Pv] = QString("ЭК нар.\nзона2\nТ, °С");
	_graphNames[Graph::E3kZone3Pv] = QString("ЭК нар.\nзона3\nТ, °С");
	_graphNames[Graph::E3kTracePv] = QString("ЭК нар.\nтрасса\nТ, °С");

	_graphNames[Graph::E3pZone1Pv] = QString("ЭП нар.\nзона1\nТ, °С");
	_graphNames[Graph::E3pZone2Pv] = QString("ЭП нар.\nзона2\nТ, °С");
	_graphNames[Graph::E3pZone3Pv] = QString("ЭП нар.\nзона3\nТ, °С");
	_graphNames[Graph::E3pZone4Pv] = QString("ЭП нар.\nзона4\nТ, °С");
	_graphNames[Graph::E3pTracePv] = QString("ЭП нар.\nтрасса\nТ, °С");

	_graphNames[Graph::E3mZone1Pv] = QString("ЭМ нар.\nзона1\nТ, °С");
	_graphNames[Graph::E3mZone2Pv] = QString("ЭМ нар.\nзона2\nТ, °С");
	_graphNames[Graph::E3mZone3Pv] = QString("ЭМ нар.\nзона3\nТ, °С");
	_graphNames[Graph::E3mTracePv] = QString("ЭМ нар.\nтрасса\nТ, °С");

	_graphNames[Graph::SgZone1Pv] = QString("ЭГ\nзона1\nТ, °С");
	_graphNames[Graph::SgZone2Pv] = QString("ЭГ\nзона2\nТ, °С");

	_graphNames[Graph::E2pRotate] = QString("ЭП внутр.\ncкорость,\nоб/мин");
	_graphNames[Graph::E2kRotate] = QString("ЭК внутр.\ncкорость,\nоб/мин");
	_graphNames[Graph::E3kRotate] = QString("ЭК нар.\ncкорость,\nоб/мин");
	_graphNames[Graph::E3pRotate] = QString("ЭП нар.\ncкорость,\nоб/мин");
	_graphNames[Graph::E3mRotate] = QString("ЭМ нар.\ncкорость,\nоб/мин");

	_graphNames[Graph::SgRotate] = QString("СГ\ncкорость,\nоб/мин");
	_graphNames[Graph::SgSpeed] = QString("СГ\ncкорость,\nм/мин");
	_graphNames[Graph::TuSpeed] = QString("ТУ\ncкорость,\nм/мин");

	_graphNames[Graph::E2pCurrent] = QString("ЭП внутр.\nток, А");
	_graphNames[Graph::E2kCurrent] = QString("ЭК внутр.\nток, А");
	_graphNames[Graph::E3kCurrent] = QString("ЭК нар.\nток, А");
	_graphNames[Graph::E3pCurrent] = QString("ЭП нар.\nток, А");
	_graphNames[Graph::E3mCurrent] = QString("ЭМ нар.\nток, А");

	_graphNames[Graph::SgCurrent] = QString("СГ\nток, А");
	_graphNames[Graph::TuCurrent] = QString("ТУ\nток, А");

	_graphNames[Graph::E2pPressure] = QString("ЭП внутр.\nдавление,\nбар");
	_graphNames[Graph::E2kPressure] = QString("ЭК внутр.\nдавление,\nбар");
	_graphNames[Graph::E3kPressure] = QString("ЭК нар.\nдавление,\nбар");
	_graphNames[Graph::E3pPressure] = QString("ЭП нар.\nдавление,\nбар");
	_graphNames[Graph::E3mPressure] = QString("ЭМ нар.\nдавление,\nбар");

	_graphNames[Graph::UzgFreq] = QString("УЗГ\nчастота,\nГц");
	_graphNames[Graph::UzgCurrent] = QString("УЗГ\nток, А");
	_graphNames[Graph::UzgPower] = QString("УЗГ\nмощность,\nВт");
	_graphNames[Graph::UzgTemp] = QString("УЗГ\nТ, °С");

}

QString	GraphSelectDialog::graphName()
{
		return _graphNames[_lastGraph];
}

void GraphSelectDialog::on_cancelButton_clicked()
{
	_lastGraph = Graph::Select;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2pZone1Pv_clicked()
{
	_lastGraph = Graph::E2pZone1Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2pZone2Pv_clicked()
{
	_lastGraph = Graph::E2pZone2Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2pZone3Pv_clicked()
{
	_lastGraph = Graph::E2pZone3Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2pZone4Pv_clicked()
{
	_lastGraph = Graph::E2pZone4Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2pTracePv_clicked()
{
	_lastGraph = Graph::E2pTracePv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2kZone1Pv_clicked()
{
	_lastGraph = Graph::E2kZone1Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2kZone2Pv_clicked()
{
	_lastGraph = Graph::E2kZone2Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2kZone3Pv_clicked()
{
	_lastGraph = Graph::E2kZone3Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2kTracePv_clicked()
{
	_lastGraph = Graph::E2kTracePv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3kZone1Pv_clicked()
{
	_lastGraph = Graph::E3kZone1Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3kZone2Pv_clicked()
{
	_lastGraph = Graph::E3kZone2Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3kZone3Pv_clicked()
{
	_lastGraph = Graph::E3kZone3Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3kTracePv_clicked()
{
	_lastGraph = Graph::E3kTracePv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3pZone1Pv_clicked()
{
	_lastGraph = Graph::E3pZone1Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3pZone2Pv_clicked()
{
	_lastGraph = Graph::E3pZone2Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3pZone3Pv_clicked()
{
	_lastGraph = Graph::E3pZone3Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3pZone4Pv_clicked()
{
	_lastGraph = Graph::E3pZone4Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3pTracePv_clicked()
{
	_lastGraph = Graph::E3pTracePv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3mZone1Pv_clicked()
{
	_lastGraph = Graph::E3mZone1Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3mZone2Pv_clicked()
{
	_lastGraph = Graph::E3mZone2Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3mZone3Pv_clicked()
{
	_lastGraph = Graph::E3mZone3Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3mTracePv_clicked()
{
	_lastGraph = Graph::E3mTracePv;
	done(_lastGraph);
}

void GraphSelectDialog::on_sgZone1Pv_clicked()
{
	_lastGraph = Graph::SgZone1Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_sgZone2Pv_clicked()
{
	_lastGraph = Graph::SgZone2Pv;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2pRotate_clicked()
{
	_lastGraph = Graph::E2pRotate;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2kRotate_clicked()
{
	_lastGraph = Graph::E2kRotate;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3kRotate_clicked()
{
	_lastGraph = Graph::E3kRotate;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3pRotate_clicked()
{
	_lastGraph = Graph::E3pRotate;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3mRotate_clicked()
{
	_lastGraph = Graph::E3mRotate;
	done(_lastGraph);
}

void GraphSelectDialog::on_tuSpeed_clicked()
{
	_lastGraph = Graph::TuSpeed;
	done(_lastGraph);
}

void GraphSelectDialog::on_sgSpeed_clicked()
{
	_lastGraph = Graph::SgSpeed;
	done(_lastGraph);
}

void GraphSelectDialog::on_sgRotate_clicked()
{
	_lastGraph = Graph::SgRotate;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2pCurrent_clicked()
{
	_lastGraph = Graph::E2pCurrent;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2kCurrent_clicked()
{
	_lastGraph = Graph::E2kCurrent;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3kCurrent_clicked()
{
	_lastGraph = Graph::E3kCurrent;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3pCurrent_clicked()
{
	_lastGraph = Graph::E3pCurrent;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3mCurrent_clicked()
{
	_lastGraph = Graph::E3mCurrent;
	done(_lastGraph);
}

void GraphSelectDialog::on_tuCurrent_clicked()
{
	_lastGraph = Graph::TuCurrent;
	done(_lastGraph);
}

void GraphSelectDialog::on_sgCurrent_clicked()
{
	_lastGraph = Graph::SgCurrent;
	done(_lastGraph);
}

void GraphSelectDialog::on_uzgFreq_clicked()
{
	_lastGraph = Graph::UzgFreq;
	done(_lastGraph);
}

void GraphSelectDialog::on_uzgCurrent_clicked()
{
	_lastGraph = Graph::UzgCurrent;
	done(_lastGraph);
}

void GraphSelectDialog::on_uzgPower_clicked()
{
	_lastGraph = Graph::UzgPower;
	done(_lastGraph);
}

void GraphSelectDialog::on_uzgTemp_clicked()
{
	_lastGraph = Graph::UzgTemp;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2pPressure_clicked()
{
	_lastGraph = Graph::E2pPressure;
	done(_lastGraph);
}

void GraphSelectDialog::on_e2kPressure_clicked()
{
	_lastGraph = Graph::E2kPressure;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3kPressure_clicked()
{
	_lastGraph = Graph::E3kPressure;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3pPressure_clicked()
{
	_lastGraph = Graph::E3pPressure;
	done(_lastGraph);
}

void GraphSelectDialog::on_e3mPressure_clicked()
{
	_lastGraph = Graph::E3mPressure;
	done(_lastGraph);
}




