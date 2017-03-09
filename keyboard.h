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
			buttonA->setText(QString("ô"));
			buttonB->setText(QString("è"));
			buttonC->setText(QString("ñ"));			
			buttonD->setText(QString("â"));
			buttonE->setText(QString("ó"));			
			buttonF->setText(QString("à"));
			buttonG->setText(QString("ï"));
			buttonH->setText(QString("ð"));
			buttonI->setText(QString("ø"));
			buttonJ->setText(QString("î"));
			buttonK->setText(QString("ë"));
			buttonL->setText(QString("ä"));
			buttonM->setText(QString("ü"));
			buttonN->setText(QString("ò"));
			buttonO->setText(QString("ù"));
			buttonP->setText(QString("ç"));
			buttonQ->setText(QString("é"));
			buttonR->setText(QString("ê"));
			buttonS->setText(QString("û"));
			buttonT->setText(QString("å"));
			buttonU->setText(QString("ã"));
			buttonV->setText(QString("ì"));
			buttonW->setText(QString("ö"));
			buttonX->setText(QString("÷"));
			buttonY->setText(QString("í"));
			buttonZ->setText(QString("ÿ"));
			buttonSlash->setText(QString("õ"));
			buttonBackslash->setText(QString("ú"));
			buttonColon->setText(QString("æ"));
			buttonSemicolon->setText(QString("ý"));
			buttonComma->setText(QString("á"));
			buttonDot->setText(QString("þ"));
			buttonRusLat->setText(QString("ËÀÒ"));
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
			buttonRusLat->setText(QString("ÐÓÑ"));

		}	
	}
};

#endif /* _KEYBOARD_INCLUDED */

