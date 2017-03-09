#include <QtGui>
#include "graphselectdialog.h"
#include "graph.h"


GraphSelectDialog::GraphSelectDialog(QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);
	
	_lastGraph = Graph::Select;
	
	_graphNames[Graph::Select] = QString("�������");
	
	_graphNames[Graph::E2pZone1Pv] = QString("�� �����.\n����1\n�, ��");
	_graphNames[Graph::E2pZone2Pv] = QString("�� �����.\n����2\n�, ��");
	_graphNames[Graph::E2pZone3Pv] = QString("�� �����.\n����3\n�, ��");
	_graphNames[Graph::E2pZone4Pv] = QString("�� �����.\n����4\n�, ��");
	_graphNames[Graph::E2pTracePv] = QString("�� �����.\n������\n�, ��");

	_graphNames[Graph::E2kZone1Pv] = QString("�� �����.\n����1\n�, ��");
	_graphNames[Graph::E2kZone2Pv] = QString("�� �����.\n����2\n�, ��");
	_graphNames[Graph::E2kZone3Pv] = QString("�� �����.\n����3\n�, ��");
	_graphNames[Graph::E2kTracePv] = QString("�� �����.\n������\n�, ��");

	_graphNames[Graph::E3kZone1Pv] = QString("�� ���.\n����1\n�, ��");
	_graphNames[Graph::E3kZone2Pv] = QString("�� ���.\n����2\n�, ��");
	_graphNames[Graph::E3kZone3Pv] = QString("�� ���.\n����3\n�, ��");
	_graphNames[Graph::E3kTracePv] = QString("�� ���.\n������\n�, ��");

	_graphNames[Graph::E3pZone1Pv] = QString("�� ���.\n����1\n�, ��");
	_graphNames[Graph::E3pZone2Pv] = QString("�� ���.\n����2\n�, ��");
	_graphNames[Graph::E3pZone3Pv] = QString("�� ���.\n����3\n�, ��");
	_graphNames[Graph::E3pZone4Pv] = QString("�� ���.\n����4\n�, ��");
	_graphNames[Graph::E3pTracePv] = QString("�� ���.\n������\n�, ��");

	_graphNames[Graph::E3mZone1Pv] = QString("�� ���.\n����1\n�, ��");
	_graphNames[Graph::E3mZone2Pv] = QString("�� ���.\n����2\n�, ��");
	_graphNames[Graph::E3mZone3Pv] = QString("�� ���.\n����3\n�, ��");
	_graphNames[Graph::E3mTracePv] = QString("�� ���.\n������\n�, ��");

	_graphNames[Graph::SgZone1Pv] = QString("��\n����1\n�, ��");
	_graphNames[Graph::SgZone2Pv] = QString("��\n����2\n�, ��");

	_graphNames[Graph::E2pRotate] = QString("�� �����.\nc�������,\n��/���");
	_graphNames[Graph::E2kRotate] = QString("�� �����.\nc�������,\n��/���");
	_graphNames[Graph::E3kRotate] = QString("�� ���.\nc�������,\n��/���");
	_graphNames[Graph::E3pRotate] = QString("�� ���.\nc�������,\n��/���");
	_graphNames[Graph::E3mRotate] = QString("�� ���.\nc�������,\n��/���");

	_graphNames[Graph::SgRotate] = QString("��\nc�������,\n��/���");
	_graphNames[Graph::SgSpeed] = QString("��\nc�������,\n�/���");
	_graphNames[Graph::TuSpeed] = QString("��\nc�������,\n�/���");

	_graphNames[Graph::E2pCurrent] = QString("�� �����.\n���, �");
	_graphNames[Graph::E2kCurrent] = QString("�� �����.\n���, �");
	_graphNames[Graph::E3kCurrent] = QString("�� ���.\n���, �");
	_graphNames[Graph::E3pCurrent] = QString("�� ���.\n���, �");
	_graphNames[Graph::E3mCurrent] = QString("�� ���.\n���, �");

	_graphNames[Graph::SgCurrent] = QString("��\n���, �");
	_graphNames[Graph::TuCurrent] = QString("��\n���, �");

	_graphNames[Graph::E2pPressure] = QString("�� �����.\n��������,\n���");
	_graphNames[Graph::E2kPressure] = QString("�� �����.\n��������,\n���");
	_graphNames[Graph::E3kPressure] = QString("�� ���.\n��������,\n���");
	_graphNames[Graph::E3pPressure] = QString("�� ���.\n��������,\n���");
	_graphNames[Graph::E3mPressure] = QString("�� ���.\n��������,\n���");

	_graphNames[Graph::UzgFreq] = QString("���\n�������,\n��");
	_graphNames[Graph::UzgCurrent] = QString("���\n���, �");
	_graphNames[Graph::UzgPower] = QString("���\n��������,\n��");
	_graphNames[Graph::UzgTemp] = QString("���\n�, ��");

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




