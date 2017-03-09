#ifndef _KEYBOARD_INCLUDED
#define _KEYBOARD_INCLUDED


#include <QtGui>

#include "ui_KeyboardDialog.h"

class KeyBoard : public QDialog, public Ui_KeyboardDialog
{
	Q_OBJECT
	
public:
	KeyBoard(QWidget * parent = 0, Qt::WFlags f = 0) :
		QDialog(parent, f)
	{
		setupUi(this);
	}

protected slots:
	void on_buttonRusLat_toggled(bool fRus = true)
	{
		if (fRus)
		{
			buttonA->setText(QString("�"));
			buttonB->setText(QString("�"));
			buttonC->setText(QString("�"));			
			buttonD->setText(QString("�"));
			buttonE->setText(QString("�"));			
			buttonF->setText(QString("�"));
			buttonG->setText(QString("�"));
			buttonH->setText(QString("�"));
			buttonI->setText(QString("�"));
			buttonJ->setText(QString("�"));
			buttonK->setText(QString("�"));
			buttonL->setText(QString("�"));
			buttonM->setText(QString("�"));
			buttonN->setText(QString("�"));
			buttonO->setText(QString("�"));
			buttonP->setText(QString("�"));
			buttonQ->setText(QString("�"));
			buttonR->setText(QString("�"));
			buttonS->setText(QString("�"));
			buttonT->setText(QString("�"));
			buttonU->setText(QString("�"));
			buttonV->setText(QString("�"));
			buttonW->setText(QString("�"));
			buttonX->setText(QString("�"));
			buttonY->setText(QString("�"));
			buttonZ->setText(QString("�"));
			buttonSlash->setText(QString("�"));
			buttonBackslash->setText(QString("�"));
			buttonColon->setText(QString("�"));
			buttonSemicolon->setText(QString("�"));
			buttonComma->setText(QString("�"));
			buttonDot->setText(QString("�"));
			buttonRusLat->setText(QString("���"));
		}
		else
		{
			buttonA->setText(QString("a"));
			buttonB->setText(QString("b"));
			buttonC->setText(QString("c"));			
			buttonD->setText(QString("d"));
			buttonE->setText(QString("e"));			
			buttonF->setText(QString("f"));
			buttonG->setText(QString("g"));
			buttonH->setText(QString("h"));
			buttonI->setText(QString("i"));
			buttonJ->setText(QString("j"));
			buttonK->setText(QString("k"));
			buttonL->setText(QString("l"));
			buttonM->setText(QString("m"));
			buttonN->setText(QString("n"));
			buttonO->setText(QString("o"));
			buttonP->setText(QString("p"));
			buttonQ->setText(QString("q"));
			buttonR->setText(QString("r"));
			buttonS->setText(QString("s"));
			buttonT->setText(QString("t"));
			buttonU->setText(QString("u"));
			buttonV->setText(QString("v"));
			buttonW->setText(QString("w"));
			buttonX->setText(QString("x"));
			buttonY->setText(QString("y"));
			buttonZ->setText(QString("z"));
			buttonSlash->setText(QString("/"));
			buttonBackslash->setText(QString("\\"));
			buttonColon->setText(QString(":"));
			buttonSemicolon->setText(QString(";"));
			buttonComma->setText(QString(","));
			buttonDot->setText(QString("."));
			buttonRusLat->setText(QString("���"));

		}	
	}
};

#endif /* _KEYBOARD_INCLUDED */

