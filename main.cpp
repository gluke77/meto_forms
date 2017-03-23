#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QThread>
#include <QSplashScreen>

#include "storedvalue.h"
#include "rangeparameter.h"
#include "basedevice.h"
#include "modbusmsg.h"
#include "baseport.h"
#include "cpu.h"
#include "storedvalue.h"
#include "hmi.h"
#include "pexline.h"

#include "keypad.h"
#include "numericentry.h"
#include "led.h"
#include "termoframe.h"
#include "mainwindow.h"
#include "logindialog.h"
// #include "splash.h"

#include "comthreads.h"

int MyMessageBox(const QString & caption, const QString & text, int button0 = QMessageBox::Ok, int button1 = 0, int button2 = 0);
int MyMessageBox(const QString & caption, const QString & text, const QString & text0, const QString & text1);

PexLine pexline;

BasePort	* com3;
BasePort	* com4;
BasePort	* com5;
BasePort	* com6;
BasePort	* com7;

Com3Thread	* com3Thread;
Com4Thread	* com4Thread;
Com5Thread	* com5Thread;
Com6Thread	* com6Thread;
Com7Thread	* com7Thread;

void buildLine();
void destroyLine();
void connectDevices();

int main(int argc, char ** argv)
{
	QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251"));
	
	QApplication app(argc, argv);

        QPixmap pixmap("mnemo.gif");
        QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
        splash.show();
        app.processEvents();
        splash.showMessage(QString("Инициализация системы. Пожалуйста, ждите..."), Qt::AlignCenter);
        app.processEvents();
	
	buildLine();
        app.processEvents();

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("meto.db");
	bool ok = db.open();

        app.processEvents();

        connectDevices();
        app.processEvents();

        startComThreads();
        app.processEvents();

        splash.clearMessage();
        app.processEvents();

        MainWindow * main = new MainWindow;
//	main->setCursor(Qt::BlankCursor);

        splash.finish(main);

        LoginDialog * loginDialog = new LoginDialog(main);
        QApplication::connect(loginDialog, SIGNAL(userChanged(QString, int)), main, SLOT(setUser(QString, int)));
        QApplication::connect(loginDialog, SIGNAL(rejected()), qApp, SLOT(quit()));
        QApplication::connect(loginDialog, SIGNAL(accepted()), main, SLOT(showMaximized()));

        loginDialog->show();

        int result = app.exec();

	destroyLine();
	
	return result;
}

void buildLine()
{
	com3 = new BasePort(L"\\\\.\\com3", CBR_115200, NOPARITY, TWOSTOPBITS, 7, 0);
	com4 = new BasePort(L"\\\\.\\com4", CBR_19200, NOPARITY, TWOSTOPBITS, 7, 2500);
	com5 = new BasePort(L"\\\\.\\com5", CBR_38400, NOPARITY, TWOSTOPBITS, 7, 500);
	com6 = new BasePort(L"\\\\.\\com6", CBR_115200, NOPARITY, TWOSTOPBITS, 7, 500);
	com7 = new BasePort(L"\\\\.\\com7", CBR_115200, NOPARITY, TWOSTOPBITS, 7, 500);

	pexline.cpu.setPort(com3);

	pexline.tuInvertor.setPort(com5);
	pexline.printer.setPort(com5);
	
	pexline.sgInvertor.setPort(com4);
	pexline.sgZone1.setPort(com4);
	pexline.sgZone2.setPort(com4);
	pexline.uzg.setPort(com4);

	pexline.e2kInvertor.setPort(com6);
	pexline.e2kPressure.setPort(com6);
	pexline.e2kTrace.setPort(com6);
	pexline.e2kZone1.setPort(com6);
	pexline.e2kZone2.setPort(com6);
	pexline.e2kZone3.setPort(com6);

	pexline.e2pInvertor.setPort(com6);
	pexline.e2pPressure.setPort(com6);
	pexline.e2pTrace.setPort(com6);
	pexline.e2pZone1.setPort(com6);
	pexline.e2pZone2.setPort(com6);
	pexline.e2pZone3.setPort(com6);
	pexline.e2pZone4.setPort(com6);

	pexline.e3kInvertor.setPort(com7);
	pexline.e3kPressure.setPort(com7);
	pexline.e3kTrace.setPort(com7);
	pexline.e3kZone1.setPort(com7);
	pexline.e3kZone2.setPort(com7);
	pexline.e3kZone3.setPort(com7);

	pexline.e3mInvertor.setPort(com7);
	pexline.e3mPressure.setPort(com7);
	pexline.e3mTrace.setPort(com7);
	pexline.e3mZone1.setPort(com7);
	pexline.e3mZone2.setPort(com7);
	pexline.e3mZone3.setPort(com7);
	
	pexline.e3pInvertor.setPort(com7);
	pexline.e3pPressure.setPort(com7);
	pexline.e3pTrace.setPort(com7);
	pexline.e3pZone1.setPort(com7);
	pexline.e3pZone2.setPort(com7);
	pexline.e3pZone3.setPort(com7);
	pexline.e3pZone4.setPort(com7);

	com3->initPort();
	com4->initPort();
	com5->initPort();
	com6->initPort();
	com7->initPort();

	com3Thread = new Com3Thread(com3);
	com4Thread = new Com4Thread(com4);
	com5Thread = new Com5Thread(com5);
	com6Thread = new Com6Thread(com6);
	com7Thread = new Com7Thread(com7);
}

void connectDevices()
{
	pexline.cpu.connect();
//	pexline.uzg.connect();

//	pexline.tuInvertor.connect();

//	pexline.sgInvertor.connect();
//	pexline.sgZone1.connect();
//	pexline.sgZone2.connect();

//	pexline.e2kInvertor.connect();
//	pexline.e2kPressure.connect();
//	pexline.e2kTrace.connect();
//	pexline.e2kZone1.connect();
//	pexline.e2kZone2.connect();
//	pexline.e2kZone3.connect();

//	pexline.e2pInvertor.connect();
//	pexline.e2pPressure.connect();
//	pexline.e2pTrace.connect();
///	pexline.e2pZone1.connect();
//	pexline.e2pZone2.connect();
//	pexline.e2pZone3.connect();
//	pexline.e2pZone4.connect();

//	pexline.e3kInvertor.connect();
//	pexline.e3kPressure.connect();
//	pexline.e3kTrace.connect();
//	pexline.e3kZone1.connect();
//	pexline.e3kZone2.connect();
//	pexline.e3kZone3.connect();

//	pexline.e3mInvertor.connect();
//	pexline.e3mPressure.connect();
//	pexline.e3mTrace.connect();
//	pexline.e3mZone1.connect();
//	pexline.e3mZone2.connect();
//	pexline.e3mZone3.connect();
	
//	pexline.e3pInvertor.connect();
//	pexline.e3pPressure.connect();
//	pexline.e3pTrace.connect();
//	pexline.e3pZone1.connect();
//	pexline.e3pZone3.connect();
//	pexline.e3pZone2.connect();
//	pexline.e3pZone4.connect();
}

void destroyLine()
{
	com3Thread->stop();
	com4Thread->stop();
	com5Thread->stop();
	com6Thread->stop();
	com7Thread->stop();

	com3Thread->wait();
	com4Thread->wait();
	com5Thread->wait();
	com6Thread->wait();
	com7Thread->wait();

	delete com3;
	delete com4;
	delete com5;
	delete com6;
	delete com7;
	
	delete com3Thread;
	delete com4Thread;
	delete com5Thread;
	delete com6Thread;
	delete com7Thread;
}

void startComThreads()
{
	if (com3Thread)
		com3Thread->start(QThread::LowPriority);

	if (com4Thread)
		com4Thread->start(QThread::LowPriority);

	if (com5Thread)										// tuInvertor polled in com4Thread
		com5Thread->start(QThread::LowPriority);

	if (com6Thread)
		com6Thread->start(QThread::LowPriority);

	if (com7Thread)
		com7Thread->start(QThread::LowPriority);
			
}

int MyMessageBox(const QString & caption, const QString & text, int button0, int button1, int button2)
{
	QMessageBox	box(caption, text, QMessageBox::NoIcon, button0, button1, button2);
	
	QFont f = box.font();
	f.setPointSize(12);
	f.setBold(true);
	box.setFont(f);
	
	return box.exec();
}

int MyMessageBox(const QString & caption, const QString & text, const QString & text0, const QString & text1)
{
	QMessageBox	box(caption, text, QMessageBox::NoIcon, 1, 2, 0);
	
	QFont f = box.font();
	f.setPointSize(12);
	f.setBold(true);
	box.setFont(f);
	box.setButtonText(1, text0);
	box.setButtonText(2, text1);
	
	return box.exec();
}
