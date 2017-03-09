#ifndef _SPLASH_INCLUDED
#define _SPLASH_INCLUDED

#include <QtGui>
#include "logindialog.h"
#include "mainwindow.h"

class Splash : public QWidget
{
	Q_OBJECT

private:

	MainWindow	* _mainWindow;

public:

	QLabel		* _label;
	QLabel		* _label2;
	QPixmap		 _pixmap;
	QVBoxLayout	* _layout;

	LoginDialog * loginDialog;

	Splash(MainWindow * mainWindow = 0, QWidget * parent = 0, Qt::WFlags f = 0) :
		QWidget(parent, f), _mainWindow(mainWindow)
	{
		loginDialog = new LoginDialog(this);

		if (_mainWindow)
			connect(loginDialog, SIGNAL(userChanged(QString, int)), _mainWindow, SLOT(setUser(QString, int)));
		connect(loginDialog, SIGNAL(rejected()), qApp, SLOT(quit()));
		connect(loginDialog, SIGNAL(accepted()), this, SLOT(on_loginDialog_accepted()));
		
		_pixmap = QPixmap("mnemo.gif");
		_label = new QLabel(this);
	
		_label->setPixmap(_pixmap);
		_label->setGeometry(98, 250, 830, 134);

		_label2 = new QLabel(this);
		_label2->setGeometry(98, 650, 830, 134);
		_label2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		_label2->setAlignment(Qt::AlignHCenter);

		QFont fnt = _label2->font();
		fnt.setPointSize(18);
//		fnt.setBold(true);

		_label2->setFont(fnt);
		_label2->setText(QString("Инициализация системы. Пожалуйста, ждите..."));
		
		_label->show();
		_label2->show();
	
//		_layout = new QVBoxLayout(this);
//		_layout->addWidget(_label);

//		connect(_movie, SIGNAL(started()), loginDialog, SLOT(show()));
//		connect(_movie, SIGNAL(started()), _label2, SLOT(hide()));


	}

public slots:

	void on_loginDialog_accepted()
	{
		//close();
		if (_mainWindow)
			_mainWindow->showMaximized();
	}

};

#endif /* _SPLASH_INCLUDED */

