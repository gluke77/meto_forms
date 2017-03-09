#include <QtSql>
#include <QtGlobal>

#include <windows.h>

#include "mainwindow.h"
#include "logindialog.h"
#include "graphselectdialog.h"
#include "autostart.h"
#include "pexline.h"
#include "veraxisframe.h"
#include "horaxisframe.h"
#include "graphframe.h"

int MyMessageBox(const QString & caption, const QString & text, int button0 = QMessageBox::Ok, int button1 = 0, int button2 = 0);
int MyMessageBox(const QString & caption, const QString & text, const QString & text0, const QString & text1);

extern PexLine pexline;

MainWindow::MainWindow(QWidget * parent) : QWidget(parent)
{
	setupUi(this);

	_loginDialog = new LoginDialog(this);
	_graphSelectDialog = new GraphSelectDialog(this);
	mainClockTimer = new QTimer(this);
	storeProcessClockTimer = new QTimer(this);

	setWindowTitle(QString("ОАО \"НПП \"СИСТЕМА\" ЛМПТ-1"));
	
	createActions();
	createMenu();
	doConnections();
	setupButtons();
	stopDebugButton->hide();
	debugAct->setChecked(false);
	setDebug(false);
	refreshOperTable();
	refreshLogOperTable();
	on_logUpdateButton_clicked();
	profilesUpdateTable();
	
	mainClockTimer->start(300);
	storeProcessClockTimer->start(5000);

	_waterMode = 0;
	
	fillWarningMap();
	
	_checkFoilEncoder = true;
	
	mnemoLabel->setMovie(&_mnemoMovie);
	
	_mnemoMovie.setFileName("mnemo.gif");
	_mnemoMovie.setCacheMode(QMovie::CacheAll);
	_mnemoMovie.start();
	_mnemoMovie.setPaused(true);
	
	_graph1Type = Graph::Select;
	_graph2Type = Graph::Select;
	_graph3Type = Graph::Select;
	
	//connect(coolerTubeButton, SIGNAL(clicked(bool)), &_mnemoMovie, SLOT(setPaused(bool)));
	//connect(coolerTubeButton, SIGNAL(clicked()), this, SLOT(storeSettings()));
	//connect(&(pexline.cpu), SIGNAL(foliEncoderChanged(bool)), &_mnemoMovie, SLOT(setPaused(bool)));
	//connect(&(pexline.cpu), SIGNAL(foilEncoderChangedInverse(bool)), &_mnemoMovie, SLOT(setPaused(bool)));

	
	logSetFilterButton->hide();
	logApplyFilterButton->hide();
	coolerTubeButton->hide();
	//uzgWorkLabel->hide();
	
	pe1zone1TermoFrame->textLabel->setText("1");
	pe1zone2TermoFrame->textLabel->setText("2");
	pe1zone3TermoFrame->textLabel->setText("3");
	pe1zone4TermoFrame->textLabel->setText("4");
	pe1traceTermoFrame->textLabel->setText("T");

	pe2zone1TermoFrame->textLabel->setText("1");
	pe2zone2TermoFrame->textLabel->setText("2");
	pe2zone3TermoFrame->textLabel->setText("3");
	pe2zone4TermoFrame->textLabel->setText("4");
	pe2traceTermoFrame->textLabel->setText("T");

	kl1zone1TermoFrame->textLabel->setText("1");
	kl1zone2TermoFrame->textLabel->setText("2");
	kl1zone3TermoFrame->textLabel->setText("3");
	kl1traceTermoFrame->textLabel->setText("T");

	kl2zone1TermoFrame->textLabel->setText("1");
	kl2zone2TermoFrame->textLabel->setText("2");
	kl2zone3TermoFrame->textLabel->setText("3");
	kl2traceTermoFrame->textLabel->setText("T");

	pemZone1TermoFrame->textLabel->setText("1");
	pemZone2TermoFrame->textLabel->setText("2");
	pemZone3TermoFrame->textLabel->setText("3");
	pemTraceTermoFrame->textLabel->setText("T");
	
	egZone1TermoFrame->textLabel->setText("1");
	egZone2TermoFrame->textLabel->setText("2");
	
	sgWaterLed->setOnColor(Qt::blue);
	sgWaterLed->setOffColor(Qt::red);
	sgWaterLed->off();
	
	e2WaterLed->setOnColor(Qt::blue);
	e2WaterLed->setOffColor(Qt::red);
	e2WaterLed->off();

	e3WaterLed->setOnColor(Qt::blue);
	e3WaterLed->setOffColor(Qt::red);
	e3WaterLed->off();
	
	tank0Led->setOnColor(Qt::blue);
	tank0Led->setOffColor(Qt::red);
	tank0Led->off();
	
	tank1Led->setOnColor(Qt::blue);
	tank1Led->setOffColor(Qt::red);
	tank1Led->off();

	tank2Led->setOnColor(Qt::red);
	tank2Led->setOffColor(palette().color(QPalette::Window));
	tank2Led->off();

	bath0Led->setOnColor(Qt::blue);
	bath0Led->setOffColor(palette().color(QPalette::Window));
	bath0Led->off();

	bath1Led->setOnColor(Qt::blue);
	bath1Led->setOffColor(palette().color(QPalette::Window));
	bath1Led->off();
	
	airInSystemLed->setOnColor(Qt::green);
	airInSystemLed->setOffColor(Qt::red);
	airInSystemLed->off();

	loadSettings();
	
	_graphVerScale = 1;
	_graphHorScale = 1;
	_graphVerPos = 0;
	_graphHorPos = 0;
	
	QPalette _palette = palette();
	
	_palette.setColor(QPalette::ButtonText, QColor(170, 0, 0));
	graphSelect1Button->setPalette(_palette);

	_palette.setColor(QPalette::ButtonText, QColor(0, 85, 0));
	graphSelect2Button->setPalette(_palette);

	_palette.setColor(QPalette::ButtonText, QColor(0, 0, 127));
	graphSelect3Button->setPalette(_palette);

	_lineStarted = false;
}


void MainWindow::on_lineSvarkaSelectButton_clicked()
{
	if (QString("Линия") == lineSvarkaSelectButton->text())
		lineSvarkaSelectButton->setText(QString("Сварка"));
	else
		lineSvarkaSelectButton->setText(QString("Линия"));
	
	update();
}

void MainWindow::on_logCommitButton_clicked()
{
	if (_unacceptedWarningList.isEmpty())
		return;

	Warning warning = _unacceptedWarningList.front();
	
	_acceptedWarningList.push_back(warning);
	_unacceptedWarningList.pop_front();
	warning.isAccepted = true;
	warning.accepted = QDateTime::currentDateTime();

	QSqlQuery query;
	query.prepare("update event_log set accepted = :accepted where id = :id and raised = :raised");
	query.bindValue(":id", warning.id);
	query.bindValue(":raised", warning.raised.toString("yyyy-MM-dd hh:mm:ss"));
	query.bindValue(":accepted", warning.accepted.toString("yyyy-MM-dd hh:mm:ss"));
	query.exec();

	_currentWarning = _acceptedWarningList.end() - 1;

	showWarning();
	pexline.cpu.setSirenMode(0);
	pexline.cpu.setLampStop(false);
}

void MainWindow::postWarning(int code, int sirenMode)
{
	// -1 == sirenMode -- autoAccept
	
	Warning	warning(code, ++_nextWarningId, _user, -1 == sirenMode);

	if (0 < sirenMode)
		pexline.cpu.setSirenMode(sirenMode);

	if (warning.isAccepted)
	{
		_acceptedWarningList.push_back(warning);
		_currentWarning = _acceptedWarningList.end() - 1;
	}
	else
		_unacceptedWarningList.push_back(warning);
	
	showWarning();
	
	QSqlQuery query;
	
	if (warning.isAccepted)
		query.prepare("insert into event_log(id, code, raised, user_name, accepted) values(:id, :code, :raised, :user_name, :accepted)");
	else
		query.prepare("insert into event_log(id, code, raised, user_name) values(:id, :code, :raised, :user_name)");
	
	query.bindValue(":id", warning.id);
	query.bindValue(":code", warning.code);
	query.bindValue(":raised", warning.raised.toString("yyyy-MM-dd hh:mm:ss"));
	query.bindValue(":user_name", warning.user);
	
	if (warning.isAccepted)
		query.bindValue(":accepted", warning.accepted.toString("yyyy-MM-dd hh:mm:ss"));

	query.exec();
}

void MainWindow::showWarning()
{
	QPalette normalPalette = palette();
	QPalette redPalette = palette();
	redPalette.setColor(QPalette::Button, Qt::red);

	if (!_unacceptedWarningList.isEmpty())
	{
		Warning aWarning = _unacceptedWarningList.front();

		QString number;
		number.setNum(aWarning.id);

		logNumberEdit->setText(number);

		QString text = aWarning.raised.toString("hh:mm:ss ") + _warningTextMap[aWarning.code];

		logEdit->setText(text);

		logCommitButton->setEnabled(true);
		logCommitButton->setPalette(redPalette);
	}
	else
	{
		logNumberEdit->clear();
		logEdit->clear();
		logCommitButton->setEnabled(false);
		logCommitButton->setPalette(normalPalette);
	}

	logEdit1->clear();
	logNumberEdit1->clear();
	logEdit2->clear();
	logNumberEdit2->clear();
	logEdit3->clear();
	logNumberEdit3->clear();

	logUpButton->setEnabled(false);
	logDownButton->setEnabled(false);

	if (!_acceptedWarningList.isEmpty())
	{

		QList<Warning>::iterator begin = _acceptedWarningList.begin();
		QList<Warning>::iterator end = _acceptedWarningList.end();

		QList<Warning>::iterator warning = _currentWarning;

		logNumberEdit3->setText(QString().setNum(warning->id));
		logEdit3->setText(warning->raised.toString("hh:mm:ss ") + _warningTextMap[warning->code]);

		if (begin == warning)
			return;

		warning--;
		logNumberEdit2->setText(QString().setNum(warning->id));
		logEdit2->setText(warning->raised.toString("hh:mm:ss ") + _warningTextMap[warning->code]);
		
		if (begin == warning)
			return;

		warning--;
		logNumberEdit1->setText(QString().setNum(warning->id));
		logEdit1->setText(warning->raised.toString("hh:mm:ss ") + _warningTextMap[warning->code]);

		if (_currentWarning + 1 < end)
			logDownButton->setEnabled(true);

		if (begin < warning)
			logUpButton->setEnabled(true);

	}

}

void MainWindow::on_logUpButton_clicked()
{
	if (!_acceptedWarningList.isEmpty())
		if (_acceptedWarningList.begin() < _currentWarning)
			_currentWarning--;

	showWarning();	
}

void MainWindow::on_logDownButton_clicked()
{
	if (!_acceptedWarningList.isEmpty())
		if (_acceptedWarningList.end() > _currentWarning  + 1)
			_currentWarning++;

	showWarning();	
}

void MainWindow::on_mainClockTimer_timeout()
{
	static int counter = 0;
	
	dateTimeEdit->setText(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"));

	if (++counter > 5)
	{
		counter = 0;

		checkMnemoE2pLed();
		checkMnemoE2kLed();
		checkMnemoE3kLed();
		checkMnemoE3pLed();
		checkMnemoE3mLed();
		checkMnemoTuLed();
		checkMnemoSgLed();
		checkMnemoBathLed();
	}
	
	if (3 == counter)
		checkAutostartLeds();
		
	if (1 == counter)
		showBottomEdit();
}

void MainWindow::setWaterMode(int waterMode)
{
	_waterMode = waterMode;
	
	switch (_waterMode)
	{
	case 0:
		waterButton->setText(QString("Вода\nУровень \"0\""));
		break;
	case 1:
		waterButton->setText(QString("Вода\nДеж. уровень"));
		break;
	case 2:
		waterButton->setText(QString("Вода\nРаб. уровень"));
		break;
	}
}

void MainWindow::on_waterButton_clicked()
{
	_waterMode++;
	if (_waterMode > 2)
		_waterMode = 0;
	
	if (!pexline.cpu.isConnected())
	{
		_waterMode = 0;
		postWarning(CpuDisconnected);
	}
	else if (!pexline.cpu.tankWaterLevel1On())
	{
		_waterMode = 0;
		postWarning(TankWaterLevel1);
	}
	else if (!pexline.cpu.pressureOn())
	{
		_waterMode = 0;
		postWarning(Pressure);
	}
	
	pexline.cpu.setWaterMode(_waterMode);
	setWaterMode(_waterMode);
}

void MainWindow::on_coolerPumpButton_clicked(bool state)
{
	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
	}
	else if (state)
	{
		if (!pexline.cpu.tankWaterLevel0On())
		{
//			state = false;
			postWarning(TankWaterLevel0);
		}
		else if (pexline.cpu.tankWaterLevel2On())
		{
//			state = false;
			postWarning(TankWaterLevel2);
		}
	}
	
	coolerPumpButton->setChecked(state);
	pexline.cpu.setCoolerPump(state);
}

void MainWindow::on_extruderPumpButton_clicked(bool state)
{
	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
	}
	else if (state)
	{
		if (!pexline.cpu.tankWaterLevel0On())
		{
//			state = false;
			postWarning(TankWaterLevel0);
		}
	}
	
	extruderPumpButton->setChecked(state);
	pexline.cpu.setExtruderPump(state);
}

void MainWindow::on_germoButton_clicked(bool state)
{
/*
	if (!state)
	{
		germoButton->setChecked(true);
		return;
	}
*/	
	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
	}
	else if (!pexline.cpu.pressureOn())
	{
		state = false;
		postWarning(Pressure);
	}
/*
	else if (!pexline.cpu.airInTubeOn())
	{
		state = false;
		postWarning(AirInTube);
	}
	else if (!pexline.cpu.germoBeginOn())
	{
		state = false;
		postWarning(GermoBegin);
	}
*/
	germoButton->setChecked(state);
	
//	if (state)
		pexline.cpu.setGermo(state);
}

bool MainWindow::on_pushdownTractorButton_clicked(bool state)
{
	bool new_state = state;
	
	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
		new_state = false;
	}
	else if (!pexline.cpu.pressureOn() && new_state)
	{
		new_state = false;
		postWarning(Pressure);
	}

	pushdownTractorButton->setChecked(new_state);
	pexline.cpu.setTractorLock(new_state);

	return state == new_state;
}

void MainWindow::on_drierButton_clicked(bool state)
{
	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
	}
	else if (!pexline.cpu.pressureOn() && state)
	{
		state = false;
		postWarning(Pressure);
	}

	drierButton->setChecked(state);
	pexline.cpu.setDrier(state);
}

void MainWindow::on_pressureButton_clicked(bool state)
{
	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
	}
	else if (!pexline.cpu.pressureOn() && state)
	{
		state = false;
		postWarning(Pressure);
	}

	pressureButton->setChecked(state);
	pexline.cpu.setAirInTube(state);
}

bool MainWindow::on_driveParamPe1MotorStartButton_clicked(bool state)
{
	bool new_state = state;

	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
		new_state = false;
	}
	else if (state && !pexline.cpu.e2WaterOn())
	{
		if (_checkE2Water)
			new_state = false;
		postWarning(E2Water, 1);
	}
	else if (!pexline.e2pInvertor.isConnected())
	{
		new_state = false;
		postWarning(E2pDisconnected);
	}
	else if (new_state && pexline.e2pInvertor.current().critical())
	{
		if ((warnBoundPe1CurrentButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(E2pOvercurrent, 1);
	}
	else if (new_state && pexline.e2pPressure.pressure().critical())
	{
		if ((warnBoundPe1PressureButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(E2pOverpressure, 1);
	}
		
	driveParamPe1MotorStartButton->setChecked(new_state);
	mnemoE2pButton->setChecked(new_state);
	if (!pexline.e2pInvertor.setRunStop(new_state))
		return false;
		
	return state == new_state;
}

bool MainWindow::on_driveParamPe2MotorStartButton_clicked(bool state)
{
	bool new_state = state;

	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
		new_state = false;
	}
	else if (state && !pexline.cpu.e3WaterOn())
	{
		if (_checkE3Water)
			new_state = false;
		postWarning(E3Water, 1);
	}
	else if (!pexline.e3pInvertor.isConnected())
	{
		new_state = false;
		postWarning(E3pDisconnected);
	}
	else if (new_state && pexline.e3pInvertor.current().critical())
	{
		if ((warnBoundPe2CurrentButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(E3pOvercurrent, 1);
	}
	else if (new_state && pexline.e3pPressure.pressure().critical())
	{
		if ((warnBoundPe2PressureButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(E3pOverpressure, 1);
	}
		
	driveParamPe2MotorStartButton->setChecked(new_state);
	mnemoE3pButton->setChecked(new_state);
	if (!pexline.e3pInvertor.setRunStop(new_state))
		return false;
		
	return new_state == state;
}

bool MainWindow::on_driveParamKl1MotorStartButton_clicked(bool state)
{
	bool new_state = state;
	
	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
		new_state = false;
	}
	else if (state && !pexline.cpu.e2WaterOn())
	{
		if (_checkE2Water)
			new_state = false;
		postWarning(E2Water, 1);
	}
	else if (!pexline.e2kInvertor.isConnected())
	{
		new_state = false;
		postWarning(E2kDisconnected);
	}
	else if (new_state && pexline.e2kInvertor.current().critical())
	{
		if ((warnBoundKl1CurrentButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(E2kOvercurrent, 1);
	}
	else if (new_state && pexline.e2kPressure.pressure().critical())
	{
		if ((warnBoundKl1PressureButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(E2kOverpressure, 1);
	}
		
	driveParamKl1MotorStartButton->setChecked(new_state);
	mnemoE2kButton->setChecked(new_state);
	if (!pexline.e2kInvertor.setRunStop(new_state))
		return false;
		
	return new_state == state;
}

bool MainWindow::on_driveParamKl2MotorStartButton_clicked(bool state)
{
	bool new_state = state;
	
	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
		new_state = false;
	}
	else if (new_state && !pexline.cpu.e3WaterOn())
	{
		if (_checkE3Water)
			new_state = false;
		postWarning(E3Water, 1);
	}
	else if (!pexline.e3kInvertor.isConnected())
	{
		new_state = false;
		postWarning(E3kDisconnected);
	}
	else if (new_state && pexline.e3kInvertor.current().critical())
	{
		if ((warnBoundKl2CurrentButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(E3kOvercurrent, 1);
	}
	else if (new_state && pexline.e3kPressure.pressure().critical())
	{
		if ((warnBoundKl2PressureButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(E3kOverpressure, 1);
	}
		
	driveParamKl2MotorStartButton->setChecked(new_state);
	mnemoE3kButton->setChecked(new_state);
	if (!pexline.e3kInvertor.setRunStop(new_state))
		return false;
	
	return new_state == state;
}

bool MainWindow::on_driveParamPemMotorStartButton_clicked(bool state)
{
	bool new_state = state;
	
	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
		new_state = false;
	}
	else if (state && !pexline.cpu.e3WaterOn())
	{
		if (_checkE3Water)
			new_state = false;
		postWarning(E3Water, 1);
	}
	else if (!pexline.e3mInvertor.isConnected())
	{
		new_state = false;
		postWarning(E3mDisconnected);
	}
	else if (new_state && pexline.e3mInvertor.current().critical())
	{
		if ((warnBoundPemCurrentButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(E3mOvercurrent, 1);
	}
	else if (new_state && pexline.e3mPressure.pressure().critical())
	{
		if ((warnBoundPemPressureButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(E3mOverpressure, 1);
	}
		
	driveParamPemMotorStartButton->setChecked(new_state);
	mnemoE3mButton->setChecked(new_state);
	if (!pexline.e3mInvertor.setRunStop(new_state))
		return false;
		
	return new_state == state;
}

bool MainWindow::on_driveParamTuMotorStartButton_clicked(bool state)
{
	bool new_state = state;
	
	if (!pexline.tuInvertor.isConnected())
	{
		new_state = false;
		postWarning(TuDisconnected);
	}
	else if (new_state && pexline.tuInvertor.current().critical())
	{
		if ((warnBoundTuCurrentButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(TuOvercurrent, 1);
	}
	
	driveParamTuMotorStartButton->setChecked(new_state);
	mnemoTuButton->setChecked(new_state);
	tractorButton->setChecked(new_state);
	if (!pexline.tuInvertor.setRunStop(new_state))
		return false;
		
	return new_state == state;
}

bool MainWindow::on_driveParamSgMotorStartButton_clicked(bool state)
{
	bool new_state = state;
	
	if (!pexline.sgInvertor.isConnected())
	{
		new_state = false;
		postWarning(SgDisconnected);
	}
	else if (new_state && pexline.sgInvertor.current().critical())
	{
		if ((warnBoundSgCurrentButton->isChecked()) && !_debugMode)
			new_state = false;
		postWarning(SgOvercurrent, 1);
	}
	
	driveParamSgMotorStartButton->setChecked(new_state);
	mnemoSgRotateButton->setChecked(new_state);
	if (!pexline.sgInvertor.setRunStop(new_state))
		return false;
		
	return new_state == state;
}

bool MainWindow::on_driveParamSgPositionButton_clicked(bool state)
{
	bool new_state = state;
	
	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
		new_state = false;
	}
	else if (!pexline.cpu.pressureOn() && new_state)
	{
		new_state = false;
		postWarning(Pressure);
	}
	else if (new_state && pexline.uzg.runStop().isOn() && !pexline.cpu.foilEncoderOn())
	{
		if (!_debugMode)
			new_state = false;
		postWarning(FoilEncoder, 1);
	}
	
	driveParamSgPositionButton->setChecked(new_state);
	mnemoSgPositionButton->setChecked(new_state);
	pexline.cpu.setHeadDown(new_state);

	return new_state == state;
}

bool MainWindow::on_driveParamUzgStartButton_clicked(bool state)
{
	bool new_state = state;

	if (!pexline.cpu.isConnected())
	{
		postWarning(CpuDisconnected);
		new_state = false;
	}
	else if (!pexline.uzg.isConnected())
	{
		new_state = false;
		postWarning(UzgDisconnected);
	}
	else if (new_state && !pexline.cpu.headWaterOn())
	{
		if (_checkSgWater)
			new_state = false;
		postWarning(SgWater, 1);
	}
	else if (pexline.uzg.busy().isOn())
	{
		new_state = true;
		postWarning(UzgBusy);
	}
	else if (new_state && 
		(pexline.uzg.temperature().scaledValue() > pexline.uzg.tempAlarm().scaledValue()))
	{
		if (!_debugMode)
			new_state = false;
		postWarning(UzgOverheat, 1);
	}
	else if (new_state && pexline.cpu.headDownOn() && !pexline.cpu.foilEncoderOn())
	{
		if (!_debugMode)
			new_state = false;
		postWarning(FoilEncoder, 1);
	}

	pexline.uzg.setRunStop(new_state);
	uzgRunStopButton->setBlink(new_state);
	driveParamUzgStartButton->setBlink(new_state);
	mnemoUzgButton->setBlink(new_state);
	driveParamUzgStartButton->setChecked(new_state);
	uzgRunStopButton->setChecked(new_state);
	mnemoUzgButton->setChecked(new_state);
	
	return new_state == state;
}

void MainWindow::on_operOperButton_clicked()
{
	if (operOperButton->isChecked())
	{
		techOperButton->setChecked(false);
		adminOperButton->setChecked(false);
	}
	else
		operOperButton->setChecked(true);
}

void MainWindow::on_techOperButton_clicked()
{
	if (techOperButton->isChecked())
	{
		operOperButton->setChecked(false);
		adminOperButton->setChecked(false);
	}
	else
		techOperButton->setChecked(true);
}

void MainWindow::on_adminOperButton_clicked()
{
	if (adminOperButton->isChecked())
	{
		techOperButton->setChecked(false);
		operOperButton->setChecked(false);
	}
	else
		adminOperButton->setChecked(true);
}

void MainWindow::on_addOperButton_clicked()
{
	// operEntry
	// passwordEntry
	// repeatPasswordEntry
	
	if (operEntry->text().isEmpty())
	{
		MyMessageBox(QString("Добавление оператора"),
			QString("Введите имя оператора"));
	}
	else if (passwordEntry->text().isEmpty())
	{
		MyMessageBox(QString("Добавление оператора"),
			QString("Введите пароль"));
	}
	else if (repeatPasswordEntry->text().isEmpty())
	{
		MyMessageBox(QString("Добавление оператора"),
			QString("Введите пароль еще раз"));
	}
	else if (repeatPasswordEntry->text() != passwordEntry->text())
	{
		MyMessageBox(QString("Добавление оператора"),
			QString("Пароль не совпадает"));
	}
	else
	{
		QSqlQuery query;
		query.prepare("insert into users values(:user, :password, :type)");
		query.bindValue(":user", operEntry->text());
		query.bindValue(":password", passwordEntry->text());

		if (operOperButton->isChecked())
			query.bindValue(":type", 0);
		else if (techOperButton->isChecked())
			query.bindValue(":type", 1);
		else if (adminOperButton->isChecked())
			query.bindValue(":type", 2);

		if (!query.exec())
			MyMessageBox(QString("Добавление оператора"),
				QString("Ошибка добавления оператора\nВозможно, оператор с таким именем существует"));
		else
			MyMessageBox(QString("Добавление оператора"),
				QString("Оператор добавлен"));
	}

	operEntry->clear();
	passwordEntry->clear();
	repeatPasswordEntry->clear();
	refreshOperTable();
}

void MainWindow::on_deleteOperButton_clicked()
{
	int row = operListTableWidget->currentRow();
	QTableWidgetItem * item = operListTableWidget->item(row, 0);
	
	QString name;

	if (item)
		name = item->text();

	QSqlQuery query;

	if (!name.isEmpty())
	{

		query.prepare("delete from users where user=:user");
		query.bindValue(":user", name);

		if (query.exec())
			MyMessageBox(QString("Удаление оператора"),
				QString("Оператор удален"));
		else
			MyMessageBox(QString("Удаление оператора"),
				QString("Ошибка удаления оператора"));
		
	}
	else
		MyMessageBox(QString("Удаление оператора"),
			QString("Ошибка удаления оператора"));

	refreshOperTable();
}

void MainWindow::refreshOperTable()
{
	//operListTableWidget
	//
	
	operListTableWidget->clear();
	operListTableWidget->setColumnCount(2);
	operListTableWidget->setRowCount(0);

	QStringList	headers;
	headers << QString("Оператор") << QString("Права доступа");
	operListTableWidget->setHorizontalHeaderLabels(headers);

	QStringList	types;
	types << QString("Оператор") << QString("Техник") << QString("Администратор");

	int row = 0;
	int userType;

	QSqlQuery query("select user, type from users");
	query.exec();

	while (query.next())
	{
		operListTableWidget->insertRow(row);
		operListTableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
		userType = query.value(1).toInt();
		operListTableWidget->setItem(row, 1, new QTableWidgetItem(types[userType]));
		row++;
	}

	int columnWidth = (operListTableWidget->width() - operListTableWidget->verticalHeader()->width()) / 2;

	operListTableWidget->setColumnWidth(0, columnWidth);
	operListTableWidget->setColumnWidth(1, columnWidth);
	
	operListTableWidget->verticalHeader()->hide();
	operListTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	operListTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void MainWindow::on_logUpdateButton_clicked()
{
	if (2 > _userType)
	{
		logAllOperButton->setChecked(true);
		logAllOperButton->setEnabled(false);
	}
	else
		logAllOperButton->setEnabled(true);
	
	logListTable->clear();
	logListTable->setColumnCount(3);
	logListTable->setRowCount(0);

	QStringList	headers;
	headers << QString("Дата/время") << QString("Оператор") << QString("Событие");
	logListTable->setHorizontalHeaderLabels(headers);

	int row = 0;
	int warningCode;

	QString	strQuery("select raised, id, user_name, code from event_log");
	strQuery += QString(" where raised between :begin_date and :end_date");
	
	QDateTime	beginDateTime;
	beginDateTime.setDate(logBeginDate->date());
	beginDateTime.setTime(logBeginTime->time());
	
	QDateTime	endDateTime;
	endDateTime.setDate(logEndDate->date());
	endDateTime.setTime(logEndTime->time());

	if ((!(logAllOperButton->isChecked()) && (logOperTable->currentItem())) || (_userType < 2))
		strQuery += QString(" and user_name = :user_name");
	
	strQuery += QString(" order by raised, id");

	QSqlQuery query;
	query.prepare(strQuery);
	query.bindValue(":begin_date", beginDateTime.toString("yyyy-MM-dd hh:mm:ss"));
	query.bindValue(":end_date", endDateTime.toString("yyyy-MM-dd hh:mm:ss"));
	
	if (_userType < 2)
		query.bindValue(":user_name", _user);
	else if (!(logAllOperButton->isChecked()) && (logOperTable->currentItem()))
		query.bindValue(":user_name", logOperTable->currentItem()->text());
	
	query.exec();

	while (query.next())
	{
		logListTable->insertRow(row);
		logListTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
		logListTable->setItem(row, 1, new QTableWidgetItem(query.value(2).toString()));
		warningCode = query.value(3).toInt();
		logListTable->setItem(row, 2, new QTableWidgetItem(_warningTextMap[warningCode]));
		row++;
	}

	logListTable->resizeColumnToContents(0);
	logListTable->resizeColumnToContents(1);
	logListTable->setColumnWidth(2, logListTable->width() -
		logListTable->columnWidth(0) - logListTable->columnWidth(1));
	
	logListTable->verticalHeader()->hide();
	logListTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	logListTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	
	int rowCount = logListTable->rowCount();
	
	if (rowCount)
		logListTable->setCurrentCell(rowCount - 1, 0);
	else
	{
		logBottomLine->setText(QString("Нет данных"));
		logLineEdit->clear();
	}
}


void MainWindow::doConnections()
{
////
// svarka page
////

{
	connect(svarkaStartSgRotateStartEntry, SIGNAL(valueChanged()), this, SLOT(showSvarkaStartTime()));
	connect(svarkaStartUzgStartEntry, SIGNAL(valueChanged()), this, SLOT(showSvarkaStartTime()));
	connect(svarkaStartTractorStartEntry, SIGNAL(valueChanged()), this, SLOT(showSvarkaStartTime()));
	connect(svarkaStartPushdownTractorStartEntry, SIGNAL(valueChanged()), this, SLOT(showSvarkaStartTime()));
	connect(svarkaStartSgPositionStartEntry, SIGNAL(valueChanged()), this, SLOT(showSvarkaStartTime()));

	connect(svarkaStartSgRotateButton, SIGNAL(clicked()), this, SLOT(showSvarkaStartTime()));
	connect(svarkaStartUzgButton, SIGNAL(clicked()), this, SLOT(showSvarkaStartTime()));
	connect(svarkaStartTractorButton, SIGNAL(clicked()), this, SLOT(showSvarkaStartTime()));
	connect(svarkaStartPushdownTractorButton, SIGNAL(clicked()), this, SLOT(showSvarkaStartTime()));
	connect(svarkaStartSgPositionButton, SIGNAL(clicked()), this, SLOT(showSvarkaStartTime()));

	connect(svarkaStartSgRotateStopEntry, SIGNAL(valueChanged()), this, SLOT(showSvarkaStopTime()));
	connect(svarkaStartUzgStopEntry, SIGNAL(valueChanged()), this, SLOT(showSvarkaStopTime()));
	connect(svarkaStartTractorStopEntry, SIGNAL(valueChanged()), this, SLOT(showSvarkaStopTime()));
	connect(svarkaStartPushdownTractorStopEntry, SIGNAL(valueChanged()), this, SLOT(showSvarkaStopTime()));
	connect(svarkaStartSgPositionStopEntry, SIGNAL(valueChanged()), this, SLOT(showSvarkaStopTime()));

	connect(svarkaStartSgRotateButton, SIGNAL(clicked()), this, SLOT(showSvarkaStopTime()));
	connect(svarkaStartUzgButton, SIGNAL(clicked()), this, SLOT(showSvarkaStopTime()));
	connect(svarkaStartTractorButton, SIGNAL(clicked()), this, SLOT(showSvarkaStopTime()));
	connect(svarkaStartPushdownTractorButton, SIGNAL(clicked()), this, SLOT(showSvarkaStopTime()));
	connect(svarkaStartSgPositionButton, SIGNAL(clicked()), this, SLOT(showSvarkaStopTime()));

}

//////////
// lineStart page
////
{
	connect(lineStartSgRotateStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));
	connect(lineStartUzgStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));
	connect(lineStartTractorStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));
	connect(lineStartPushdownTractorStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));
	connect(lineStartSgPositionStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));
	connect(lineStartWaterModeStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));
	connect(lineStartPe1RotateStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));
	connect(lineStartPe2RotateStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));
	connect(lineStartPemRotateStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));
	connect(lineStartKl1RotateStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));
	connect(lineStartKl2RotateStartEntry, SIGNAL(valueChanged()), this, SLOT(showLineStartTime()));

	connect(lineStartSgRotateButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));
	connect(lineStartUzgButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));
	connect(lineStartTractorButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));
	connect(lineStartPushdownTractorButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));
	connect(lineStartSgPositionButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));
	connect(lineStartWaterModeButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));
	connect(lineStartKl2RotateButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));
	connect(lineStartKl1RotateButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));
	connect(lineStartPe1RotateButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));
	connect(lineStartPe2RotateButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));
	connect(lineStartPemRotateButton, SIGNAL(clicked()), this, SLOT(showLineStartTime()));

	connect(lineStartSgRotateStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));
	connect(lineStartUzgStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));
	connect(lineStartTractorStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));
	connect(lineStartPushdownTractorStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));
	connect(lineStartSgPositionStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));
	connect(lineStartWaterModeStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));
	connect(lineStartPe1RotateStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));
	connect(lineStartPe2RotateStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));
	connect(lineStartPemRotateStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));
	connect(lineStartKl1RotateStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));
	connect(lineStartKl2RotateStopEntry, SIGNAL(valueChanged()), this, SLOT(showLineStopTime()));

	connect(lineStartSgRotateButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	connect(lineStartUzgButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	connect(lineStartTractorButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	connect(lineStartPushdownTractorButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	connect(lineStartSgPositionButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	connect(lineStartWaterModeButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	connect(lineStartKl2RotateButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	connect(lineStartKl1RotateButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	connect(lineStartPe1RotateButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	connect(lineStartPe2RotateButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	connect(lineStartPemRotateButton, SIGNAL(clicked()), this, SLOT(showLineStopTime()));
	

}

	connect(logOperTable, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateLogUpDownOperButtons()));

//////
// buttons
///////

{
	connect(mnemoButton, SIGNAL(clicked()), this, SLOT(showMainScreen()));
	
	connect(menuButton, SIGNAL(clicked()), this, SLOT(showMenu()));
	
	connect(termo1Button1, SIGNAL(clicked()), this, SLOT(showTermo1()));
	connect(termo1Button2, SIGNAL(clicked()), this, SLOT(showTermo1()));
	connect(termo1Button3, SIGNAL(clicked()), this, SLOT(showTermo1()));

	connect(termo2Button1, SIGNAL(clicked()), this, SLOT(showTermo2()));
	connect(termo2Button2, SIGNAL(clicked()), this, SLOT(showTermo2()));
	connect(termo2Button3, SIGNAL(clicked()), this, SLOT(showTermo2()));

	connect(termo3Button1, SIGNAL(clicked()), this, SLOT(showTermo3()));
	connect(termo3Button2, SIGNAL(clicked()), this, SLOT(showTermo3()));
	connect(termo3Button3, SIGNAL(clicked()), this, SLOT(showTermo3()));
	
	connect(allOnButton1, SIGNAL(clicked()), this, SLOT(allTermoOn1()));
	connect(allOffButton1, SIGNAL(clicked()), this, SLOT(allTermoOff1()));

	connect(allOnButton2, SIGNAL(clicked()), this, SLOT(allTermoOn2()));
	connect(allOffButton2, SIGNAL(clicked()), this, SLOT(allTermoOff2()));

	connect(allOnButton3, SIGNAL(clicked()), this, SLOT(allTermoOn3()));
	connect(allOffButton3, SIGNAL(clicked()), this, SLOT(allTermoOff3()));

	connect(_loginDialog, SIGNAL(userChanged(QString, int)), this, SLOT(setUser(QString, int)));
	connect(operButton, SIGNAL(clicked()), _loginDialog, SLOT(exec()));
	connect(mainClockTimer, SIGNAL(timeout()), this, SLOT(on_mainClockTimer_timeout()));
	connect(storeProcessClockTimer, SIGNAL(timeout()), this, SLOT(storeProcess()));
	
	//connect(startButton, SIGNAL(clicked()), startButton, SLOT(startBlink()));
	connect(startButton, SIGNAL(clicked(bool)), this, SLOT(doAutostart(bool)));
}	
/////////////////////////////////
// External devices
/////////////////////////////////	
	
//	pexline.uzg.connect();
{	
	uzgLinkButton->setChecked(false);
	uzgLinkButton->setup();
	uzgRunStopButton->setChecked(false);
	uzgRunStopButton->setup();
	driveParamUzgStartButton->setChecked(false);
	driveParamUzgStartButton->setup();
	mnemoUzgButton->setChecked(false);
	mnemoUzgButton->setup();
	uzgKeepOnButton->setChecked(false);
	uzgKeepOnButton->setup();
	
	uzgFreqEdit->setPrecision(0);
	uzgMinFreqEdit->setPrecision(0);
	uzgMaxFreqEdit->setPrecision(0);
	uzgCurrentEdit->setPrecision(2);
	uzgTempAlarmEdit->setPrecision(0);
	uzgTempStopEdit->setPrecision(0);
	uzgKeepFreqStepEdit->setPrecision(0);
	uzgKeepFreqMaxDeltaEdit->setPrecision(0);
	
	connect(&(pexline.uzg.tvConnected()), SIGNAL(trigged(bool)), uzgLinkButton, SLOT(setChecked(bool)));
	connect(&(pexline.uzg.keepOn()), SIGNAL(trigged(bool)), uzgKeepOnButton, SLOT(setChecked(bool)));

	connect(uzgLinkButton, SIGNAL(clicked(bool)), &(pexline.uzg), SLOT(setConnected(bool)));
	connect(uzgKeepOnButton, SIGNAL(clicked(bool)), &(pexline.uzg), SLOT(setKeepOn(bool)));
	connect(uzgStoreButton, SIGNAL(clicked()), &(pexline.uzg), SLOT(storeSettings()));
	
	connect(&(pexline.uzg.runStop()), SIGNAL(trigged(bool)), this, SLOT(onUzgRunStopChanged(bool)));
	
	connect(&(pexline.uzg.busy()), SIGNAL(trigged(bool)), uzgRunStopButton, SLOT(setBlink(bool)));
	connect(&(pexline.uzg.busy()), SIGNAL(trigged(bool)), driveParamUzgStartButton, SLOT(setBlink(bool)));
	connect(&(pexline.uzg.busy()), SIGNAL(trigged(bool)), mnemoUzgButton, SLOT(setBlink(bool)));

	connect(uzgRunStopButton, SIGNAL(clicked(bool)), this, SLOT(on_driveParamUzgStartButton_clicked(bool)));
	connect(mnemoUzgButton, SIGNAL(clicked(bool)), this, SLOT(on_driveParamUzgStartButton_clicked(bool)));

	connect(&(pexline.uzg.freq()), SIGNAL(scaledValueChanged(double)), uzgFreqEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.freqUpper()), SIGNAL(scaledValueChanged(double)), uzgMaxFreqEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.freqLower()), SIGNAL(scaledValueChanged(double)), uzgMinFreqEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.pwm()), SIGNAL(scaledValueChanged(double)), uzgPwmEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.pwmBase()), SIGNAL(scaledValueChanged(double)), uzgPwmBaseEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.pwmShift()), SIGNAL(scaledValueChanged(double)), uzgPwmShiftEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.pwmMax()), SIGNAL(scaledValueChanged(double)), uzgMaxPwmEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.pwmMin()), SIGNAL(scaledValueChanged(double)), uzgMinPwmEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.current()), SIGNAL(scaledValueChanged(double)), uzgCurrentEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.temperature()), SIGNAL(scaledValueChanged(double)), this, SLOT(onUzgTemperatureChanged(double)));
	connect(&(pexline.uzg.tempAlarm()), SIGNAL(scaledValueChanged(double)), uzgTempAlarmEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.tempStop()), SIGNAL(scaledValueChanged(double)), uzgTempStopEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.keepFreqStep()), SIGNAL(scaledValueChanged(double)), uzgKeepFreqStepEdit, SLOT(setValue(double)));
	connect(&(pexline.uzg.keepFreqMaxDelta()), SIGNAL(scaledValueChanged(double)), uzgKeepFreqMaxDeltaEdit, SLOT(setValue(double)));
	
	connect(uzgFreqEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setFreq(double)));
	connect(uzgMaxFreqEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setFreqUpper(double)));
	connect(uzgMinFreqEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setFreqLower(double)));
	
	connect(uzgPwmEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setPwm(double)));
	connect(uzgPwmBaseEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setPwmBase(double)));
	connect(uzgPwmShiftEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setPwmShift(double)));
	connect(uzgMaxPwmEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setPwmMax(double)));
	connect(uzgMinPwmEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setPwmMin(double)));
	
	connect(uzgKeepFreqStepEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setKeepFreqStep(double)));
	connect(uzgKeepFreqMaxDeltaEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setKeepFreqMaxDelta(double)));
	
	connect(uzgTempAlarmEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setTempAlarm(double)));
	connect(uzgTempStopEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setTempStop(double)));
}	
///////////////////////////
// printer connect
{
	printerConnectButton->setChecked(false);
	printerConnectButton->setup();
	printerRunStopButton->setChecked(false);
	printerRunStopButton->setup();
	printerRunStopButton->setBlinkable(true);

	printerCounterEdit->setPrecision(0);
	
	connect(&(pexline.printer.tvConnected()), SIGNAL(trigged(bool)), printerConnectButton, SLOT(setChecked(bool)));
	connect(printerConnectButton, SIGNAL(clicked(bool)), &(pexline.printer), SLOT(setConnected(bool)));

	connect(printerCounterResetButton, SIGNAL(clicked()), &(pexline.printer), SLOT(resetCounter()));
	
	connect(&(pexline.printer.productCount()), SIGNAL(scaledValueChanged(double)), printerCounterEdit, SLOT(setValue(double)));
	
	connect(&(pexline.printer.runStop()), SIGNAL(trigged(bool)), printerRunStopButton, SLOT(setAlterChecked(bool)));
	connect(&(pexline.printer.msgNo()), SIGNAL(rawValueChanged(int)), this, SLOT(onPrinterMsgNoChanged(int)));
	
	//	connect(printerRunStopButton, SIGNAL(clicked(bool)), this, SLOT(on_printerRunStopButton_clicked(bool)));

/*
	connect(mnemoUzgButton, SIGNAL(clicked(bool)), this, SLOT(on_driveParamUzgStartButton_clicked(bool)));

	connect(uzgFreqEdit, SIGNAL(valueChanged(double)), &(pexline.uzg), SLOT(setFreq(double)));
*/
}

///////////////////////////

	pexline.cpu.connect();
{	
	connect(&(pexline.cpu), SIGNAL(lampStopChanged(bool)),
		this, SLOT(onStopButtonChanged(bool)));

	connect(&(pexline.cpu), SIGNAL(drierChanged(bool)),
		drierButton, SLOT(setChecked(bool)));
	
	connect(&(pexline.cpu), SIGNAL(coolerTubeChanged(bool)),
		coolerTubeButton, SLOT(setChecked(bool)));
	
	connect(coolerTubeButton, SIGNAL(clicked(bool)),
		&(pexline.cpu), SLOT(setCoolerTube(bool)));
	
	connect(&(pexline.cpu), SIGNAL(bunkerMotorChanged(bool)),
		foilBunkerButton, SLOT(setChecked(bool)));
	
	connect(foilBunkerButton, SIGNAL(clicked(bool)),
		&(pexline.cpu), SLOT(setBunkerMotor(bool)));

	connect(&(pexline.cpu), SIGNAL(tractorLockChanged(bool)),
		pushdownTractorButton, SLOT(setChecked(bool)));
	
	connect(&(pexline.cpu), SIGNAL(extruderPumpChanged(bool)),
		extruderPumpButton, SLOT(setChecked(bool)));
	
	connect(&(pexline.cpu), SIGNAL(germoChanged(bool)),
		germoButton, SLOT(setChecked(bool)));
	
	connect(&(pexline.cpu), SIGNAL(airInTubeChanged(bool)),
		pressureButton, SLOT(setChecked(bool)));
	
	connect(&(pexline.cpu), SIGNAL(coolerPumpChanged(bool)),
		coolerPumpButton, SLOT(setChecked(bool)));
	
	connect(&(pexline.cpu), SIGNAL(sgChanged(bool)),
		driveParamSgPositionButton, SLOT(setChecked(bool)));
	
	connect(&(pexline.cpu), SIGNAL(sgChanged(bool)),
		mnemoSgPositionButton, SLOT(setChecked(bool)));

	connect(mnemoSgPositionButton, SIGNAL(clicked(bool)),
		this, SLOT(on_driveParamSgPositionButton_clicked(bool)));

	connect(&(pexline.cpu), SIGNAL(waterModeChanged(int)),
		this, SLOT(setWaterMode(int)));
}	
//	pexline.sgInvertor.connect();		
{		
	connect(&(pexline.sgInvertor.tvConnected()), SIGNAL(trigged(bool)),
		driveParamSgLinkButton, SLOT(setChecked(bool)));
	
	connect(driveParamSgLinkButton, SIGNAL(clicked(bool)),
		&(pexline.sgInvertor), SLOT(setConnected(bool)));
	
	connect(driveParamSgMotorEntry, SIGNAL(valueChanged(double)),
		&(pexline.sgInvertor), SLOT(setFrequency(double)));

	connect(&(pexline.sgInvertor.frequency()), SIGNAL(scaledValueChanged(double)),
		driveParamSgMotorEntry, SLOT(setValue(double)));
		
//	driveParamSgMotorStartButton->setBlinkable(true);
	
	connect(&(pexline.sgInvertor.runStop()), SIGNAL(trigged(bool)),
		driveParamSgMotorStartButton, SLOT(setAlterChecked(bool)));
		
	connect(&(pexline.sgInvertor.runStop()), SIGNAL(trigged(bool)),
		mnemoSgRotateButton, SLOT(setAlterChecked(bool)));
	
	connect(mnemoSgRotateButton, SIGNAL(clicked(bool)),
		this, SLOT(on_driveParamSgMotorStartButton_clicked(bool)));
	
	connect(&(pexline.sgInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		driveParamSgMotorEdit, SLOT(setValue(double)));

	connect(&(pexline.sgInvertor.current()), SIGNAL(stateChanged(int)),
		this, SLOT(onSgCurrentStateChanged(int)));

	connect(&(pexline.sgInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundSgCurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.sgInvertor.current().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundSgCurrentMinEntry, SLOT(setValue(double)));
	
	connect(&(pexline.sgInvertor.current().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundSgCurrentMaxEntry, SLOT(setValue(double)));

	connect(warnBoundSgCurrentMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.sgInvertor), SLOT(setCurrentLoCriticalBound(double)));

	connect(warnBoundSgCurrentMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.sgInvertor), SLOT(setCurrentHiCriticalBound(double)));

}
//	pexline.tuInvertor.connect();		
{		
	connect(&(pexline.tuInvertor.tvConnected()), SIGNAL(trigged(bool)),
		driveParamTuLinkButton, SLOT(setChecked(bool)));
	
	connect(driveParamTuLinkButton, SIGNAL(clicked(bool)),
		&(pexline.tuInvertor), SLOT(setConnected(bool)));
	
	connect(driveParamTuMotorEntry, SIGNAL(valueChanged(double)),
		&(pexline.tuInvertor), SLOT(setFrequency(double)));

	connect(&(pexline.tuInvertor.frequency()), SIGNAL(scaledValueChanged(double)),
		driveParamTuMotorEntry, SLOT(setValue(double)));
		
//	driveParamTuMotorStartButton->setBlinkable(true);
	
	connect(&(pexline.tuInvertor.runStop()), SIGNAL(trigged(bool)),
		driveParamTuMotorStartButton, SLOT(setAlterChecked(bool)));
		
	connect(&(pexline.tuInvertor.runStop()), SIGNAL(trigged(bool)),
		mnemoTuButton, SLOT(setAlterChecked(bool)));

	connect(&(pexline.tuInvertor.runStop()), SIGNAL(trigged(bool)),
		tractorButton, SLOT(setChecked(bool)));

	connect(mnemoTuButton, SIGNAL(clicked(bool)),
		this, SLOT(on_driveParamTuMotorStartButton_clicked(bool)));
	
	connect(tractorButton, SIGNAL(clicked(bool)),
		this, SLOT(on_driveParamTuMotorStartButton_clicked(bool)));

	connect(&(pexline.tuInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		driveParamTuMotorEdit, SLOT(setValue(double)));
	
	connect(&(pexline.tuInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundTuCurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.tuInvertor.current()), SIGNAL(stateChanged(int)),
		this, SLOT(onTuCurrentStateChanged(int)));

	connect(&(pexline.tuInvertor.current().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundTuCurrentMinEntry, SLOT(setValue(double)));
	
	connect(&(pexline.tuInvertor.current().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundTuCurrentMaxEntry, SLOT(setValue(double)));

	connect(warnBoundTuCurrentMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.tuInvertor), SLOT(setCurrentLoCriticalBound(double)));

	connect(warnBoundTuCurrentMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.tuInvertor), SLOT(setCurrentHiCriticalBound(double)));

}

//	pexline.e2pInvertor.connect();		
{		
	connect(&(pexline.e2pPressure.tvConnected()), SIGNAL(trigged(bool)),
		warnBoundPe1PressureLinkButton, SLOT(setChecked(bool)));
	
	connect(warnBoundPe1PressureLinkButton, SIGNAL(clicked(bool)),
		&(pexline.e2pPressure), SLOT(setConnected(bool)));

	connect(&(pexline.e2pInvertor.tvConnected()), SIGNAL(trigged(bool)),
		driveParamPe1LinkButton, SLOT(setChecked(bool)));
	
	connect(driveParamPe1LinkButton, SIGNAL(clicked(bool)),
		&(pexline.e2pInvertor), SLOT(setConnected(bool)));
	
	connect(driveParamPe1MotorEntry, SIGNAL(valueChanged(double)),
		&(pexline.e2pInvertor), SLOT(setFrequency(double)));

	driveParamPe1MotorEntry->setBlinkable(true);
	
	connect(&(pexline.e2pInvertor.frequency()), SIGNAL(scaledValueChanged(double)),
		driveParamPe1MotorEntry, SLOT(setAlterValue(double)));
		
	driveParamPe1MotorStartButton->setBlinkable(true);
	mnemoE2pButton->setBlinkable(true);
	
	connect(&(pexline.e2pInvertor.runStop()), SIGNAL(trigged(bool)),
		driveParamPe1MotorStartButton, SLOT(setAlterChecked(bool)));
		
	connect(&(pexline.e2pInvertor.runStop()), SIGNAL(trigged(bool)),
		mnemoE2pButton, SLOT(setAlterChecked(bool)));

	connect(mnemoE2pButton, SIGNAL(clicked(bool)),
		this, SLOT(on_driveParamPe1MotorStartButton_clicked(bool)));

	connect(&(pexline.e2pInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		driveParamPe1MotorEdit, SLOT(setValue(double)));

	connect(&(pexline.e2pInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		bottomE2PCurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.e2pInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe1CurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.e2pPressure.pressure().currentValue()), SIGNAL(scaledValueChanged(double)),
		bottomE2PPressureEdit, SLOT(setValue(double)));

	connect(&(pexline.e2pPressure.pressure().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe1PressureEdit, SLOT(setValue(double)));

	connect(&(pexline.e2pInvertor.current().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe1CurrentMinEntry, SLOT(setValue(double)));
	
	connect(&(pexline.e2pInvertor.current().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe1CurrentMaxEntry, SLOT(setValue(double)));

	connect(warnBoundPe1CurrentMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e2pInvertor), SLOT(setCurrentLoCriticalBound(double)));

	connect(warnBoundPe1CurrentMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e2pInvertor), SLOT(setCurrentHiCriticalBound(double)));

	connect(&(pexline.e2pPressure.pressure().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe1PressureMinEntry, SLOT(setValue(double)));
	
	connect(&(pexline.e2pPressure.pressure().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe1PressureMaxEntry, SLOT(setValue(double)));

	connect(warnBoundPe1PressureMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e2pPressure), SLOT(setPressureLoCriticalBound(double)));

	connect(warnBoundPe1PressureMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e2pPressure), SLOT(setPressureHiCriticalBound(double)));

///////
	connect(&(pexline.e2pInvertor.current()), SIGNAL(stateChanged(int)),
		this, SLOT(onE2pCurrentStateChanged(int)));
	
	connect(&(pexline.e2pPressure.pressure()), SIGNAL(stateChanged(int)),
		this, SLOT(onE2pPressureStateChanged(int)));

}
//	pexline.e3pInvertor.connect();		
{		
	connect(&(pexline.e3pPressure.tvConnected()), SIGNAL(trigged(bool)),
		warnBoundPe2PressureLinkButton, SLOT(setChecked(bool)));
	
	connect(warnBoundPe2PressureLinkButton, SIGNAL(clicked(bool)),
		&(pexline.e3pPressure), SLOT(setConnected(bool)));

	connect(&(pexline.e3pInvertor.tvConnected()), SIGNAL(trigged(bool)),
		driveParamPe2LinkButton, SLOT(setChecked(bool)));
	
	connect(driveParamPe2LinkButton, SIGNAL(clicked(bool)),
		&(pexline.e3pInvertor), SLOT(setConnected(bool)));
	
	connect(driveParamPe2MotorEntry, SIGNAL(valueChanged(double)),
		&(pexline.e3pInvertor), SLOT(setFrequency(double)));

	driveParamPe2MotorEntry->setBlinkable(true);
	
	connect(&(pexline.e3pInvertor.frequency()), SIGNAL(scaledValueChanged(double)),
		driveParamPe2MotorEntry, SLOT(setAlterValue(double)));
		
	driveParamPe2MotorStartButton->setBlinkable(true);
	mnemoE3pButton->setBlinkable(true);
	
	connect(&(pexline.e3pInvertor.runStop()), SIGNAL(trigged(bool)),
		driveParamPe2MotorStartButton, SLOT(setAlterChecked(bool)));
		
	connect(&(pexline.e3pInvertor.runStop()), SIGNAL(trigged(bool)),
		mnemoE3pButton, SLOT(setAlterChecked(bool)));

	connect(mnemoE3pButton, SIGNAL(clicked(bool)),
		this, SLOT(on_driveParamPe2MotorStartButton_clicked(bool)));

	connect(&(pexline.e3pInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		driveParamPe2MotorEdit, SLOT(setValue(double)));

	connect(&(pexline.e3pInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		bottomE3PCurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.e3pInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe2CurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.e3pPressure.pressure().currentValue()), SIGNAL(scaledValueChanged(double)),
		bottomE3PPressureEdit, SLOT(setValue(double)));

	connect(&(pexline.e3pPressure.pressure().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe2PressureEdit, SLOT(setValue(double)));

	connect(&(pexline.e3pInvertor.current().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe2CurrentMaxEntry, SLOT(setValue(double)));

	connect(&(pexline.e3pInvertor.current().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe2CurrentMinEntry, SLOT(setValue(double)));

	connect(warnBoundPe2CurrentMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3pInvertor), SLOT(setCurrentLoCriticalBound(double)));

	connect(warnBoundPe2CurrentMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3pInvertor), SLOT(setCurrentHiCriticalBound(double)));

	connect(&(pexline.e3pPressure.pressure().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe2PressureMinEntry, SLOT(setValue(double)));
	
	connect(&(pexline.e3pPressure.pressure().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPe2PressureMaxEntry, SLOT(setValue(double)));

	connect(warnBoundPe2PressureMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3pPressure), SLOT(setPressureLoCriticalBound(double)));

	connect(warnBoundPe2PressureMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3pPressure), SLOT(setPressureHiCriticalBound(double)));

///////
	connect(&(pexline.e3pInvertor.current()), SIGNAL(stateChanged(int)),
		this, SLOT(onE3pCurrentStateChanged(int)));
	
	connect(&(pexline.e3pPressure.pressure()), SIGNAL(stateChanged(int)),
		this, SLOT(onE3pPressureStateChanged(int)));
}

//	pexline.e2kInvertor.connect();		
{		
	connect(&(pexline.e2kPressure.tvConnected()), SIGNAL(trigged(bool)),
		warnBoundKl1PressureLinkButton, SLOT(setChecked(bool)));
	
	connect(warnBoundKl1PressureLinkButton, SIGNAL(clicked(bool)),
		&(pexline.e2kPressure), SLOT(setConnected(bool)));

	connect(&(pexline.e2kInvertor.tvConnected()), SIGNAL(trigged(bool)),
		driveParamKl1LinkButton, SLOT(setChecked(bool)));
	
	connect(driveParamKl1LinkButton, SIGNAL(clicked(bool)),
		&(pexline.e2kInvertor), SLOT(setConnected(bool)));
	
	connect(driveParamKl1MotorEntry, SIGNAL(valueChanged(double)),
		&(pexline.e2kInvertor), SLOT(setFrequency(double)));

	driveParamKl1MotorEntry->setBlinkable(true);
	
	connect(&(pexline.e2kInvertor.frequency()), SIGNAL(scaledValueChanged(double)),
		driveParamKl1MotorEntry, SLOT(setAlterValue(double)));

	driveParamKl1MotorStartButton->setBlinkable(true);
	mnemoE2kButton->setBlinkable(true);
	
	connect(&(pexline.e2kInvertor.runStop()), SIGNAL(trigged(bool)),
		driveParamKl1MotorStartButton, SLOT(setAlterChecked(bool)));
	
	connect(&(pexline.e2kInvertor.runStop()), SIGNAL(trigged(bool)),
		mnemoE2kButton, SLOT(setAlterChecked(bool)));

	connect(mnemoE2kButton, SIGNAL(clicked(bool)),
		this, SLOT(on_driveParamKl1MotorStartButton_clicked(bool)));
	
	connect(&(pexline.e2kInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		driveParamKl1MotorEdit, SLOT(setValue(double)));

	connect(&(pexline.e2kInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		bottomE2KCurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.e2kInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl1CurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.e2kPressure.pressure().currentValue()), SIGNAL(scaledValueChanged(double)),
		bottomE2KPressureEdit, SLOT(setValue(double)));

	connect(&(pexline.e2kPressure.pressure().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl1PressureEdit, SLOT(setValue(double)));

	connect(&(pexline.e2kInvertor.current().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl1CurrentMinEntry, SLOT(setValue(double)));

	connect(&(pexline.e2kInvertor.current().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl1CurrentMaxEntry, SLOT(setValue(double)));

	connect(warnBoundKl1CurrentMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e2kInvertor), SLOT(setCurrentLoCriticalBound(double)));

	connect(warnBoundKl1CurrentMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e2kInvertor), SLOT(setCurrentHiCriticalBound(double)));

	connect(&(pexline.e2kPressure.pressure().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl1PressureMinEntry, SLOT(setValue(double)));
	
	connect(&(pexline.e2kPressure.pressure().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl1PressureMaxEntry, SLOT(setValue(double)));

	connect(warnBoundKl1PressureMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e2kPressure), SLOT(setPressureLoCriticalBound(double)));

	connect(warnBoundKl1PressureMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e2kPressure), SLOT(setPressureHiCriticalBound(double)));

///////
	connect(&(pexline.e2kInvertor.current()), SIGNAL(stateChanged(int)),
		this, SLOT(onE2kCurrentStateChanged(int)));
	
	connect(&(pexline.e2kPressure.pressure()), SIGNAL(stateChanged(int)),
		this, SLOT(onE2kPressureStateChanged(int)));

}


//	pexline.e3kInvertor.connect();		
{		
	connect(&(pexline.e3kPressure.tvConnected()), SIGNAL(trigged(bool)),
		warnBoundKl2PressureLinkButton, SLOT(setChecked(bool)));
	
	connect(warnBoundKl2PressureLinkButton, SIGNAL(clicked(bool)),
		&(pexline.e3kPressure), SLOT(setConnected(bool)));

	connect(&(pexline.e3kInvertor.tvConnected()), SIGNAL(trigged(bool)),
		driveParamKl2LinkButton, SLOT(setChecked(bool)));
	
	connect(driveParamKl2LinkButton, SIGNAL(clicked(bool)),
		&(pexline.e3kInvertor), SLOT(setConnected(bool)));
	
	connect(driveParamKl2MotorEntry, SIGNAL(valueChanged(double)),
		&(pexline.e3kInvertor), SLOT(setFrequency(double)));

	driveParamKl2MotorEntry->setBlinkable(true);
	
	connect(&(pexline.e3kInvertor.frequency()), SIGNAL(scaledValueChanged(double)),
		driveParamKl2MotorEntry, SLOT(setAlterValue(double)));
		
	driveParamKl2MotorStartButton->setBlinkable(true);
	mnemoE3kButton->setBlinkable(true);
		
	connect(&(pexline.e3kInvertor.runStop()), SIGNAL(trigged(bool)),
		driveParamKl2MotorStartButton, SLOT(setAlterChecked(bool)));
		
	connect(&(pexline.e3kInvertor.runStop()), SIGNAL(trigged(bool)),
		mnemoE3kButton, SLOT(setAlterChecked(bool)));

	connect(mnemoE3kButton, SIGNAL(clicked(bool)),
		this, SLOT(on_driveParamKl2MotorStartButton_clicked(bool)));
	
	connect(&(pexline.e3kInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		driveParamKl2MotorEdit, SLOT(setValue(double)));

	connect(&(pexline.e3kInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		bottomE3KCurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.e3kInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl2CurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.e3kPressure.pressure().currentValue()), SIGNAL(scaledValueChanged(double)),
		bottomE3KPressureEdit, SLOT(setValue(double)));

	connect(&(pexline.e3kPressure.pressure().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl2PressureEdit, SLOT(setValue(double)));

	connect(&(pexline.e3kInvertor.current().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl2CurrentMinEntry, SLOT(setValue(double)));

	connect(&(pexline.e3kInvertor.current().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl2CurrentMaxEntry, SLOT(setValue(double)));

	connect(warnBoundKl2CurrentMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3kInvertor), SLOT(setCurrentLoCriticalBound(double)));

	connect(warnBoundKl2CurrentMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3kInvertor), SLOT(setCurrentHiCriticalBound(double)));

	connect(&(pexline.e3kPressure.pressure().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl2PressureMinEntry, SLOT(setValue(double)));
	
	connect(&(pexline.e3kPressure.pressure().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundKl2PressureMaxEntry, SLOT(setValue(double)));

	connect(warnBoundKl2PressureMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3kPressure), SLOT(setPressureLoCriticalBound(double)));

	connect(warnBoundKl2PressureMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3kPressure), SLOT(setPressureHiCriticalBound(double)));

///////
	connect(&(pexline.e3kInvertor.current()), SIGNAL(stateChanged(int)),
		this, SLOT(onE3kCurrentStateChanged(int)));
	
	connect(&(pexline.e3kPressure.pressure()), SIGNAL(stateChanged(int)),
		this, SLOT(onE3kPressureStateChanged(int)));

	

}


//	pexline.e3mInvertor.connect();		
{		
	connect(&(pexline.e3mPressure.tvConnected()), SIGNAL(trigged(bool)),
		warnBoundPemPressureLinkButton, SLOT(setChecked(bool)));
	
	connect(warnBoundPemPressureLinkButton, SIGNAL(clicked(bool)),
		&(pexline.e3mPressure), SLOT(setConnected(bool)));

	connect(&(pexline.e3mInvertor.tvConnected()), SIGNAL(trigged(bool)),
		driveParamPemLinkButton, SLOT(setChecked(bool)));
	
	connect(driveParamPemLinkButton, SIGNAL(clicked(bool)),
		&(pexline.e3mInvertor), SLOT(setConnected(bool)));
	
	connect(driveParamPemMotorEntry, SIGNAL(valueChanged(double)),
		&(pexline.e3mInvertor), SLOT(setFrequency(double)));

	driveParamPemMotorEntry->setBlinkable(true);

	connect(&(pexline.e3mInvertor.frequency()), SIGNAL(scaledValueChanged(double)),
		driveParamPemMotorEntry, SLOT(setAlterValue(double)));
		
	driveParamPemMotorStartButton->setBlinkable(true);
	mnemoE3mButton->setBlinkable(true);
	
	connect(&(pexline.e3mInvertor.runStop()), SIGNAL(trigged(bool)),
		driveParamPemMotorStartButton, SLOT(setAlterChecked(bool)));

	connect(&(pexline.e3mInvertor.runStop()), SIGNAL(trigged(bool)),
		mnemoE3mButton, SLOT(setAlterChecked(bool)));

	connect(mnemoE3mButton, SIGNAL(clicked(bool)),
		this, SLOT(on_driveParamPemMotorStartButton_clicked(bool)));
		
	connect(&(pexline.e3mInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		driveParamPemMotorEdit, SLOT(setValue(double)));
		
	connect(&(pexline.e3mInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		bottomE3MCurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.e3mInvertor.current().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundPemCurrentEdit, SLOT(setValue(double)));

	connect(&(pexline.e3mPressure.pressure().currentValue()), SIGNAL(scaledValueChanged(double)),
		bottomE3MPressureEdit, SLOT(setValue(double)));
		
	connect(&(pexline.e3mPressure.pressure().currentValue()), SIGNAL(scaledValueChanged(double)),
		warnBoundPemPressureEdit, SLOT(setValue(double)));

	connect(&(pexline.e3mInvertor.current().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPemCurrentMinEntry, SLOT(setValue(double)));

	connect(&(pexline.e3mInvertor.current().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPemCurrentMaxEntry, SLOT(setValue(double)));

	connect(warnBoundPemCurrentMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3mInvertor), SLOT(setCurrentLoCriticalBound(double)));

	connect(warnBoundPemCurrentMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3mInvertor), SLOT(setCurrentHiCriticalBound(double)));

	connect(&(pexline.e3mPressure.pressure().loCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPemPressureMinEntry, SLOT(setValue(double)));
	
	connect(&(pexline.e3mPressure.pressure().hiCriticalBound()), SIGNAL(scaledValueChanged(double)),
		warnBoundPemPressureMaxEntry, SLOT(setValue(double)));

	connect(warnBoundPemPressureMinEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3mPressure), SLOT(setPressureLoCriticalBound(double)));

	connect(warnBoundPemPressureMaxEntry, SIGNAL(valueChanged(double)), 
		&(pexline.e3mPressure), SLOT(setPressureHiCriticalBound(double)));

///////

	connect(&(pexline.e3mInvertor.current()), SIGNAL(stateChanged(int)),
		this, SLOT(onE3mCurrentStateChanged(int)));
	
	connect(&(pexline.e3mPressure.pressure()), SIGNAL(stateChanged(int)),
		this, SLOT(onE3mPressureStateChanged(int)));

}


	
//////////////////
/// Termos
//////////////////
{
//	pexline.sgZone1.connect();
	egZone1TermoFrame->runStopButton->setBlinkable(false);
	egZone2TermoFrame->runStopButton->setBlinkable(false);
	egZone1TermoFrame->preset->setBlinkable(false);
	egZone2TermoFrame->preset->setBlinkable(false);
	egZone1TermoFrame->connectTermo(&(pexline.sgZone1));
	egZone2TermoFrame->connectTermo(&(pexline.sgZone2));
	
//	pexline.e2kZone1.connect();
	pe1zone1TermoFrame->connectTermo(&(pexline.e2pZone1));
	pe1zone2TermoFrame->connectTermo(&(pexline.e2pZone2));
	pe1zone3TermoFrame->connectTermo(&(pexline.e2pZone3));
	pe1zone4TermoFrame->connectTermo(&(pexline.e2pZone4));
	pe1traceTermoFrame->connectTermo(&(pexline.e2pTrace));

	kl1zone1TermoFrame->connectTermo(&(pexline.e2kZone1));
	kl1zone2TermoFrame->connectTermo(&(pexline.e2kZone2));
	kl1zone3TermoFrame->connectTermo(&(pexline.e2kZone3));
	kl1traceTermoFrame->connectTermo(&(pexline.e2kTrace));	

	pe2zone1TermoFrame->connectTermo(&(pexline.e3pZone1));
	pe2zone2TermoFrame->connectTermo(&(pexline.e3pZone2));
	pe2zone3TermoFrame->connectTermo(&(pexline.e3pZone3));
	pe2zone4TermoFrame->connectTermo(&(pexline.e3pZone4));
	pe2traceTermoFrame->connectTermo(&(pexline.e3pTrace));

	kl2zone1TermoFrame->connectTermo(&(pexline.e3kZone1));
	kl2zone2TermoFrame->connectTermo(&(pexline.e3kZone2));
	kl2zone3TermoFrame->connectTermo(&(pexline.e3kZone3));
	kl2traceTermoFrame->connectTermo(&(pexline.e3kTrace));		

	pemZone1TermoFrame->connectTermo(&(pexline.e3mZone1));
	pemZone2TermoFrame->connectTermo(&(pexline.e3mZone2));
	pemZone3TermoFrame->connectTermo(&(pexline.e3mZone3));
	pemTraceTermoFrame->connectTermo(&(pexline.e3mTrace));		
}
// water leds
{
	connect(&(pexline.cpu), SIGNAL(bathWaterLevel0Changed(bool)), bath0Led, SLOT(set(bool)));
	connect(&(pexline.cpu), SIGNAL(bathWaterLevel1Changed(bool)), bath1Led, SLOT(set(bool)));
	
	connect(&(pexline.cpu), SIGNAL(tankWaterLevel0Changed(bool)), this, SLOT(onTankWaterLevel0Changed(bool)));
	connect(&(pexline.cpu), SIGNAL(tankWaterLevel1Changed(bool)), this, SLOT(onTankWaterLevel1Changed(bool)));
	connect(&(pexline.cpu), SIGNAL(tankWaterLevel2Changed(bool)), this, SLOT(onTankWaterLevel2Changed(bool)));
	
	connect(&(pexline.cpu), SIGNAL(headWaterChanged(bool)), this, SLOT(onSgWaterChanged(bool)));
	connect(&(pexline.cpu), SIGNAL(e2WaterChanged(bool)), this, SLOT(onE2WaterChanged(bool)));
	connect(&(pexline.cpu), SIGNAL(e3WaterChanged(bool)), this, SLOT(onE3WaterChanged(bool)));
	
	connect(&(pexline.cpu), SIGNAL(pressureChanged(bool)), this, SLOT(onPressureChanged(bool)));
	connect(&(pexline.cpu), SIGNAL(foilEncoderChanged(bool)), this, SLOT(onFoilEncoderChanged(bool)));
	
	connect(&(pexline.cpu), SIGNAL(endOfFoilChanged(bool)), this, SLOT(onEndOfFoilChanged(bool)));
	connect(&(pexline.cpu), SIGNAL(primaryReelChanged(bool)), this, SLOT(onPrimaryReelChanged(bool)));
	connect(&(pexline.cpu), SIGNAL(secondaryReelChanged(bool)), this, SLOT(onSecondaryReelChanged(bool)));
	
	connect(&(pexline.cpu), SIGNAL(e2PEmptyChanged(bool)), this, SLOT(onE2PEmptyChanged(bool)));
	connect(&(pexline.cpu), SIGNAL(e2KEmptyChanged(bool)), this, SLOT(onE2KEmptyChanged(bool)));
	connect(&(pexline.cpu), SIGNAL(e3KEmptyChanged(bool)), this, SLOT(onE3KEmptyChanged(bool)));
	connect(&(pexline.cpu), SIGNAL(e3PEmptyChanged(bool)), this, SLOT(onE3PEmptyChanged(bool)));
	connect(&(pexline.cpu), SIGNAL(e3MEmptyChanged(bool)), this, SLOT(onE3MEmptyChanged(bool)));

	connect(&(pexline.cpu), SIGNAL(disconnected()), this, SLOT(onCpuDisconnected()));
	connect(&(pexline.uzg), SIGNAL(disconnected()), this, SLOT(onUzgDisconnected()));
}

}

void MainWindow::setUser(QString user, int userType)
{
	_userType = userType;
	_user = user;
	operLabel->setText(_user);
	
	_acceptedWarningList.clear();
	_unacceptedWarningList.clear();
	_nextWarningId = 0;
	showWarning();
	postWarning(NewUser, -1);
	
	if (_userType < 1)
	{
		if (_debugMode)
			debugAct->trigger();
		
		saveLoadAct->setEnabled(false);
		rasxodAct->setEnabled(false);
		graphAct->setEnabled(false);
		debugAct->setEnabled(false);
		lineStartAct->setEnabled(false);
		svarkaStartAct->setEnabled(false);
		warnBoundsAct->setEnabled(false);
		
		if ((graph == stack->currentIndex()) ||
			(profiles == stack->currentIndex()))
			showMainScreen();
	}
	else
	{
		saveLoadAct->setEnabled(true);
		rasxodAct->setEnabled(true);
		graphAct->setEnabled(true);
		debugAct->setEnabled(true);
		lineStartAct->setEnabled(true);
		svarkaStartAct->setEnabled(true);
		warnBoundsAct->setEnabled(true);
	}
		
	if (_userType == 2)
		userAct->setEnabled(true);
	else
	{
		userAct->setEnabled(false);
		if ( operTable == stack->currentIndex())
			showMainScreen();
	}
	
	on_logUpdateButton_clicked();
	
}

void setupButton(ToggleButton * button)
{
//	button->setOnColor(Qt::red);
//	button->setOffColor(Qt::green);
	button->setOnText(QString("Стоп"));
	button->setOffText(QString("Пуск"));
}

void MainWindow::setupButtons()
{
	setupButton(lineStartSgRotateStartButton);
	setupButton(lineStartUzgStartButton);
	setupButton(lineStartTractorStartButton);
	setupButton(lineStartPushdownTractorStartButton);
	setupButton(lineStartSgPositionStartButton);
	setupButton(lineStartPe1RotateStartButton);
	setupButton(lineStartPe2RotateStartButton);
	setupButton(lineStartKl1RotateStartButton);
	setupButton(lineStartKl2RotateStartButton);
	setupButton(lineStartPemRotateStartButton);
	
	lineStartSgPositionStartButton->setOnText(QString("Исх."));
	lineStartSgPositionStartButton->setOffText(QString("Раб."));
	
	setupButton(svarkaStartSgRotateStartButton);
	setupButton(svarkaStartUzgStartButton);
	setupButton(svarkaStartTractorStartButton);
	setupButton(svarkaStartPushdownTractorStartButton);
	setupButton(svarkaStartSgPositionStartButton);

	svarkaStartSgPositionStartButton->setOnText(QString("Исх."));
	svarkaStartSgPositionStartButton->setOffText(QString("Раб."));

	setupButton(driveParamPe1MotorStartButton);
	setupButton(driveParamPe2MotorStartButton);
	setupButton(driveParamKl1MotorStartButton);
	setupButton(driveParamKl2MotorStartButton);
	setupButton(driveParamPemMotorStartButton);
	setupButton(driveParamTuMotorStartButton);
	setupButton(driveParamSgMotorStartButton);
	setupButton(driveParamSgPositionButton);
	setupButton(driveParamUzgStartButton);

	driveParamSgPositionButton->setOnText(QString("Исх. пол."));
	driveParamSgPositionButton->setOffText(QString("Раб. пол."));

	driveParamUzgStartButton->setOnText(QString("Стоп УЗГ"));
	driveParamUzgStartButton->setOffText(QString("Пуск УЗГ"));
	
	startButton->setOnText(QString("СТОП"));
	startButton->setOffText(QString("ПУСК"));

	uzgRunStopButton->setOnText(QString("СТОП"));
	uzgRunStopButton->setOffText(QString("ПУСК"));

	mnemoE2pButton->setOnText(QString("Стоп\nЭП вн."));
	mnemoE2pButton->setOffText(QString("Пуск\nЭП вн."));
	
	mnemoE2kButton->setOnText(QString("Стоп\nЭК вн."));
	mnemoE2kButton->setOffText(QString("Пуск\nЭК вн."));

	mnemoE3pButton->setOnText(QString("Стоп\nЭП нар."));
	mnemoE3pButton->setOffText(QString("Пуск\nЭП нар."));
	
	mnemoE3kButton->setOnText(QString("Стоп\nЭК нар."));
	mnemoE3kButton->setOffText(QString("Пуск\nЭК нар."));

	mnemoE3mButton->setOnText(QString("Стоп\nЭМ нар."));
	mnemoE3mButton->setOffText(QString("Пуск\nЭМ нар."));

	mnemoSgRotateButton->setOnText(QString("Стоп\nСГ"));
	mnemoSgRotateButton->setOffText(QString("Пуск\nСГ"));

	mnemoSgPositionButton->setOnText(QString("Исх.\nпол."));
	mnemoSgPositionButton->setOffText(QString("Раб.\nпол."));

	mnemoUzgButton->setOnText(QString("Стоп\nУЗГ"));
	mnemoUzgButton->setOffText(QString("Пуск\nУЗГ"));

	mnemoTuButton->setOnText(QString("Стоп\nТУ"));
	mnemoTuButton->setOffText(QString("Пуск\nТУ"));

	lineStartSgRotateStartEntry->setMaxValue(10);
	lineStartUzgStartEntry->setMaxValue(10);
	lineStartTractorStartEntry->setMaxValue(10);
	lineStartPushdownTractorStartEntry->setMaxValue(10);
	lineStartSgPositionStartEntry->setMaxValue(10);
	lineStartPe1RotateStartEntry->setMaxValue(10);
	lineStartKl1RotateStartEntry->setMaxValue(10);
	lineStartPe2RotateStartEntry->setMaxValue(10);
	lineStartKl2RotateStartEntry->setMaxValue(10);
	lineStartPemRotateStartEntry->setMaxValue(10);
	lineStartWaterModeStartEntry->setMaxValue(10);

	svarkaStartSgRotateStartEntry->setMaxValue(10);
	svarkaStartUzgStartEntry->setMaxValue(10);
	svarkaStartTractorStartEntry->setMaxValue(10);
	svarkaStartPushdownTractorStartEntry->setMaxValue(10);
	svarkaStartSgPositionStartEntry->setMaxValue(10);

	lineStartSgRotateStopEntry->setMaxValue(10);
	lineStartUzgStopEntry->setMaxValue(10);
	lineStartTractorStopEntry->setMaxValue(10);
	lineStartPushdownTractorStopEntry->setMaxValue(10);
	lineStartSgPositionStopEntry->setMaxValue(10);
	lineStartPe1RotateStopEntry->setMaxValue(10);
	lineStartKl1RotateStopEntry->setMaxValue(10);
	lineStartPe2RotateStopEntry->setMaxValue(10);
	lineStartKl2RotateStopEntry->setMaxValue(10);
	lineStartPemRotateStopEntry->setMaxValue(10);
	lineStartWaterModeStopEntry->setMaxValue(10);

	svarkaStartSgRotateStopEntry->setMaxValue(10);
	svarkaStartUzgStopEntry->setMaxValue(10);
	svarkaStartTractorStopEntry->setMaxValue(10);
	svarkaStartPushdownTractorStopEntry->setMaxValue(10);
	svarkaStartSgPositionStopEntry->setMaxValue(10);



}


void MainWindow::showMainScreen()
{
	stack->setCurrentIndex(main);
}

void MainWindow::showMenu()
{
	menu->popup(mapToGlobal(menuButton->geometry().bottomLeft()));
}

void MainWindow::showTermo1()
{
	termo1Button1->setChecked(true);
	termo1Button2->setChecked(true);
	termo1Button3->setChecked(true);

	termo2Button1->setChecked(false);
	termo2Button2->setChecked(false);
	termo2Button3->setChecked(false);

	termo3Button1->setChecked(false);
	termo3Button2->setChecked(false);
	termo3Button3->setChecked(false);

	stack->setCurrentIndex(termo1);
}

void MainWindow::showTermo2()
{
	termo1Button1->setChecked(false);
	termo1Button2->setChecked(false);
	termo1Button3->setChecked(false);

	termo2Button1->setChecked(true);
	termo2Button2->setChecked(true);
	termo2Button3->setChecked(true);

	termo3Button1->setChecked(false);
	termo3Button2->setChecked(false);
	termo3Button3->setChecked(false);
	
	stack->setCurrentIndex(termo2);
}


void MainWindow::showTermo3()
{
	termo1Button1->setChecked(false);
	termo1Button2->setChecked(false);
	termo1Button3->setChecked(false);

	termo2Button1->setChecked(false);
	termo2Button2->setChecked(false);
	termo2Button3->setChecked(false);

	termo3Button1->setChecked(true);
	termo3Button2->setChecked(true);
	termo3Button3->setChecked(true);

	stack->setCurrentIndex(termo3);
}

void MainWindow::showDeviceParams()
{
	stack->setCurrentIndex(deviceParams);
}

void MainWindow::showWarnBounds()
{
	stack->setCurrentIndex(warnBounds);
}

void MainWindow::showLineStart()
{
	stack->setCurrentIndex(lineStart);
	showLineStartTime();
	showLineStopTime();
}

void MainWindow::showSvarkaStart()
{
	stack->setCurrentIndex(svarkaStart);
	showSvarkaStartTime();
	showSvarkaStopTime();
}

void MainWindow::showOperTable()
{
	stack->setCurrentIndex(operTable);
	refreshOperTable();
}

void MainWindow::showUzg()
{
	stack->setCurrentIndex(uzg);
}

void MainWindow::showPrinter()
{
	stack->setCurrentIndex(printer);
}

void MainWindow::showLog()
{
	stack->setCurrentIndex(log);
	refreshLogOperTable();
	on_logEndNowButton_clicked();
	logBeginDate->setDate(logEndDate->date().addDays(-1));
	on_logUpdateButton_clicked();
}

void MainWindow::showGraph()
{
	stack->setCurrentIndex(graph);
}

void MainWindow::showProfiles()
{
	stack->setCurrentIndex(profiles);
	profilesUpdateTable();
}

void MainWindow::showScreen(int screenNumber)
{
	switch (screenNumber)
	{
	case main:

		showMainScreen();
		break;
		
	case termo1:

		showTermo1();
		break;

	case termo2:

		showTermo2();
		break;

	case termo3:

		showTermo3();
		break;

	case deviceParams:
	
		showDeviceParams();
		break;
	
	case svarkaStart:
		
		showSvarkaStart();
		break;
	
	case lineStart:
	
		showLineStart();
		break;
	
	case warnBounds:
	
		showWarnBounds();
		break;

	case operTable:

		showOperTable();
		break;

	case uzg:
		showUzg();
		break;
	
	case printer:
		showPrinter();
		break;

	case log:
		showLog();
		break;
		
	case graph:
		showGraph();
		break;

	case profiles:
		showProfiles();
		break;
		
	}
}

void MainWindow::allTermoOn1()
{
	pexline.e2pZone1.run();
	Sleep(100);
	pexline.e2pZone2.run();
	Sleep(100);
	pexline.e2pZone3.run();
	Sleep(100);
	pexline.e2pZone4.run();
	Sleep(100);
	pexline.e2pTrace.run();
	Sleep(100);
	pexline.e2kZone1.run();
	Sleep(100);
	pexline.e2kZone2.run();
	Sleep(100);
	pexline.e2kZone3.run();
	Sleep(100);
	pexline.e2kTrace.run();
}

void MainWindow::allTermoOn2()
{
	pexline.e3pZone1.run();
	pexline.e3pZone2.run();
	pexline.e3pZone3.run();
	pexline.e3pZone4.run();
	pexline.e3pTrace.run();
	
	pexline.e3kZone1.run();
	pexline.e3kZone2.run();
	pexline.e3kZone3.run();
	pexline.e3kTrace.run();
}

void MainWindow::allTermoOn3()
{
	pexline.e3mZone1.run();
	pexline.e3mZone2.run();
	pexline.e3mZone3.run();
	pexline.e3mTrace.run();
	
	pexline.sgZone1.run();
	pexline.sgZone2.run();

}

void MainWindow::allTermoOff1()
{
	pexline.e2pZone1.stop();
	pexline.e2pZone2.stop();
	pexline.e2pZone3.stop();
	pexline.e2pZone4.stop();
	pexline.e2pTrace.stop();
	
	pexline.e2kZone1.stop();
	pexline.e2kZone2.stop();
	pexline.e2kZone3.stop();
	pexline.e2kTrace.stop();
}

void MainWindow::allTermoOff2()
{
	pexline.e3pZone1.stop();
	pexline.e3pZone2.stop();
	pexline.e3pZone3.stop();
	pexline.e3pZone4.stop();
	pexline.e3pTrace.stop();
	
	pexline.e3kZone1.stop();
	pexline.e3kZone2.stop();
	pexline.e3kZone3.stop();
	pexline.e3kTrace.stop();
}

void MainWindow::allTermoOff3()
{
	pexline.e3mZone1.stop();
	pexline.e3mZone2.stop();
	pexline.e3mZone3.stop();
	pexline.e3mTrace.stop();
	
	pexline.sgZone1.stop();
	pexline.sgZone2.stop();
}

void MainWindow::createActions()
{
	lineStartAct = new QAction(QString("Свойства пуска линии"), this);
	connect(lineStartAct, SIGNAL(activated()), this, SLOT(showLineStart()));
	
	svarkaStartAct = new QAction(QString("Свойства пуска сварки"), this);
	connect(svarkaStartAct, SIGNAL(activated()), this, SLOT(showSvarkaStart()));

	deviceParamsAct = new QAction(QString("Параметры приводов"), this);
	connect(deviceParamsAct, SIGNAL(activated()), this, SLOT(showDeviceParams()));
	
	warnBoundsAct = new QAction(QString("Тревожные границы"), this);
	connect(warnBoundsAct, SIGNAL(activated()), this, SLOT(showWarnBounds()));

	termo1Act = new QAction(QString("Температурные режимы внутреннего ЭУ"), this);
	connect(termo1Act, SIGNAL(activated()), this, SLOT(showTermo1()));

	termo2Act = new QAction(QString("Температурные режимы наружного ЭУ"), this);
	connect(termo2Act, SIGNAL(activated()), this, SLOT(showTermo2()));

	termo3Act = new QAction(QString("Температурные режимы маркерного ЭУ и ЭГ"), this);
	connect(termo3Act, SIGNAL(activated()), this, SLOT(showTermo3()));

	uzgAct = new QAction(QString("Настройки генератора"), this);
	connect(uzgAct, SIGNAL(activated()), this, SLOT(showUzg()));

	printerAct = new QAction(QString("Настройки маркирующего устройства"), this);
	connect(printerAct, SIGNAL(activated()), this, SLOT(showPrinter()));

	graphAct = new QAction(QString("Графики"), this);
	connect(graphAct, SIGNAL(activated()), this, SLOT(showGraph()));
	
	rasxodAct = new QAction(QString("Расход материала"), this);
	rasxodAct->setEnabled(false);
	
	logAct = new QAction(QString("Архив событий"), this);
	connect(logAct, SIGNAL(activated()), this, SLOT(showLog()));
	
	userAct = new QAction(QString("Управление доступом"), this);
	connect(userAct, SIGNAL(activated()), this, SLOT(showOperTable()));
	
	saveLoadAct = new QAction(QString("Сохр./загр. настроек"), this);
	connect(saveLoadAct, SIGNAL(activated()), this, SLOT(showProfiles()));
	
	debugAct = new QAction(QString("Режим отладки"), this);
	debugAct->setCheckable(true);
	connect(debugAct, SIGNAL(toggled(bool)), this, SLOT(setDebug(bool)));
	connect(debugAct, SIGNAL(toggled(bool)), stopDebugButton, SLOT(setVisible(bool)));
	connect(debugAct, SIGNAL(toggled(bool)), startButton, SLOT(setHidden(bool)));
	connect(debugAct, SIGNAL(toggled(bool)), lineSvarkaSelectButton, SLOT(setHidden(bool)));
	connect(stopDebugButton, SIGNAL(clicked()), debugAct, SLOT(trigger()));	

	quitAct = new QAction(QString("Выход"), this);
	connect(quitAct, SIGNAL(activated()), this, SLOT(tryQuit()));
}

void MainWindow::setDebug(bool check)
{
	_debugMode = check;
	
	_checkSgWater = !_debugMode;
	_checkE2Water = !_debugMode;
	_checkE3Water = !_debugMode;
	_checkFoilEncoder = _debugMode;

	lineStartSgRotateStartButton->setVisible(false);
	lineStartUzgStartButton->setVisible(false);
	lineStartTractorStartButton->setVisible(false);
	lineStartPushdownTractorStartButton->setVisible(false);
	lineStartSgPositionStartButton->setVisible(false);
	lineStartPe1RotateStartButton->setVisible(false);
	lineStartPe2RotateStartButton->setVisible(false);
	lineStartKl1RotateStartButton->setVisible(false);
	lineStartKl2RotateStartButton->setVisible(false);
	lineStartPemRotateStartButton->setVisible(false);
	
	svarkaStartSgRotateStartButton->setVisible(false);
	svarkaStartUzgStartButton->setVisible(false);
	svarkaStartTractorStartButton->setVisible(false);
	svarkaStartPushdownTractorStartButton->setVisible(false);
	svarkaStartSgPositionStartButton->setVisible(false);

	driveParamPe1MotorStartButton->setVisible(check);
	driveParamPe2MotorStartButton->setVisible(check);
	driveParamKl1MotorStartButton->setVisible(check);
	driveParamKl2MotorStartButton->setVisible(check);
	driveParamPemMotorStartButton->setVisible(check);
	driveParamTuMotorStartButton->setVisible(check);
	driveParamSgMotorStartButton->setVisible(check);
	driveParamSgPositionButton->setVisible(check);
	driveParamUzgStartButton->setVisible(check);
	
	mnemoE2pButton->setVisible(check);
	mnemoE2kButton->setVisible(check);
	mnemoE3kButton->setVisible(check);
	mnemoE3pButton->setVisible(check);
	mnemoE3mButton->setVisible(check);
	mnemoSgRotateButton->setVisible(check);
	mnemoSgPositionButton->setVisible(check);
	mnemoUzgButton->setVisible(check);
	mnemoTuButton->setVisible(check);

	if (check)
		postWarning(DebugModeStart, -1);
	else
		postWarning(DebugModeStop, -1);
}

void MainWindow::createMenu()
{
	menu = new QMenu(this);
	
	menu->setFont(QFont("MS Shell Dlg", 12, QFont::Bold));

	menu->addSeparator();	
	menu->addAction(lineStartAct);
	menu->addSeparator();
	menu->addAction(svarkaStartAct);
	menu->addSeparator();
	menu->addAction(deviceParamsAct);
	menu->addSeparator();
	menu->addAction(warnBoundsAct);
	menu->addSeparator();
	menu->addAction(termo1Act);
	menu->addSeparator();
	menu->addAction(termo2Act);
	menu->addSeparator();
	menu->addAction(termo3Act);
	menu->addSeparator();
	menu->addAction(uzgAct);
	menu->addSeparator();
	menu->addAction(printerAct);
	menu->addSeparator();
	menu->addAction(logAct);
	menu->addSeparator();
	menu->addAction(graphAct);
	menu->addSeparator();
//	menu->addAction(rasxodAct);
//	menu->addSeparator();
	menu->addAction(saveLoadAct);
	menu->addSeparator();
	menu->addAction(debugAct);
	menu->addSeparator();
	menu->addAction(userAct);
	menu->addSeparator();
	menu->addAction(quitAct);
	menu->addSeparator();
}

double	min_diameter = 0.01;

#include <math.h>

void MainWindow::on_driveParamTuMotorEntry_valueChanged()
{
	if (!(driveParamProportionallyButton->isChecked()))
		return;

	double v = driveParamTuMotorEntry->value();
	driveParamSgSpeedEntry->setValue(v, false);

	double d = driveParamSgDiameterEntry->value();

	if (d < min_diameter)
		return;

	double f = v * 1000. / d / M_PI;
	driveParamSgMotorEntry->setValue(f, false, true);
	//driveParamSgMotorEntry->emitValueChanged();
}

void MainWindow::on_driveParamSgMotorEntry_valueChanged()
{
	double f = driveParamSgMotorEntry->value();
	double d = driveParamSgDiameterEntry->value();

	if (d < min_diameter)
		return;
		
	double v = f * d * M_PI / 1000.;
	driveParamSgSpeedEntry->setValue(v, false);

	if (driveParamProportionallyButton->isChecked())
	{
		driveParamTuMotorEntry->setValue(v, false, true);
		//driveParamTuMotorEntry->emitValueChanged();
	}
}

void MainWindow::on_driveParamSgSpeedEntry_valueChanged()
{
	double v = driveParamSgSpeedEntry->value();
	double d = driveParamSgDiameterEntry->value();

	if (d < min_diameter)
		return;

	double f = v * 1000. / d / M_PI;
	driveParamSgMotorEntry->setValue(f, false, true);
	//driveParamSgMotorEntry->emitValueChanged();

	if (driveParamProportionallyButton->isChecked())
	{
		driveParamTuMotorEntry->setValue(v, false, true);
		//driveParamTuMotorEntry->emitValueChanged();
	}	
}

void MainWindow::on_driveParamSgDiameterEntry_valueChanged()
{
	double d = driveParamSgDiameterEntry->value();
	double f = driveParamSgMotorEntry->value();
	
	if (d < min_diameter)
		return;
		
	double v = f * d * M_PI / 1000.;
	driveParamSgSpeedEntry->setValue(v, false);
	
	if (driveParamProportionallyButton->isChecked())
	{
		driveParamTuMotorEntry->setValue(v, false, true);
		//driveParamTuMotorEntry->emitValueChanged();
	}
}

void MainWindow::onCpuDisconnected()
{
	postWarning(CpuDisconnected, 1);
}

void MainWindow::onUzgDisconnected()
{
	postWarning(UzgDisconnected, 1);
}

void MainWindow::onTankWaterLevel0Changed(bool state)
{
	static bool old_state = false;

	tank0Led->set(state);
	
	if (old_state && !state)
		postWarning(TankWaterLevel0, 1);
	
	old_state = state;
}

void MainWindow::onTankWaterLevel1Changed(bool state)
{
	static bool old_state = false;
	
	tank1Led->set(state);
	
	if (old_state && !state)
		postWarning(TankWaterLevel1, 1);
	
	old_state = state;
}

void MainWindow::onTankWaterLevel2Changed(bool state)
{
	static bool old_state = false;
	
	tank2Led->set(state);
	
	if (state && !old_state)
		postWarning(TankWaterLevel2, 1);
	
	old_state = state;
}

void MainWindow::onPressureChanged(bool state)
{
	static bool old_state = false;
	
	airInSystemLed->set(state);
	
	if (old_state && !state)
		postWarning(Pressure, 1);
	
	old_state = state;
}

void MainWindow::onFoilEncoderChanged(bool state)
{
	static bool old_state = false;
	
	if (old_state && !state &&
		(pexline.e2pInvertor.runStop().isOn() ||
		pexline.e2kInvertor.runStop().isOn() ||
		pexline.e3pInvertor.runStop().isOn() ||
		pexline.e3kInvertor.runStop().isOn() ||
		pexline.e3mInvertor.runStop().isOn() ||
		(pexline.uzg.runStop().isOn() && pexline.cpu.headDownOn())))
	{
		postWarning(FoilEncoder, 1);
		if (_checkFoilEncoder)
		{
			pexline.cpu.setHeadDown(false);
			pexline.e2pInvertor.stop();
			pexline.e2kInvertor.stop();
			pexline.cpu.setGermo(true);
			pexline.e3pInvertor.stop();
			pexline.e3kInvertor.stop();
			pexline.e3mInvertor.stop();
			if (2 == pexline.cpu.waterMode())
				pexline.cpu.setWaterMode(1);
			//doAutostart(false);
		}//	pexline.alarmStop();
	};

	old_state = state;
	
	_mnemoMovie.setPaused(!state);
}

void MainWindow::onEndOfFoilChanged(bool state)
{
	static bool old_state = false;

	if (old_state && !state)
	{
		if (foilControlButton->isChecked())
			postWarning(EndOfFoil, 1);
	}
	
	old_state = state;
}

void MainWindow::onPrimaryReelChanged(bool state)
{
	static bool old_state = false;

	if (old_state && !state)
	{
		if (foilControlButton->isChecked())
			postWarning(PrimaryReel, 1);
	}
	
	old_state = state;
}

void MainWindow::onSecondaryReelChanged(bool state)
{
	static bool old_state = false;

	if (old_state && !state)
	{
		if (foilControlButton->isChecked())
			postWarning(SecondaryReel, 1);
	}
	
	old_state = state;
}

void MainWindow::onSgRunStopChanged(bool state)
{

}

void MainWindow::onUzgRunStopChanged(bool state)
{
	uzgRunStopButton->setChecked(state);
	driveParamUzgStartButton->setChecked(state);
	mnemoUzgButton->setChecked(state);
}

void MainWindow::onSgWaterChanged(bool state)
{
	static bool old_state = false;
	
	sgWaterLed->set(state);
	
	if (old_state && !state)
	{
		postWarning(SgWater, 1);
		if (_checkSgWater)
			alarmStop(0);
	}
	
	old_state = state;
}

void MainWindow::onE2WaterChanged(bool state)
{
	static bool old_state = false;
	
	e2WaterLed->set(state);
	
	if (old_state && !state)
	{
		postWarning(E2Water, 1);
		if (_checkE2Water)
			alarmStop(0);
	}
	
	old_state = state;
}

void MainWindow::onE3WaterChanged(bool state)
{
	static bool old_state = false;
	
	e3WaterLed->set(state);
	
	if (old_state && !state)
	{
		postWarning(E3Water, 1);
		if (_checkE3Water)
			alarmStop(0);
	}
	
	old_state = state;
}


void MainWindow::onUzgTemperatureChanged(double t)
{
	uzgTemperatureEdit->setValue(t);

	if (pexline.uzg.runStop().isOn())
	{
		if (t > pexline.uzg.tempAlarm().scaledValue())
			postWarning(UzgOverheat, 1);
			
		if (t > pexline.uzg.tempStop().scaledValue())
		{
			postWarning(UzgStop,  1);
			pexline.uzg.setRunStop(false);
			pexline.cpu.setHeadDown(false);
			pexline.cpu.setGermo(true);
		}
	}
}

void MainWindow::onStopButtonChanged(bool state)
{
	static bool old_state = false;
	
	if (state && !old_state)
		alarmStop();

	old_state = state;
}
	
void MainWindow::onTuCurrentStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		driveParamTuMotorEdit->setGreen();
		warnBoundTuCurrentEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		driveParamTuMotorEdit->setRed();
		warnBoundTuCurrentEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(TuOvercurrent, 1);
			if (!_debugMode  && warnBoundTuCurrentButton->isChecked())
				alarmStop(0);
		}
	}

	old_state = state;
}

void MainWindow::onSgCurrentStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		driveParamSgMotorEdit->setGreen();
		warnBoundSgCurrentEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		driveParamSgMotorEdit->setRed();
		warnBoundSgCurrentEdit->setRed();
		
		if ((old_state != state) && (warnBoundSgCurrentButton->isChecked()))
		{
			postWarning(SgOvercurrent, 1);
			pexline.cpu.setHeadDown(false);
			pexline.sgInvertor.stop();
			pexline.cpu.setGermo(true);
//			pexline.alarmStop();
		}
	}
	
	old_state = state;
}

void MainWindow::onE2pCurrentStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		bottomE2PCurrentEdit->setGreen();
		driveParamPe1MotorEdit->setGreen();
		warnBoundPe1CurrentEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		bottomE2PCurrentEdit->setRed();
		driveParamPe1MotorEdit->setRed();
		warnBoundPe1CurrentEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(E2pOvercurrent, 1);
			if (!_debugMode && warnBoundPe1CurrentButton->isChecked())
				alarmStop(0);
		}
	}

	old_state = state;
}

void MainWindow::onE2kCurrentStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		bottomE2KCurrentEdit->setGreen();
		driveParamKl1MotorEdit->setGreen();
		warnBoundKl1CurrentEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		bottomE2KCurrentEdit->setRed();
		driveParamKl1MotorEdit->setRed();
		warnBoundKl1CurrentEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(E2kOvercurrent, 1);
			if (!_debugMode && warnBoundKl1CurrentButton->isChecked())
				alarmStop(0);
		}
	}
	
	old_state = state;

}

void MainWindow::onE3pCurrentStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		bottomE3PCurrentEdit->setGreen();
		driveParamPe2MotorEdit->setGreen();
		warnBoundPe2CurrentEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		bottomE3PCurrentEdit->setRed();
		driveParamPe2MotorEdit->setRed();
		warnBoundPe2CurrentEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(E3pOvercurrent, 1);
			if (!_debugMode && warnBoundPe2CurrentButton->isChecked())
				alarmStop(0);
		}
	}
	old_state = state;
}

void MainWindow::onE3kCurrentStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		bottomE3KCurrentEdit->setGreen();
		driveParamKl2MotorEdit->setGreen();
		warnBoundKl2CurrentEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		bottomE3KCurrentEdit->setRed();
		driveParamKl2MotorEdit->setRed();
		warnBoundKl2CurrentEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(E3kOvercurrent, 1);
			if (!_debugMode && warnBoundKl2CurrentButton->isChecked())
				alarmStop(0);
		}
	}

	old_state = state;
}

void MainWindow::onE3mCurrentStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		bottomE3MCurrentEdit->setGreen();
		driveParamPemMotorEdit->setGreen();
		warnBoundPemCurrentEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		bottomE3MCurrentEdit->setRed();
		driveParamPemMotorEdit->setRed();
		warnBoundPemCurrentEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(E3mOvercurrent, 1);
			if (!_debugMode && warnBoundPemCurrentButton->isChecked())
				alarmStop(0);
		}
		
	}

	old_state = state;

}

void MainWindow::onE2pPressureStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		//bottomE2PPressureEdit->setGreen();
		warnBoundPe1PressureEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		//bottomE2PPressureEdit->setRed();
		warnBoundPe1PressureEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(E2pOverpressure, 1);
			if (!_debugMode && warnBoundPe1PressureButton->isChecked())
				alarmStop(0);
		}
	}
		
	old_state = state;

}
void MainWindow::onE2kPressureStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		//bottomE2KPressureEdit->setGreen();
		warnBoundKl1PressureEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		//bottomE2KPressureEdit->setRed();
		warnBoundKl1PressureEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(E2kOverpressure, 1);
			if (!_debugMode && warnBoundKl1PressureButton->isChecked())
				alarmStop(0);
		}
	}
		
	old_state = state;
}

void MainWindow::onE3pPressureStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		//bottomE3PPressureEdit->setGreen();
		warnBoundPe2PressureEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		//bottomE3PPressureEdit->setRed();
		warnBoundPe2PressureEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(E3pOverpressure, 1);
			if (!_debugMode && warnBoundPe2PressureButton->isChecked())
				alarmStop(0);
		}
	}
	old_state = state;
}

void MainWindow::onE3kPressureStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		//bottomE3KPressureEdit->setGreen();
		warnBoundKl2PressureEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		//bottomE3KPressureEdit->setRed();
		warnBoundKl2PressureEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(E3kOverpressure, 1);
			if (!_debugMode && warnBoundKl2PressureButton->isChecked())
				alarmStop(0);
		}
	}
		
	old_state = state;
}

void MainWindow::onE3mPressureStateChanged(int state)
{
	static int old_state = RangeParameter::Ok;
	
	if (RangeParameter::Ok == state)
	{
		//bottomE3MPressureEdit->setGreen();
		warnBoundPemPressureEdit->setGreen();
	}
	else if (RangeParameter::Critical == state)
	{
		//bottomE3MPressureEdit->setRed();
		warnBoundPemPressureEdit->setRed();
		
		if (old_state != state)
		{
			postWarning(E3mOverpressure, 1);
			if (!_debugMode && warnBoundPemPressureButton->isChecked())
				alarmStop(0);
		}
	}
		
	old_state = state;
}

void MainWindow::onE2PEmptyChanged(bool state)
{
	static int old_state = false;
	
	if (old_state && !state)
		postWarning(E2PEmpty, 1);
	
	old_state = state;
}

void MainWindow::onE2KEmptyChanged(bool state)
{
	static int old_state = false;
	
	if (old_state && !state)
		postWarning(E2KEmpty, 1);
	
	old_state = state;
}

void MainWindow::onE3MEmptyChanged(bool state)
{
	static int old_state = false;
	
	if (old_state && !state)
		postWarning(E3MEmpty, 1);
	
	old_state = state;
}

void MainWindow::onE3PEmptyChanged(bool state)
{
	static int old_state = false;
	
	if (old_state && !state)
		postWarning(E3PEmpty, 1);
	
	old_state = state;
}

void MainWindow::onE3KEmptyChanged(bool state)
{
	static int old_state = false;
	
	if (old_state && !state)
		postWarning(E3KEmpty, 1);
	
	old_state = state;
}

void MainWindow::fillWarningMap()
{
	_nextWarningId = 0;
	_acceptedWarningList.clear();
	_unacceptedWarningList.clear();
	_warningTextMap.clear();
	
	_warningTextMap[PrinterDisconnected] = QString("Ошибка связи с маркирующим устройством");
	_warningTextMap[UzgDisconnected] = QString("Ошибка связи с УЗГ");
	_warningTextMap[CpuDisconnected] = QString("Ошибка связи с ЦПУ");
	_warningTextMap[TankWaterLevel0] = QString("Вода в баке ниже уровня 0");
	_warningTextMap[TankWaterLevel1] = QString("Вода в баке ниже уровня 1");
	_warningTextMap[TankWaterLevel2] = QString("Вода в баке выше уровня 2");
	_warningTextMap[Pressure] = QString("Давление воздуха в системе ниже нормы");
	_warningTextMap[FoilEncoder] = QString("Нет движения фольги");
	_warningTextMap[SgWater] = QString("Нет охлаждения СГ");
	_warningTextMap[E2Water] = QString("Нет охлаждения Э2");
	_warningTextMap[E3Water] = QString("Нет охлаждения Э3");
	_warningTextMap[AirInTube] = QString("Подача воздуха в трубу выключена");
	_warningTextMap[GermoBegin] = QString("Каретка герметизации не в положении \"НАЧАЛО\"");

	_warningTextMap[SgDisconnected] = QString("Ошибка связи с инвертором СГ");
	_warningTextMap[TuDisconnected] = QString("Ошибка связи с инвертором ТУ");
	_warningTextMap[E2pDisconnected] = QString("Ошибка связи с инвертором внутр. ЭП");
	_warningTextMap[E2kDisconnected] = QString("Ошибка связи с инвертором внутр. ЭК");
	_warningTextMap[E3pDisconnected] = QString("Ошибка связи с инвертором нар. ЭП");
	_warningTextMap[E3kDisconnected] = QString("Ошибка связи с инвертором нар. ЭК");
	_warningTextMap[E3mDisconnected] = QString("Ошибка связи с инвертором нар. ЭМ");
	_warningTextMap[UzgBusy] = QString("УЗГ занят");
	_warningTextMap[UzgOverheat] = QString("Перегрев УЗГ");
	_warningTextMap[SgOvercurrent] = QString("Превышение допустимого тока СГ");
	_warningTextMap[TuOvercurrent] = QString("Превышение допустимого тока ТУ");
	_warningTextMap[E2pOvercurrent] = QString("Превышение допустимого тока внутр. ЭП");
	_warningTextMap[E2kOvercurrent] = QString("Превышение допустимого тока внутр. ЭК");
	_warningTextMap[E3pOvercurrent] = QString("Превышение допустимого тока нар. ЭП");
	_warningTextMap[E3kOvercurrent] = QString("Превышение допустимого тока нар. ЭК");
	_warningTextMap[E3mOvercurrent] = QString("Превышение допустимого тока нар. ЭМ");
	_warningTextMap[E2pOverpressure] = QString("Превышение допустимого давления внутр. ЭП");
	_warningTextMap[E2kOverpressure] = QString("Превышение допустимого давления внутр. ЭК");
	_warningTextMap[E3pOverpressure] = QString("Превышение допустимого давления нар. ЭП");
	_warningTextMap[E3kOverpressure] = QString("Превышение допустимого давления нар. ЭК");
	_warningTextMap[E3mOverpressure] = QString("Превышение допустимого давления нар. ЭМ");

	_warningTextMap[UzgStop] = QString("УЗГ остановлен");
	_warningTextMap[AlarmStop] = QString("АВАРИЙНЫЙ ОСТАНОВ");

	_warningTextMap[NewUser] = QString("Начало работы оператора");
	
	_warningTextMap[AutostopLine] = QString("Автоостанов линии");
	_warningTextMap[AutostartLine] = QString("Автозапуск линии");
	_warningTextMap[AutostartLineOk] = QString("Автозапуск линии выполнен");
	_warningTextMap[AutostartLineCancel] = QString("Автозапуск линии отменен");
	_warningTextMap[AutostopSvarka] = QString("Автоостанов сварки");
	_warningTextMap[AutostartSvarka] = QString("Автозапуск сварки");
	_warningTextMap[AutostartSvarkaOk] = QString("Автозапуск сварки выполнен");
	_warningTextMap[AutostartSvarkaCancel] = QString("Автозапуск сварки отменен");
	
	_warningTextMap[E2pTemperature] = QString("Температура внутр. ЭП вне рабочей зоны");
	_warningTextMap[E2kTemperature] = QString("Температура внутр. ЭК вне рабочей зоны");
	_warningTextMap[E3kTemperature] = QString("Температура нар. ЭК вне рабочей зоны");
	_warningTextMap[E3pTemperature] = QString("Температура нар. ЭП вне рабочей зоны");
	_warningTextMap[E3mTemperature] = QString("Температура нар. ЭМ вне рабочей зоны");
	_warningTextMap[SgTemperature] = QString("Температура СГ вне рабочей зоны");
	
	_warningTextMap[DebugModeStart] = QString("Режим отладки включен");
	_warningTextMap[DebugModeStop] = QString("Режим отладки выключен");

	_warningTextMap[PrimaryReel] = QString("Заканчивается фольга в нижней бобине");
	_warningTextMap[SecondaryReel] = QString("Заканчивается фольга в верхней бобине");
	_warningTextMap[EndOfFoil] = QString("Фольга смотана в накопитель");
	
	_warningTextMap[Quit] = QString("Завершение работы");
	
	_warningTextMap[E2PEmpty] = QString("Пневмозагрузчик внутр. ЭП пуст");
	_warningTextMap[E2KEmpty] = QString("Пневмозагрузчик внутр. ЭК пуст");
	_warningTextMap[E3KEmpty] = QString("Пневмозагрузчик нар. ЭК пуст");
	_warningTextMap[E3PEmpty] = QString("Пневмозагрузчик нар. ЭП пуст");
	_warningTextMap[E3MEmpty] = QString("Пневмозагрузчик нар. ЭМ пуст");
}
void MainWindow::alarmStop(int sirenMode)
{
	postWarning(AlarmStop, sirenMode);
	startButton->setChecked(false);
	startButton->setBlink(false);
	_lineStarted = false;
	pexline.alarmStop();
}
bool MainWindow::autostartCheck(int check)
{
	bool result = false;
	
	switch(check)
	{
	case Autostart::AirInSystem:
		result = pexline.cpu.pressureOn();
		
		if (!result)
			postWarning(Pressure);
			
		break;

	case Autostart::SgWater:
		result = pexline.cpu.headWaterOn();

		if (!result)
			postWarning(SgWater);
			
		break;

	case Autostart::SgTemperature:
		result = pexline.sgZone1.temperature().ok() && pexline.sgZone2.temperature().ok();

		if (!result)
			postWarning(SgTemperature);
			
		break;

	case Autostart::UzgReady:
		result = pexline.uzg.busy().isOff() && pexline.uzg.runStop().isOff();
		
		if (!result)
			postWarning(UzgBusy);
		else if (!(result = (pexline.uzg.temperature().scaledValue() < pexline.uzg.tempAlarm().scaledValue())))
			postWarning(UzgOverheat);
		
		break;
		
	case Autostart::E2Water:
		result =  pexline.cpu.e2WaterOn();

		if (!result)
			postWarning(E2Water);
			
		break;

	case Autostart::E3Water:
		result = pexline.cpu.e3WaterOn();
		
		if (!result)
			postWarning(E3Water);
			
		break;

	case Autostart::E2pTemperature:
		result = pexline.e2pZone1.temperature().ok() &&
			pexline.e2pZone2.temperature().ok() &&
			pexline.e2pZone3.temperature().ok() &&
			pexline.e2pZone4.temperature().ok() &&
			pexline.e2pTrace.temperature().ok();
	
		if (!result)
			postWarning(E2pTemperature);
			
		break;
	
	case Autostart::E2kTemperature:
		result = pexline.e2kZone1.temperature().ok() &&
			pexline.e2kZone2.temperature().ok() &&
			pexline.e2kZone3.temperature().ok() &&
			pexline.e2kTrace.temperature().ok();

		if (!result)
			postWarning(E2kTemperature);
			
		break;

	case Autostart::E3pTemperature:
		result = pexline.e3pZone1.temperature().ok() &&
			pexline.e3pZone2.temperature().ok() &&
			pexline.e3pZone3.temperature().ok() &&
			pexline.e3pZone4.temperature().ok() &&
			pexline.e3pTrace.temperature().ok();
			
		if (!result)
			postWarning(E3pTemperature);
			
		break;

	case Autostart::E3kTemperature:
		result = pexline.e3kZone1.temperature().ok() &&
			pexline.e3kZone2.temperature().ok() &&
			pexline.e3kZone3.temperature().ok() &&
			pexline.e3kTrace.temperature().ok();

		if (!result)
			postWarning(E3kTemperature);
			
		break;

	case Autostart::E3mTemperature:
		result = pexline.e3mZone1.temperature().ok() &&
			pexline.e3mZone2.temperature().ok() &&
			pexline.e3mZone3.temperature().ok() &&
			pexline.e3mTrace.temperature().ok();

		if (!result)
			postWarning(E3mTemperature);
			
		break;

	default:
		result = false;
		break;
	}
	
	return result;
}
bool MainWindow::execute(int action, bool start)
{
	switch (action)
	{
	case Autostart::Start:
		return true;
		
	case Autostart::MovementDetect:
		_checkFoilEncoder = start;
		return true;
	
	case Autostart::SgRotate:
		return on_driveParamSgMotorStartButton_clicked(start);
	
	case Autostart::Uzg:
		return on_driveParamUzgStartButton_clicked(start);
	
	case Autostart::TuMotor:
		return on_driveParamTuMotorStartButton_clicked(start);
		
	case Autostart::TuPushdown:
		return on_pushdownTractorButton_clicked(start);
	
	case Autostart::SgPosition:
		return on_driveParamSgPositionButton_clicked(start);
		
	case Autostart::E2pMotor:
		return on_driveParamPe1MotorStartButton_clicked(start);
		
	case Autostart::E2kMotor:
		return on_driveParamKl1MotorStartButton_clicked(start);
		
	case Autostart::E3pMotor:
		return on_driveParamPe2MotorStartButton_clicked(start);
		
	case Autostart::E3kMotor:
		return on_driveParamKl2MotorStartButton_clicked(start);
		
	case Autostart::E3mMotor:
		return on_driveParamPemMotorStartButton_clicked(start);
	
	case Autostart::Bath:
		if (start)
			pexline.cpu.setWaterMode(2);
		else if (pexline.cpu.waterMode() == 2)
			pexline.cpu.setWaterMode(1);
		return true;
	
	default:
		return false;
	}
}

#include "list"
using namespace std;

struct ProgramItem
{
	int		action;
	double	time;
	
	ProgramItem(int a, double t) : action(a), time(t) {};
	bool operator<(const ProgramItem & a) const
	{
		return time < a.time;
	};
};

bool MainWindow::doAutostart(bool start)
{
	static bool	startLine = true;
	bool result = true;
	
	list<ProgramItem>	program;
	
	_lineStarted = start;

	startButton->setBlink(true);
	
	if (start)
        {
		if (QString("Сварка") == lineSvarkaSelectButton->text())
			startLine = false;
		else
			startLine = true;
        }

	if (start)
	{
		if (startLine)
		{
			postWarning(AutostartLine, -1);

			// do checks
			
			_checkFoilEncoder = lineStartMoveDetectButton->isChecked();
			
			if (lineStartSgWaterButton->isChecked() && result)
				result = autostartCheck(Autostart::SgWater);
			else
				autostartCheck(Autostart::SgWater);

			_checkSgWater = lineStartSgWaterButton->isChecked();
		
			if (lineStartInnerWaterButton->isChecked() && result)
				result = autostartCheck(Autostart::E2Water);
			else
				autostartCheck(Autostart::E2Water);

			_checkE2Water = lineStartInnerWaterButton->isChecked();

			if (lineStartOuterWaterButton->isChecked() && result)
				result = autostartCheck(Autostart::E3Water);
			else
				autostartCheck(Autostart::E3Water);

			_checkE3Water = lineStartOuterWaterButton->isChecked();

			if (lineStartEgTempButton->isChecked() && result)
				result = autostartCheck(Autostart::SgTemperature);
			else
				autostartCheck(Autostart::SgTemperature);
			
			if (lineStartSystemAirButton->isChecked() && result)
				result = autostartCheck(Autostart::AirInSystem);
			else
				autostartCheck(Autostart::AirInSystem);
		
			if (lineStartUzgReadyButton->isChecked() && result)
				result = autostartCheck(Autostart::UzgReady);
			else
				autostartCheck(Autostart::UzgReady);

			if (lineStartPe1TempButton->isChecked() && result)
				result = autostartCheck(Autostart::E2pTemperature);
			else
				autostartCheck(Autostart::E2pTemperature);

			if (lineStartPe2TempButton->isChecked() && result)
				result = autostartCheck(Autostart::E3pTemperature);
			else
				autostartCheck(Autostart::E3pTemperature);

			if (lineStartPemTempButton->isChecked() && result)
				result = autostartCheck(Autostart::E3mTemperature);
			else
				autostartCheck(Autostart::E3mTemperature);

			if (lineStartKl1TempButton->isChecked() && result)
				result = autostartCheck(Autostart::E2kTemperature);
			else
				autostartCheck(Autostart::E2kTemperature);

			if (lineStartKl2TempButton->isChecked() && result)
				result = autostartCheck(Autostart::E3kTemperature);
			else
				autostartCheck(Autostart::E3kTemperature);

			if (!result)
			{
				startButton->setChecked(false);
				startButton->setBlink(false);
				postWarning(AutostartLineCancel, -1);
				_lineStarted = false;
				return false;
			}
			
			if (lineStartSgRotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::SgRotate, lineStartSgRotateStartEntry->value()));
				
			if (lineStartUzgButton->isChecked())
				program.push_back(ProgramItem(Autostart::Uzg, lineStartUzgStartEntry->value()));
		
			if (lineStartTractorButton->isChecked())
				program.push_back(ProgramItem(Autostart::TuMotor, lineStartTractorStartEntry->value()));

			if (lineStartPushdownTractorButton->isChecked())
				program.push_back(ProgramItem(Autostart::TuPushdown, lineStartPushdownTractorStartEntry->value()));

			if (lineStartSgPositionButton->isChecked())
				program.push_back(ProgramItem(Autostart::SgPosition, lineStartSgPositionStartEntry->value()));		

			if (lineStartWaterModeButton->isChecked())
				program.push_back(ProgramItem(Autostart::Bath, lineStartWaterModeStartEntry->value()));		

			if (lineStartPe1RotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::E2pMotor, lineStartPe1RotateStartEntry->value()));		

			if (lineStartPe2RotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::E3pMotor, lineStartPe2RotateStartEntry->value()));		

			if (lineStartPemRotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::E3mMotor, lineStartPemRotateStartEntry->value()));		

			if (lineStartKl1RotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::E2kMotor, lineStartKl1RotateStartEntry->value()));		

			if (lineStartKl2RotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::E3kMotor, lineStartKl2RotateStartEntry->value()));		

		}
		else
		{
			postWarning(AutostartSvarka, -1);
			
			// do checks
			
			_checkE2Water = false;
			_checkE3Water = false;
			
			_checkFoilEncoder = svarkaStartMoveDetectButton->isChecked();
			
			if (svarkaStartSgWaterButton->isChecked() && result)
				result = autostartCheck(Autostart::SgWater);
			else
				autostartCheck(Autostart::SgWater);
				
			_checkSgWater = svarkaStartSgWaterButton->isChecked();
				
			if (svarkaStartSgTempButton->isChecked() && result)
				result = autostartCheck(Autostart::SgTemperature);
			else
				autostartCheck(Autostart::SgTemperature);
			
			if (svarkaStartSystemAirButton->isChecked() && result)
				result = autostartCheck(Autostart::AirInSystem);
			else
				autostartCheck(Autostart::AirInSystem);
		
			if (svarkaStartUzgReadyButton->isChecked() && result)
				result = autostartCheck(Autostart::UzgReady);
			else
				autostartCheck(Autostart::UzgReady);

			if (!result)
			{
				startButton->setChecked(false);
				startButton->setBlink(false);
				postWarning(AutostartSvarkaCancel, -1);
				_lineStarted = false;
				return false;
			}
			
			if (svarkaStartSgRotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::SgRotate, svarkaStartSgRotateStartEntry->value()));
				
			if (svarkaStartUzgButton->isChecked())
				program.push_back(ProgramItem(Autostart::Uzg, svarkaStartUzgStartEntry->value()));
		
			if (svarkaStartTractorButton->isChecked())
				program.push_back(ProgramItem(Autostart::TuMotor, svarkaStartTractorStartEntry->value()));

			if (svarkaStartPushdownTractorButton->isChecked())
				program.push_back(ProgramItem(Autostart::TuPushdown, svarkaStartPushdownTractorStartEntry->value()));

			if (svarkaStartSgPositionButton->isChecked())
				program.push_back(ProgramItem(Autostart::SgPosition, svarkaStartSgPositionStartEntry->value()));
		}
	}
	else
	{
		if (startLine)
		{
			postWarning(AutostopLine, -1);

			if (lineStartSgRotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::SgRotate, lineStartSgRotateStopEntry->value()));
				
			if (lineStartUzgButton->isChecked())
				program.push_back(ProgramItem(Autostart::Uzg, lineStartUzgStopEntry->value()));
		
			if (lineStartTractorButton->isChecked())
				program.push_back(ProgramItem(Autostart::TuMotor, lineStartTractorStopEntry->value()));

			if (lineStartPushdownTractorButton->isChecked())
				program.push_back(ProgramItem(Autostart::TuPushdown, lineStartPushdownTractorStopEntry->value()));

			if (lineStartSgPositionButton->isChecked())
				program.push_back(ProgramItem(Autostart::SgPosition, lineStartSgPositionStopEntry->value()));		

			//if (lineStartWaterModeButton->isChecked())
				program.push_back(ProgramItem(Autostart::Bath, lineStartWaterModeStopEntry->value()));		

			if (lineStartPe1RotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::E2pMotor, lineStartPe1RotateStopEntry->value()));		

			if (lineStartPe2RotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::E3pMotor, lineStartPe2RotateStopEntry->value()));		

			if (lineStartPemRotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::E3mMotor, lineStartPemRotateStopEntry->value()));		

			if (lineStartKl1RotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::E2kMotor, lineStartKl1RotateStopEntry->value()));		

			if (lineStartKl2RotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::E3kMotor, lineStartKl2RotateStopEntry->value()));		

		}
		else
		{
			postWarning(AutostopSvarka, -1);

			if (svarkaStartSgRotateButton->isChecked())
				program.push_back(ProgramItem(Autostart::SgRotate, svarkaStartSgRotateStopEntry->value()));
				
			if (svarkaStartUzgButton->isChecked())
				program.push_back(ProgramItem(Autostart::Uzg, svarkaStartUzgStopEntry->value()));
		
			if (svarkaStartTractorButton->isChecked())
				program.push_back(ProgramItem(Autostart::TuMotor, svarkaStartTractorStopEntry->value()));

			if (svarkaStartPushdownTractorButton->isChecked())
				program.push_back(ProgramItem(Autostart::TuPushdown, svarkaStartPushdownTractorStopEntry->value()));

			if (svarkaStartSgPositionButton->isChecked())
				program.push_back(ProgramItem(Autostart::SgPosition, svarkaStartSgPositionStopEntry->value()));
		}
	}

	program.sort();
		
	list<ProgramItem>::iterator lb = program.begin();
	list<ProgramItem>::iterator le = program.end();
	list<ProgramItem>::iterator li;
	list<ProgramItem>::iterator tmpi;
	
	int delay = 0;
	
	for (li = lb; li != le; li++)
	{
		tmpi = li;
		tmpi++;
		
		if (le != tmpi)
			delay = (int)((tmpi->time - li->time) * 1000);
		else
			delay = 0;
		
		result = execute(li->action, start);
		
		if (start && !result)
		{
			execute(li->action, false);
			do
			{
				li--;
				execute(li->action, false);
			} while (li != lb);
			
			startButton->setChecked(false);
			startButton->setBlink(false);
			if (startLine)
				postWarning(AutostartLineCancel, -1);
			else
				postWarning(AutostartSvarkaCancel, -1);
			_lineStarted = false;
			return false;
		}	
		
		Sleep(delay);
		
		if ((Autostart::Uzg == li->action) && start)
			while (pexline.uzg.busy().isOn())
				;
	}

	if (start)
        {
		if (startLine)
			postWarning(AutostartLineOk, -1);
		else
			postWarning(AutostartSvarkaOk, -1);
        }

	startButton->setChecked(start);
	startButton->setBlink(false);
	_lineStarted = start;
	return true;
}

void MainWindow::on_logBeginNowButton_clicked()
{
	logBeginDate->setDate(QDate::currentDate());
	logBeginTime->setTime(QTime::currentTime());
}

void MainWindow::on_logEndNowButton_clicked()
{
	logEndDate->setDate(QDate::currentDate());
	logEndTime->setTime(QTime::currentTime());
}

void MainWindow::on_graphBeginNowButton_clicked()
{
	graphBeginDate->setDate(QDate::currentDate());
	graphBeginTime->setTime(QTime::currentTime());
}

void MainWindow::on_graphEndNowButton_clicked()
{
	graphEndDate->setDate(QDate::currentDate());
	graphEndTime->setTime(QTime::currentTime());
}

void MainWindow::refreshLogOperTable()
{
	logOperTable->clear();
	logOperTable->setColumnCount(1);
	logOperTable->setRowCount(0);
	
	logOperTable->verticalHeader()->hide();
	logOperTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	logOperTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QStringList	headers;
	headers << QString("Оператор");
	logOperTable->setHorizontalHeaderLabels(headers);

	int row = 0;

	QSqlQuery query("select user from users");
	query.exec();

	while (query.next())
	{
		logOperTable->insertRow(row);
		logOperTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
		
		if (_user == logOperTable->item(row, 0)->text())
			logOperTable->setCurrentCell(row, 0);
		
		row++;
	}

	logOperTable->setColumnWidth(0, logOperTable->width()-5);
	
	updateLogUpDownOperButtons();

}

void MainWindow::on_logUpOperButton_clicked()
{
	int row = logOperTable->currentRow();
	
	if (0 < row)
		row--;
		
	logOperTable->setCurrentCell(row, 0);
	
//	updateLogUpDownOperButtons();
}

void MainWindow::on_logDownOperButton_clicked()
{
	int row = logOperTable->currentRow();
	
	if (logOperTable->rowCount() - 1 > row)
		row++;
		
	logOperTable->setCurrentCell(row, 0);
	
//	updateLogUpDownOperButtons();
}

void MainWindow::updateLogUpDownOperButtons()
{
	logUpOperButton->setEnabled((0 < logOperTable->currentRow())
		&& !(logAllOperButton->isChecked()));
	logDownOperButton->setEnabled((logOperTable->rowCount() - 1 > logOperTable->currentRow()) &&
		!(logAllOperButton->isChecked()));
}
void MainWindow::on_logListTable_currentCellChanged(int curRow, int curCol, int prevRow, int prevCol)
{
	int rowCount = logListTable->rowCount();

	if (rowCount)
		logBottomLine->setText(QString("Запись %1 из %2").arg(curRow + 1).arg(rowCount));
	else
		logBottomLine->setText(QString("Нет данных"));
	
	QTableWidgetItem * item = logListTable->item(curRow, 2);
	
	if (item)
		logLineEdit->setText(item->text());
	else
		logLineEdit->clear();

	logUpListButton->setEnabled(curRow > 0);
	logDownListButton->setEnabled(curRow < rowCount - 1);
}

void MainWindow::on_logUpListButton_clicked()
{
	int row = logListTable->currentRow();
	
	if (row > 0)
		row--;
		
	logListTable->setCurrentCell(row, 0);
}

void MainWindow::on_logDownListButton_clicked()
{
	int row = logListTable->currentRow();
	
	if (row < logListTable->rowCount() - 1)
		row++;
		
	logListTable->setCurrentCell(row, 0);
}

void MainWindow::on_graphFromBuffer1Button_clicked()
{
	graphBeginDate->setDate(_logBuffer1DateTime.date());
	graphBeginTime->setTime(_logBuffer1DateTime.time());
}

void MainWindow::on_graphFromBuffer2Button_clicked()
{
	graphEndDate->setDate(_logBuffer2DateTime.date());
	graphEndTime->setTime(_logBuffer2DateTime.time());
}
void MainWindow::on_logFromBuffer1Button_clicked()
{
	logBeginDate->setDate(_logBuffer1DateTime.date());
	logBeginTime->setTime(_logBuffer1DateTime.time());
}

void MainWindow::on_logFromBuffer2Button_clicked()
{
	logEndDate->setDate(_logBuffer2DateTime.date());
	logEndTime->setTime(_logBuffer2DateTime.time());
}

void MainWindow::on_logToBuffer1Button_clicked()
{
	QTableWidgetItem * item = logListTable->item(logListTable->currentRow(), 0);
	
	if (item)
		_logBuffer1DateTime = QDateTime::fromString(item->text(), QString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::on_logToBuffer2Button_clicked()
{
	QTableWidgetItem * item = logListTable->item(logListTable->currentRow(), 0);
	
	if (item)
		_logBuffer2DateTime = QDateTime::fromString(item->text(), QString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::checkMnemoE2pLed(int state)
{
	bool ok = true;
	
	ok &= pexline.e2pZone1.isConnected();
	ok &= pexline.e2pZone2.isConnected();
	ok &= pexline.e2pZone3.isConnected();
	ok &= pexline.e2pZone4.isConnected();
	ok &= pexline.e2pTrace.isConnected();
	ok &= pexline.e2pInvertor.isConnected();
	ok &= pexline.e2pPressure.isConnected();
	ok &= pexline.cpu.isConnected();
	
	ok &= pexline.e2pZone1.temperature().ok();
	ok &= pexline.e2pZone2.temperature().ok();
	ok &= pexline.e2pZone3.temperature().ok();
	ok &= pexline.e2pZone4.temperature().ok();
	ok &= pexline.e2pTrace.temperature().ok();
	ok &= pexline.e2pInvertor.current().ok();
	ok &= pexline.e2pPressure.pressure().ok();
	ok &= pexline.cpu.e2WaterOn();
	
	mnemoE2pLed->set(!ok);
}

void MainWindow::checkMnemoE2kLed(int state)
{
	bool ok = true;
	
	ok &= pexline.e2kZone1.isConnected();
	ok &= pexline.e2kZone2.isConnected();
	ok &= pexline.e2kZone3.isConnected();
	ok &= pexline.e2kTrace.isConnected();
	ok &= pexline.e2kInvertor.isConnected();
	ok &= pexline.e2kPressure.isConnected();
	ok &= pexline.cpu.isConnected();
	
	ok &= pexline.e2kZone1.temperature().ok();
	ok &= pexline.e2kZone2.temperature().ok();
	ok &= pexline.e2kZone3.temperature().ok();
	ok &= pexline.e2kTrace.temperature().ok();
	ok &= pexline.e2kInvertor.current().ok();
	ok &= pexline.e2kPressure.pressure().ok();
	ok &= pexline.cpu.e2WaterOn();
	
	mnemoE2kLed->set(!ok);
}

void MainWindow::checkMnemoE3kLed(int state)
{
	bool ok = true;
	
	ok &= pexline.e3kZone1.isConnected();
	ok &= pexline.e3kZone2.isConnected();
	ok &= pexline.e3kZone3.isConnected();
	ok &= pexline.e3kTrace.isConnected();
	ok &= pexline.e3kInvertor.isConnected();
	ok &= pexline.e3kPressure.isConnected();
	ok &= pexline.cpu.isConnected();
	
	ok &= pexline.e3kZone1.temperature().ok();
	ok &= pexline.e3kZone2.temperature().ok();
	ok &= pexline.e3kZone3.temperature().ok();
	ok &= pexline.e3kTrace.temperature().ok();
	ok &= pexline.e3kInvertor.current().ok();
	ok &= pexline.e3kPressure.pressure().ok();
	ok &= pexline.cpu.e3WaterOn();
	
	mnemoE3kLed->set(!ok);
}

void MainWindow::checkMnemoE3pLed(int state)
{
	bool ok = true;
	
	ok &= pexline.e3pZone1.isConnected();
	ok &= pexline.e3pZone2.isConnected();
	ok &= pexline.e3pZone3.isConnected();
	ok &= pexline.e3pZone4.isConnected();
	ok &= pexline.e3pTrace.isConnected();
	ok &= pexline.e3pInvertor.isConnected();
	ok &= pexline.e3pPressure.isConnected();
	ok &= pexline.cpu.isConnected();
	
	ok &= pexline.e3pZone1.temperature().ok();
	ok &= pexline.e3pZone2.temperature().ok();
	ok &= pexline.e3pZone3.temperature().ok();
	ok &= pexline.e3pZone4.temperature().ok();
	ok &= pexline.e3pTrace.temperature().ok();
	ok &= pexline.e3pInvertor.current().ok();
	ok &= pexline.e3pPressure.pressure().ok();
	ok &= pexline.cpu.e3WaterOn();
	
	mnemoE3pLed->set(!ok);
}

void MainWindow::checkMnemoE3mLed(int state)
{
	bool ok = true;
	
	ok &= pexline.e3mZone1.isConnected();
	ok &= pexline.e3mZone2.isConnected();
	ok &= pexline.e3mZone3.isConnected();
	ok &= pexline.e3mTrace.isConnected();
	ok &= pexline.e3mInvertor.isConnected();
	ok &= pexline.e3mPressure.isConnected();
	ok &= pexline.cpu.isConnected();
	
	ok &= pexline.e3mZone1.temperature().ok();
	ok &= pexline.e3mZone2.temperature().ok();
	ok &= pexline.e3mZone3.temperature().ok();
	ok &= pexline.e3mTrace.temperature().ok();
	ok &= pexline.e3mInvertor.current().ok();
	ok &= pexline.e3mPressure.pressure().ok();
	ok &= pexline.cpu.e3WaterOn();
	
	mnemoE3mLed->set(!ok);
}

void MainWindow::checkMnemoSgLed(int state)
{
	bool ok = true;
	
	ok &= pexline.sgZone1.isConnected();
	ok &= pexline.sgZone2.isConnected();
	ok &= pexline.sgInvertor.isConnected();
	ok &= pexline.cpu.isConnected();
	
	ok &= pexline.sgZone1.temperature().ok();
	ok &= pexline.sgZone2.temperature().ok();
	ok &= pexline.sgInvertor.current().ok();
	ok &= pexline.cpu.headWaterOn();
	
	ok &= pexline.uzg.isConnected();
	ok &= (pexline.uzg.temperature().scaledValue() < pexline.uzg.tempAlarm().scaledValue());
	
	mnemoSgLed->set(!ok);
}

void MainWindow::checkMnemoTuLed(int state)
{
	bool ok = true;
	
	ok &= pexline.tuInvertor.isConnected();
	ok &= pexline.cpu.isConnected();
	
	ok &= pexline.tuInvertor.current().ok();
	
	mnemoTuLed->set(!ok);
}

void MainWindow::checkMnemoBathLed(int state)
{
	bool ok = true;
	
	ok &= pexline.cpu.isConnected();
	
	ok &= pexline.cpu.tankWaterLevel0On();
	ok &= pexline.cpu.tankWaterLevel1On();
	ok &= !pexline.cpu.tankWaterLevel2On();

	mnemoBathLed->set(!ok);
}
void MainWindow::showSvarkaStartTime()
{
	double time = 0;

	if (svarkaStartSgRotateButton->isChecked())
		if (time < svarkaStartSgRotateStartEntry->value())
			time = svarkaStartSgRotateStartEntry->value();
				
	if (svarkaStartUzgButton->isChecked())
		if (time < svarkaStartUzgStartEntry->value())
			time = svarkaStartUzgStartEntry->value();
		
	if (svarkaStartTractorButton->isChecked())
		if (time < svarkaStartTractorStartEntry->value())
			time = svarkaStartTractorStartEntry->value();

	if (svarkaStartPushdownTractorButton->isChecked())
		if (time < svarkaStartPushdownTractorStartEntry->value())
			time = svarkaStartPushdownTractorStartEntry->value();
			
	if (svarkaStartSgPositionButton->isChecked())
		if (time < svarkaStartSgPositionStartEntry->value())
			time = svarkaStartSgPositionStartEntry->value();
			
	svarkaStartTotalStartEdit->setValue(time);	
}

void MainWindow::showSvarkaStopTime()
{
	double time = 0;

	if (svarkaStartSgRotateButton->isChecked())
		if (time < svarkaStartSgRotateStopEntry->value())
			time = svarkaStartSgRotateStopEntry->value();
				
	if (svarkaStartUzgButton->isChecked())
		if (time < svarkaStartUzgStopEntry->value())
			time = svarkaStartUzgStopEntry->value();
		
	if (svarkaStartTractorButton->isChecked())
		if (time < svarkaStartTractorStopEntry->value())
			time = svarkaStartTractorStopEntry->value();

	if (svarkaStartPushdownTractorButton->isChecked())
		if (time < svarkaStartPushdownTractorStopEntry->value())
			time = svarkaStartPushdownTractorStopEntry->value();
			
	if (svarkaStartSgPositionButton->isChecked())
		if (time < svarkaStartSgPositionStopEntry->value())
			time = svarkaStartSgPositionStopEntry->value();
			
	svarkaStartTotalStopEdit->setValue(time);	
}

void MainWindow::showLineStartTime()
{
	double time = 0;

	if (lineStartSgRotateButton->isChecked())
		if (time < lineStartSgRotateStartEntry->value())
			time = lineStartSgRotateStartEntry->value();
				
	if (lineStartUzgButton->isChecked())
		if (time < lineStartUzgStartEntry->value())
			time = lineStartUzgStartEntry->value();
		
	if (lineStartTractorButton->isChecked())
		if (time < lineStartTractorStartEntry->value())
			time = lineStartTractorStartEntry->value();

	if (lineStartPushdownTractorButton->isChecked())
		if (time < lineStartPushdownTractorStartEntry->value())
			time = lineStartPushdownTractorStartEntry->value();
			
	if (lineStartSgPositionButton->isChecked())
		if (time < lineStartSgPositionStartEntry->value())
			time = lineStartSgPositionStartEntry->value();
	
	if (lineStartWaterModeButton->isChecked())
		if (time < lineStartWaterModeStartEntry->value())
			time = lineStartWaterModeStartEntry->value();
		
	if (lineStartPe1RotateButton->isChecked())
		if (time < lineStartPe1RotateStartEntry->value())
			time = lineStartPe1RotateStartEntry->value();
		
	if (lineStartPe2RotateButton->isChecked())
		if (time < lineStartPe2RotateStartEntry->value())
			time = lineStartPe2RotateStartEntry->value();
		
	if (lineStartPemRotateButton->isChecked())
		if (time < lineStartPemRotateStartEntry->value())
			time = lineStartPemRotateStartEntry->value();
		
	if (lineStartKl1RotateButton->isChecked())
		if (time < lineStartKl1RotateStartEntry->value())
			time = lineStartKl1RotateStartEntry->value();
		
	if (lineStartKl2RotateButton->isChecked())
		if (time < lineStartKl2RotateStartEntry->value())
			time = lineStartKl2RotateStartEntry->value();
		
	lineStartTotalStartEdit->setValue(time);	
}

void MainWindow::showLineStopTime()
{
	double time = 0;

	if (lineStartSgRotateButton->isChecked())
		if (time < lineStartSgRotateStopEntry->value())
			time = lineStartSgRotateStopEntry->value();
				
	if (lineStartUzgButton->isChecked())
		if (time < lineStartUzgStopEntry->value())
			time = lineStartUzgStopEntry->value();
		
	if (lineStartTractorButton->isChecked())
		if (time < lineStartTractorStopEntry->value())
			time = lineStartTractorStopEntry->value();

	if (lineStartPushdownTractorButton->isChecked())
		if (time < lineStartPushdownTractorStopEntry->value())
			time = lineStartPushdownTractorStopEntry->value();
			
	if (lineStartSgPositionButton->isChecked())
		if (time < lineStartSgPositionStopEntry->value())
			time = lineStartSgPositionStopEntry->value();
	
	if (lineStartWaterModeButton->isChecked())
		if (time < lineStartWaterModeStopEntry->value())
			time = lineStartWaterModeStopEntry->value();
		
	if (lineStartPe1RotateButton->isChecked())
		if (time < lineStartPe1RotateStopEntry->value())
			time = lineStartPe1RotateStopEntry->value();
		
	if (lineStartPe2RotateButton->isChecked())
		if (time < lineStartPe2RotateStopEntry->value())
			time = lineStartPe2RotateStopEntry->value();
		
	if (lineStartPemRotateButton->isChecked())
		if (time < lineStartPemRotateStopEntry->value())
			time = lineStartPemRotateStopEntry->value();
		
	if (lineStartKl1RotateButton->isChecked())
		if (time < lineStartKl1RotateStopEntry->value())
			time = lineStartKl1RotateStopEntry->value();
		
	if (lineStartKl2RotateButton->isChecked())
		if (time < lineStartKl2RotateStopEntry->value())
			time = lineStartKl2RotateStopEntry->value();
		
	lineStartTotalStopEdit->setValue(time);	
}

void MainWindow::checkAutostartLeds()
{
	bool result;
	
	result = pexline.cpu.pressureOn();
	svarkaStartSystemAirLed->set(!result);
	lineStartSystemAirLed->set(!result);

	result = pexline.cpu.headWaterOn();
	svarkaStartSgWaterLed->set(!result);
	lineStartSgWaterLed->set(!result);

	result = pexline.sgZone1.temperature().ok() && pexline.sgZone2.temperature().ok();
	svarkaStartSgTempLed->set(!result);
	lineStartEgTempLed->set(!result);

	result = pexline.uzg.busy().isOff() && pexline.uzg.runStop().isOff();
	result &= pexline.uzg.temperature().scaledValue() < pexline.uzg.tempAlarm().scaledValue();
	svarkaStartUzgReadyLed->set(!result);
	lineStartUzgReadyLed->set(!result);

	result =  pexline.cpu.e2WaterOn();
	lineStartInnerWaterLed->set(!result);

	result =  pexline.cpu.e3WaterOn();
	lineStartOuterWaterLed->set(!result);

	result = pexline.e2pZone1.temperature().ok() &&
		pexline.e2pZone2.temperature().ok() &&
		pexline.e2pZone3.temperature().ok() &&
		pexline.e2pZone4.temperature().ok() &&
		pexline.e2pTrace.temperature().ok();

	lineStartPe1TempLed->set(!result);

	result = pexline.e2kZone1.temperature().ok() &&
		pexline.e2kZone2.temperature().ok() &&
		pexline.e2kZone3.temperature().ok() &&
		pexline.e2kTrace.temperature().ok();

	lineStartKl1TempLed->set(!result);

	result = pexline.e3pZone1.temperature().ok() &&
		pexline.e3pZone2.temperature().ok() &&
		pexline.e3pZone3.temperature().ok() &&
		pexline.e3pZone4.temperature().ok() &&
		pexline.e3pTrace.temperature().ok();
			
	lineStartPe2TempLed->set(!result);

	result = pexline.e3kZone1.temperature().ok() &&
		pexline.e3kZone2.temperature().ok() &&
		pexline.e3kZone3.temperature().ok() &&
		pexline.e3kTrace.temperature().ok();

	lineStartKl2TempLed->set(!result);

	result = pexline.e3mZone1.temperature().ok() &&
		pexline.e3mZone2.temperature().ok() &&
		pexline.e3mZone3.temperature().ok() &&
		pexline.e3mTrace.temperature().ok();

	lineStartPemTempLed->set(!result);
}


void MainWindow::showBottomEdit()
{
	if (pexline.tuInvertor.isDisconnected())
	{
		tubeSpeedEdit->setRed();
		tubeSpeedEdit->setValue(0);
	}
	else
	{
		tubeSpeedEdit->setGreen();
		if (pexline.tuInvertor.runStop().isOff())
			tubeSpeedEdit->setValue(0);
		else
			tubeSpeedEdit->setValue(pexline.tuInvertor.frequency().scaledValue());
	}
	
	if (pexline.e2pInvertor.isDisconnected())
	{
		bottomE2PFreqEdit->setRed();
		bottomE2PFreqEdit->setValue(0);
	}
	else
	{
		bottomE2PFreqEdit->setGreen();
		if (pexline.e2pInvertor.runStop().isOff())
			bottomE2PFreqEdit->setValue(0);
		else
			bottomE2PFreqEdit->setValue(pexline.e2pInvertor.frequency().scaledValue());
	}
	
	if (pexline.e3pInvertor.isDisconnected())
	{
		bottomE3PFreqEdit->setRed();
		bottomE3PFreqEdit->setValue(0);
	}
	else
	{
		bottomE3PFreqEdit->setGreen();
		if (pexline.e3pInvertor.runStop().isOff())
			bottomE3PFreqEdit->setValue(0);
		else
			bottomE3PFreqEdit->setValue(pexline.e3pInvertor.frequency().scaledValue());
	}
	
	if (pexline.e2kInvertor.isDisconnected())
	{
		bottomE2KFreqEdit->setRed();
		bottomE2KFreqEdit->setValue(0);
	}
	else
	{
		bottomE2KFreqEdit->setGreen();
		if (pexline.e2kInvertor.runStop().isOff())
			bottomE2KFreqEdit->setValue(0);
		else
			bottomE2KFreqEdit->setValue(pexline.e2kInvertor.frequency().scaledValue());
	}
	
	if (pexline.e3kInvertor.isDisconnected())
	{
		bottomE3KFreqEdit->setRed();
		bottomE3KFreqEdit->setValue(0);
	}
	else
	{
		bottomE3KFreqEdit->setGreen();
		if (pexline.e3kInvertor.runStop().isOff())
			bottomE3KFreqEdit->setValue(0);
		else
			bottomE3KFreqEdit->setValue(pexline.e3kInvertor.frequency().scaledValue());
	}
			
	if (pexline.e3mInvertor.isDisconnected())
	{
		bottomE3MFreqEdit->setRed();
		bottomE3MFreqEdit->setValue(0);
	}
	else
	{
		bottomE3MFreqEdit->setGreen();
		if (pexline.e3mInvertor.runStop().isOff())
			bottomE3MFreqEdit->setValue(0);
		else
			bottomE3MFreqEdit->setValue(pexline.e3mInvertor.frequency().scaledValue());
	}

	if (pexline.sgInvertor.isDisconnected())
	{
		bottomSgSpeedEdit->setRed();
		bottomSgSpeedEdit->setValue(0);

		bottomSgFreqEdit->setRed();
		bottomSgFreqEdit->setValue(0);
	}
	else
	{
		bottomSgSpeedEdit->setGreen();
		bottomSgFreqEdit->setGreen();
		
		if (pexline.sgInvertor.runStop().isOff())
		{
			bottomSgFreqEdit->setValue(0);
			bottomSgSpeedEdit->setValue(0);
		}
		else
		{
			bottomSgFreqEdit->setValue(pexline.sgInvertor.frequency().scaledValue());
			bottomSgSpeedEdit->setValue(driveParamSgSpeedEntry->value());
		}
	}

	if (pexline.cpu.headDown())
		bottomSgStateLabel->setText(QString("Раб.пол."));
	else
		bottomSgStateLabel->setText(QString("Исх.пол."));

	if (pexline.e2pPressure.isConnected() && pexline.e2pPressure.pressure().ok())
		bottomE2PPressureEdit->setGreen();
	else
		bottomE2PPressureEdit->setRed();

	if (pexline.e2kPressure.isConnected() && pexline.e2kPressure.pressure().ok())
		bottomE2KPressureEdit->setGreen();
	else
		bottomE2KPressureEdit->setRed();

	if (pexline.e3kPressure.isConnected() && pexline.e3kPressure.pressure().ok())
		bottomE3KPressureEdit->setGreen();
	else
		bottomE3KPressureEdit->setRed();	

	if (pexline.e3pPressure.isConnected() && pexline.e3pPressure.pressure().ok())
		bottomE3PPressureEdit->setGreen();
	else
		bottomE3PPressureEdit->setRed();	

	if (pexline.e3mPressure.isConnected() && pexline.e3mPressure.pressure().ok())
		bottomE3MPressureEdit->setGreen();
	else
		bottomE3MPressureEdit->setRed();	

	bottomE2PZone1Led->set(pexline.e2pZone1.isDisconnected() || !(pexline.e2pZone1.temperature().ok()));
	bottomE2PZone2Led->set(pexline.e2pZone2.isDisconnected() || !(pexline.e2pZone2.temperature().ok()));
	bottomE2PZone3Led->set(pexline.e2pZone3.isDisconnected() || !(pexline.e2pZone3.temperature().ok()));
	bottomE2PZone4Led->set(pexline.e2pZone4.isDisconnected() || !(pexline.e2pZone4.temperature().ok()));
	bottomE2PTraceLed->set(pexline.e2pTrace.isDisconnected() || !(pexline.e2pTrace.temperature().ok()));

	bottomE2KZone1Led->set(pexline.e2kZone1.isDisconnected() || !(pexline.e2kZone1.temperature().ok()));
	bottomE2KZone2Led->set(pexline.e2kZone2.isDisconnected() || !(pexline.e2kZone2.temperature().ok()));
	bottomE2KZone3Led->set(pexline.e2kZone3.isDisconnected() || !(pexline.e2kZone3.temperature().ok()));
	bottomE2KTraceLed->set(pexline.e2kTrace.isDisconnected() || !(pexline.e2kTrace.temperature().ok()));

	bottomE3KZone1Led->set(pexline.e3kZone1.isDisconnected() || !(pexline.e3kZone1.temperature().ok()));
	bottomE3KZone2Led->set(pexline.e3kZone2.isDisconnected() || !(pexline.e3kZone2.temperature().ok()));
	bottomE3KZone3Led->set(pexline.e3kZone3.isDisconnected() || !(pexline.e3kZone3.temperature().ok()));
	bottomE3KTraceLed->set(pexline.e3kTrace.isDisconnected() || !(pexline.e3kTrace.temperature().ok()));

	bottomE3PZone1Led->set(pexline.e3pZone1.isDisconnected() || !(pexline.e3pZone1.temperature().ok()));
	bottomE3PZone2Led->set(pexline.e3pZone2.isDisconnected() || !(pexline.e3pZone2.temperature().ok()));
	bottomE3PZone3Led->set(pexline.e3pZone3.isDisconnected() || !(pexline.e3pZone3.temperature().ok()));
	bottomE3PZone4Led->set(pexline.e3pZone4.isDisconnected() || !(pexline.e3pZone4.temperature().ok()));
	bottomE3PTraceLed->set(pexline.e3pTrace.isDisconnected() || !(pexline.e3pTrace.temperature().ok()));

	bottomE3MZone1Led->set(pexline.e3mZone1.isDisconnected() || !(pexline.e3mZone1.temperature().ok()));
	bottomE3MZone2Led->set(pexline.e3mZone2.isDisconnected() || !(pexline.e3mZone2.temperature().ok()));
	bottomE3MZone3Led->set(pexline.e3mZone3.isDisconnected() || !(pexline.e3mZone3.temperature().ok()));
	bottomE3MTraceLed->set(pexline.e3mTrace.isDisconnected() || !(pexline.e3mTrace.temperature().ok()));

	bottomSgZone1Led->set(pexline.sgZone1.isDisconnected() || !(pexline.sgZone1.temperature().ok()));
	bottomSgZone2Led->set(pexline.sgZone2.isDisconnected() || !(pexline.sgZone2.temperature().ok()));

	if (pexline.uzg.isDisconnected())
	{
		bottomUzgFreqEdit->setValue(0);
		bottomUzgFreqEdit->setRed();
		bottomUzgCurrentEdit->setValue(0);
		bottomUzgCurrentEdit->setRed();
		bottomUzgPowerEdit->setValue(0);
		bottomUzgPowerEdit->setRed();
		bottomUzgLed->on();
	}
	else
	{
		bottomUzgFreqEdit->setGreen();
		bottomUzgCurrentEdit->setGreen();
		bottomUzgPowerEdit->setGreen();

		if (pexline.uzg.runStop().isOff())
		{
			bottomUzgFreqEdit->setValue(0);
			bottomUzgCurrentEdit->setValue(0);
			bottomUzgPowerEdit->setValue(0);
		}
		else
		{
			bottomUzgFreqEdit->setValue(uzgFreqEdit->value());
			bottomUzgCurrentEdit->setValue(uzgPwmEdit->value());
			bottomUzgPowerEdit->setValue(uzgCurrentEdit->value());
		}
		
		bottomUzgLed->set(pexline.uzg.temperature().scaledValue() > pexline.uzg.tempAlarm().scaledValue());
	}
}

void MainWindow::storeSettings()
{
	QFile	file("meto_forms.cfg");
	file.open(QIODevice::WriteOnly);
	
	QDataStream	stream(&file);
	stream.setVersion(QDataStream::Qt_4_0);
	
	stream << pexline.uzg.isConnected();

// Invertor and Pressure connection
{
	stream << pexline.e2pInvertor.isConnected()
		<< pexline.e2kInvertor.isConnected()
		<< pexline.e3kInvertor.isConnected()
		<< pexline.e3pInvertor.isConnected()
		<< pexline.e3mInvertor.isConnected()
		<< pexline.sgInvertor.isConnected()
		<< pexline.tuInvertor.isConnected();
		
	stream << pexline.e2pPressure.isConnected()
		<< pexline.e2kPressure.isConnected()
		<< pexline.e3kPressure.isConnected()
		<< pexline.e3pPressure.isConnected()
		<< pexline.e3mPressure.isConnected();
}
// warnBounds
{
		stream << warnBoundPe1PressureButton->isChecked()
		<< warnBoundPe2PressureButton->isChecked()
		<< warnBoundKl1PressureButton->isChecked()
		<< warnBoundKl2PressureButton->isChecked()
		<< warnBoundPemPressureButton->isChecked()

		<< warnBoundPe1CurrentButton->isChecked()
		<< warnBoundPe2CurrentButton->isChecked()
		<< warnBoundKl1CurrentButton->isChecked()
		<< warnBoundKl2CurrentButton->isChecked()
		<< warnBoundPemCurrentButton->isChecked()
		
		<< warnBoundTuCurrentButton->isChecked()
		<< warnBoundSgCurrentButton->isChecked()
		<< warnBoundDiameterButton->isChecked()

		<< warnBoundPe1PressureMinEntry->value()
		<< warnBoundPe2PressureMinEntry->value()
		<< warnBoundKl1PressureMinEntry->value()
		<< warnBoundKl2PressureMinEntry->value()
		<< warnBoundPemPressureMinEntry->value()

		<< warnBoundPe1CurrentMinEntry->value()
		<< warnBoundPe2CurrentMinEntry->value()
		<< warnBoundKl1CurrentMinEntry->value()
		<< warnBoundKl2CurrentMinEntry->value()
		<< warnBoundPemCurrentMinEntry->value()
		
		<< warnBoundTuCurrentMinEntry->value()
		<< warnBoundSgCurrentMinEntry->value()
		<< warnBoundDiameterMinEntry->value()

		<< warnBoundPe1PressureMaxEntry->value()
		<< warnBoundPe2PressureMaxEntry->value()
		<< warnBoundKl1PressureMaxEntry->value()
		<< warnBoundKl2PressureMaxEntry->value()
		<< warnBoundPemPressureMaxEntry->value()

		<< warnBoundPe1CurrentMaxEntry->value()
		<< warnBoundPe2CurrentMaxEntry->value()
		<< warnBoundKl1CurrentMaxEntry->value()
		<< warnBoundKl2CurrentMaxEntry->value()
		<< warnBoundPemCurrentMaxEntry->value()
		
		<< warnBoundTuCurrentMaxEntry->value()
		<< warnBoundSgCurrentMaxEntry->value()
		<< warnBoundDiameterMaxEntry->value();
}
// termo connection
{ 
	stream << pexline.e2pZone1.isConnected()
		<< pexline.e2pZone2.isConnected()
		<< pexline.e2pZone3.isConnected()
		<< pexline.e2pZone4.isConnected()
		<< pexline.e2pTrace.isConnected()
		
		<< pexline.e3pZone1.isConnected()
		<< pexline.e3pZone2.isConnected()
		<< pexline.e3pZone3.isConnected()
		<< pexline.e3pZone4.isConnected()
		<< pexline.e3pTrace.isConnected()

		<< pexline.e2kZone1.isConnected()
		<< pexline.e2kZone2.isConnected()
		<< pexline.e2kZone3.isConnected()
		<< pexline.e2kTrace.isConnected()
		
		<< pexline.e3mZone1.isConnected()
		<< pexline.e3mZone2.isConnected()
		<< pexline.e3mZone3.isConnected()
		<< pexline.e3mTrace.isConnected()

		<< pexline.sgZone2.isConnected()
		<< pexline.sgZone1.isConnected();
}

// termo deltas
{ 
	stream << pe1zone1TermoFrame->minDelta->value()
		<< pe1zone2TermoFrame->minDelta->value()
		<< pe1zone3TermoFrame->minDelta->value()
		<< pe1zone4TermoFrame->minDelta->value()
		<< pe1traceTermoFrame->minDelta->value()
		
		<< pe2zone1TermoFrame->minDelta->value()
		<< pe2zone2TermoFrame->minDelta->value()
		<< pe2zone3TermoFrame->minDelta->value()
		<< pe2zone4TermoFrame->minDelta->value()
		<< pe2traceTermoFrame->minDelta->value()

		<< kl1zone1TermoFrame->minDelta->value()
		<< kl1zone2TermoFrame->minDelta->value()
		<< kl1zone3TermoFrame->minDelta->value()
		<< kl1traceTermoFrame->minDelta->value()
		
		<< kl2zone1TermoFrame->minDelta->value()
		<< kl2zone2TermoFrame->minDelta->value()
		<< kl2zone3TermoFrame->minDelta->value()
		<< kl2traceTermoFrame->minDelta->value()

		<< pemZone1TermoFrame->minDelta->value()
		<< pemZone2TermoFrame->minDelta->value()
		<< pemZone3TermoFrame->minDelta->value()
		<< pemTraceTermoFrame->minDelta->value()

		<< egZone2TermoFrame->minDelta->value()
		<< egZone1TermoFrame->minDelta->value()

		<< pe1zone1TermoFrame->maxDelta->value()
		<< pe1zone2TermoFrame->maxDelta->value()
		<< pe1zone3TermoFrame->maxDelta->value()
		<< pe1zone4TermoFrame->maxDelta->value()
		<< pe1traceTermoFrame->maxDelta->value()
		
		<< pe2zone1TermoFrame->maxDelta->value()
		<< pe2zone2TermoFrame->maxDelta->value()
		<< pe2zone3TermoFrame->maxDelta->value()
		<< pe2zone4TermoFrame->maxDelta->value()
		<< pe2traceTermoFrame->maxDelta->value()

		<< kl1zone1TermoFrame->maxDelta->value()
		<< kl1zone2TermoFrame->maxDelta->value()
		<< kl1zone3TermoFrame->maxDelta->value()
		<< kl1traceTermoFrame->maxDelta->value()
		
		<< kl2zone1TermoFrame->maxDelta->value()
		<< kl2zone2TermoFrame->maxDelta->value()
		<< kl2zone3TermoFrame->maxDelta->value()
		<< kl2traceTermoFrame->maxDelta->value()

		<< pemZone1TermoFrame->maxDelta->value()
		<< pemZone2TermoFrame->maxDelta->value()
		<< pemZone3TermoFrame->maxDelta->value()
		<< pemTraceTermoFrame->maxDelta->value()

		<< egZone2TermoFrame->maxDelta->value()
		<< egZone1TermoFrame->maxDelta->value();
}
// svarka start
{		
	stream << svarkaStartMoveDetectButton->isChecked()
		<< svarkaStartSgRotateButton->isChecked()
		<< svarkaStartUzgButton->isChecked()
		<< svarkaStartTractorButton->isChecked()
		<< svarkaStartPushdownTractorButton->isChecked()
		<< svarkaStartSgPositionButton->isChecked()

		<< svarkaStartSgRotateStartEntry->value()
		<< svarkaStartUzgStartEntry->value()
		<< svarkaStartTractorStartEntry->value()
		<< svarkaStartPushdownTractorStartEntry->value()
		<< svarkaStartSgPositionStartEntry->value()

		<< svarkaStartSgRotateStopEntry->value()
		<< svarkaStartUzgStopEntry->value()
		<< svarkaStartTractorStopEntry->value()
		<< svarkaStartPushdownTractorStopEntry->value()
		<< svarkaStartSgPositionStopEntry->value()
		
		<< svarkaStartSgWaterButton->isChecked()
		<< svarkaStartSgTempButton->isChecked()
		<< svarkaStartSystemAirButton->isChecked()
		<< svarkaStartUzgReadyButton->isChecked();
}
// line start
{		
	stream << lineStartMoveDetectButton->isChecked()
		<< lineStartSgRotateButton->isChecked()
		<< lineStartUzgButton->isChecked()
		<< lineStartTractorButton->isChecked()
		<< lineStartPushdownTractorButton->isChecked()
		<< lineStartSgPositionButton->isChecked()
		<< lineStartPe1RotateButton->isChecked()
		<< lineStartPe2RotateButton->isChecked()
		<< lineStartKl1RotateButton->isChecked()
		<< lineStartKl2RotateButton->isChecked()
		<< lineStartPemRotateButton->isChecked()
		<< lineStartWaterModeButton->isChecked()

		<< lineStartSgRotateStartEntry->value()
		<< lineStartUzgStartEntry->value()
		<< lineStartTractorStartEntry->value()
		<< lineStartPushdownTractorStartEntry->value()
		<< lineStartSgPositionStartEntry->value()
		<< lineStartPe1RotateStartEntry->value()
		<< lineStartPe2RotateStartEntry->value()
		<< lineStartKl1RotateStartEntry->value()
		<< lineStartKl2RotateStartEntry->value()
		<< lineStartPemRotateStartEntry->value()
		<< lineStartWaterModeStartEntry->value()
		
		<< lineStartSgRotateStopEntry->value()
		<< lineStartUzgStopEntry->value()
		<< lineStartTractorStopEntry->value()
		<< lineStartPushdownTractorStopEntry->value()
		<< lineStartSgPositionStopEntry->value()
		<< lineStartPe1RotateStopEntry->value()
		<< lineStartPe2RotateStopEntry->value()
		<< lineStartKl1RotateStopEntry->value()
		<< lineStartKl2RotateStopEntry->value()
		<< lineStartPemRotateStopEntry->value()
		<< lineStartWaterModeStopEntry->value()
		
		<< lineStartSgWaterButton->isChecked()
		<< lineStartOuterWaterButton->isChecked()
		<< lineStartInnerWaterButton->isChecked()
		<< lineStartPe1TempButton->isChecked()
		<< lineStartPe2TempButton->isChecked()
		<< lineStartKl1TempButton->isChecked()
		<< lineStartKl2TempButton->isChecked()
		<< lineStartPemTempButton->isChecked()
		<< lineStartEgTempButton->isChecked()
		<< lineStartSystemAirButton->isChecked()
		<< lineStartUzgReadyButton->isChecked();
}

	
	stream << driveParamSgDiameterEntry->value();
	stream << driveParamProportionallyButton->isChecked();
	
	file.close();
}

void MainWindow::loadSettings()
{
	QFile	file("meto_forms.cfg");
	file.open(QIODevice::ReadOnly);
	
	QDataStream	stream(&file);
	
	bool	bdata;
	double	ddata;

	stream >> bdata;
	pexline.uzg.setConnected(bdata);

// Invertor and Pressure connection
{
	
	stream >> bdata;  pexline.e2pInvertor.setConnected(bdata);
	stream >> bdata;  pexline.e2kInvertor.setConnected(bdata);
	stream >> bdata;  pexline.e3kInvertor.setConnected(bdata);
	stream >> bdata;  pexline.e3pInvertor.setConnected(bdata);
	stream >> bdata;  pexline.e3mInvertor.setConnected(bdata);
	stream >> bdata;  pexline.sgInvertor.setConnected(bdata);
	stream >> bdata;  pexline.tuInvertor.setConnected(bdata);
		
	stream >> bdata;  pexline.e2pPressure.setConnected(bdata);
	stream >> bdata;  pexline.e2kPressure.setConnected(bdata);
	stream >> bdata;  pexline.e3kPressure.setConnected(bdata);
	stream >> bdata;  pexline.e3pPressure.setConnected(bdata);
	stream >> bdata;  pexline.e3mPressure.setConnected(bdata);
}
// warnBounds
{
	stream >> bdata;  warnBoundPe1PressureButton->setChecked(bdata);
	stream >> bdata;  warnBoundPe2PressureButton->setChecked(bdata);
	stream >> bdata;  warnBoundKl1PressureButton->setChecked(bdata);
	stream >> bdata;  warnBoundKl2PressureButton->setChecked(bdata);
	stream >> bdata;  warnBoundPemPressureButton->setChecked(bdata);

	stream >> bdata;  warnBoundPe1CurrentButton->setChecked(bdata);
	stream >> bdata;  warnBoundPe2CurrentButton->setChecked(bdata);
	stream >> bdata;  warnBoundKl1CurrentButton->setChecked(bdata);
	stream >> bdata;  warnBoundKl2CurrentButton->setChecked(bdata);
	stream >> bdata;  warnBoundPemCurrentButton->setChecked(bdata);
		
	stream >> bdata;  warnBoundTuCurrentButton->setChecked(bdata);
	stream >> bdata;  warnBoundSgCurrentButton->setChecked(bdata);
	stream >> bdata;  warnBoundDiameterButton->setChecked(bdata);

	stream >> ddata; warnBoundPe1PressureMinEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundPe2PressureMinEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundKl1PressureMinEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundKl2PressureMinEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundPemPressureMinEntry->setValue(ddata, true, true);

	stream >> ddata; warnBoundPe1CurrentMinEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundPe2CurrentMinEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundKl1CurrentMinEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundKl2CurrentMinEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundPemCurrentMinEntry->setValue(ddata, true, true);
		
	stream >> ddata; warnBoundTuCurrentMinEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundSgCurrentMinEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundDiameterMinEntry->setValue(ddata, true, true);

	stream >> ddata; warnBoundPe1PressureMaxEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundPe2PressureMaxEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundKl1PressureMaxEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundKl2PressureMaxEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundPemPressureMaxEntry->setValue(ddata, true, true);

	stream >> ddata; warnBoundPe1CurrentMaxEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundPe2CurrentMaxEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundKl1CurrentMaxEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundKl2CurrentMaxEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundPemCurrentMaxEntry->setValue(ddata, true, true);
		
	stream >> ddata; warnBoundTuCurrentMaxEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundSgCurrentMaxEntry->setValue(ddata, true, true);
	stream >> ddata; warnBoundDiameterMaxEntry->setValue(ddata, true, true);
}
// termo connection
{ 
	stream >> bdata; pexline.e2pZone1.setConnected(bdata);
	stream >> bdata; pexline.e2pZone2.setConnected(bdata);
	stream >> bdata; pexline.e2pZone3.setConnected(bdata);
	stream >> bdata; pexline.e2pZone4.setConnected(bdata);
	stream >> bdata; pexline.e2pTrace.setConnected(bdata);
		
	stream >> bdata; pexline.e3pZone1.setConnected(bdata);
	stream >> bdata; pexline.e3pZone2.setConnected(bdata);
	stream >> bdata; pexline.e3pZone3.setConnected(bdata);
	stream >> bdata; pexline.e3pZone4.setConnected(bdata);
	stream >> bdata; pexline.e3pTrace.setConnected(bdata);

	stream >> bdata; pexline.e2kZone1.setConnected(bdata);
	stream >> bdata; pexline.e2kZone2.setConnected(bdata);
	stream >> bdata; pexline.e2kZone3.setConnected(bdata);
	stream >> bdata; pexline.e2kTrace.setConnected(bdata);
		
	stream >> bdata; pexline.e3mZone1.setConnected(bdata);
	stream >> bdata; pexline.e3mZone2.setConnected(bdata);
	stream >> bdata; pexline.e3mZone3.setConnected(bdata);
	stream >> bdata; pexline.e3mTrace.setConnected(bdata);

	stream >> bdata; pexline.sgZone2.setConnected(bdata);
	stream >> bdata; pexline.sgZone1.setConnected(bdata);
}

// termo deltas
{ 
	stream >> ddata; pe1zone1TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pe1zone2TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pe1zone3TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pe1zone4TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pe1traceTermoFrame->minDelta->setValue(ddata, true, true);
		
	stream >> ddata; pe2zone1TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pe2zone2TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pe2zone3TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pe2zone4TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pe2traceTermoFrame->minDelta->setValue(ddata, true, true);

	stream >> ddata; kl1zone1TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; kl1zone2TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; kl1zone3TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; kl1traceTermoFrame->minDelta->setValue(ddata, true, true);

	stream >> ddata; kl2zone1TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; kl2zone2TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; kl2zone3TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; kl2traceTermoFrame->minDelta->setValue(ddata, true, true);
		
	stream >> ddata; pemZone1TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pemZone2TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pemZone3TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; pemTraceTermoFrame->minDelta->setValue(ddata, true, true);

	stream >> ddata; egZone2TermoFrame->minDelta->setValue(ddata, true, true);
	stream >> ddata; egZone1TermoFrame->minDelta->setValue(ddata, true, true);

	stream >> ddata; pe1zone1TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pe1zone2TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pe1zone3TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pe1zone4TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pe1traceTermoFrame->maxDelta->setValue(ddata, true, true);
		
	stream >> ddata; pe2zone1TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pe2zone2TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pe2zone3TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pe2zone4TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pe2traceTermoFrame->maxDelta->setValue(ddata, true, true);

	stream >> ddata; kl1zone1TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; kl1zone2TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; kl1zone3TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; kl1traceTermoFrame->maxDelta->setValue(ddata, true, true);
		
	stream >> ddata; kl2zone1TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; kl2zone2TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; kl2zone3TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; kl2traceTermoFrame->maxDelta->setValue(ddata, true, true);

	stream >> ddata; pemZone1TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pemZone2TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pemZone3TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; pemTraceTermoFrame->maxDelta->setValue(ddata, true, true);

	stream >> ddata; egZone2TermoFrame->maxDelta->setValue(ddata, true, true);
	stream >> ddata; egZone1TermoFrame->maxDelta->setValue(ddata, true, true);
}
// svarka start
{		
	stream >> bdata; svarkaStartMoveDetectButton->setChecked(bdata);
	stream >> bdata; svarkaStartSgRotateButton->setChecked(bdata);
	stream >> bdata; svarkaStartUzgButton->setChecked(bdata);
	stream >> bdata; svarkaStartTractorButton->setChecked(bdata);
	stream >> bdata; svarkaStartPushdownTractorButton->setChecked(bdata);
	stream >> bdata; svarkaStartSgPositionButton->setChecked(bdata);

	stream >> ddata; svarkaStartSgRotateStartEntry->setValue(ddata, true, true);
	stream >> ddata; svarkaStartUzgStartEntry->setValue(ddata, true, true);
	stream >> ddata; svarkaStartTractorStartEntry->setValue(ddata, true, true);
	stream >> ddata; svarkaStartPushdownTractorStartEntry->setValue(ddata, true, true);
	stream >> ddata; svarkaStartSgPositionStartEntry->setValue(ddata, true, true);

	stream >> ddata; svarkaStartSgRotateStopEntry->setValue(ddata, true, true);
	stream >> ddata; svarkaStartUzgStopEntry->setValue(ddata, true, true);
	stream >> ddata; svarkaStartTractorStopEntry->setValue(ddata, true, true);
	stream >> ddata; svarkaStartPushdownTractorStopEntry->setValue(ddata, true, true);
	stream >> ddata; svarkaStartSgPositionStopEntry->setValue(ddata, true, true);
		
	stream >> bdata; svarkaStartSgWaterButton->setChecked(bdata);
	stream >> bdata; svarkaStartSgTempButton->setChecked(bdata);
	stream >> bdata; svarkaStartSystemAirButton->setChecked(bdata);
	stream >> bdata; svarkaStartUzgReadyButton->setChecked(bdata);
}
// line start
{		
	stream >> bdata; lineStartMoveDetectButton->setChecked(bdata);
	stream >> bdata; lineStartSgRotateButton->setChecked(bdata);
	stream >> bdata; lineStartUzgButton->setChecked(bdata);
	stream >> bdata; lineStartTractorButton->setChecked(bdata);
	stream >> bdata; lineStartPushdownTractorButton->setChecked(bdata);
	stream >> bdata; lineStartSgPositionButton->setChecked(bdata);
	stream >> bdata; lineStartPe1RotateButton->setChecked(bdata);
	stream >> bdata; lineStartPe2RotateButton->setChecked(bdata);
	stream >> bdata; lineStartKl1RotateButton->setChecked(bdata);
	stream >> bdata; lineStartKl2RotateButton->setChecked(bdata);
	stream >> bdata; lineStartPemRotateButton->setChecked(bdata);
	stream >> bdata; lineStartWaterModeButton->setChecked(bdata);

	stream >> ddata; lineStartSgRotateStartEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartUzgStartEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartTractorStartEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartPushdownTractorStartEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartSgPositionStartEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartPe1RotateStartEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartPe2RotateStartEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartKl1RotateStartEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartKl2RotateStartEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartPemRotateStartEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartWaterModeStartEntry->setValue(ddata, true, true);
		
	stream >> ddata; lineStartSgRotateStopEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartUzgStopEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartTractorStopEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartPushdownTractorStopEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartSgPositionStopEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartPe1RotateStopEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartPe2RotateStopEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartKl1RotateStopEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartKl2RotateStopEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartPemRotateStopEntry->setValue(ddata, true, true);
	stream >> ddata; lineStartWaterModeStopEntry->setValue(ddata, true, true);
		
	stream >> bdata; lineStartSgWaterButton->setChecked(bdata);
	stream >> bdata; lineStartOuterWaterButton->setChecked(bdata);
	stream >> bdata; lineStartInnerWaterButton->setChecked(bdata);
	stream >> bdata; lineStartPe1TempButton->setChecked(bdata);
	stream >> bdata; lineStartPe2TempButton->setChecked(bdata);
	stream >> bdata; lineStartKl1TempButton->setChecked(bdata);
	stream >> bdata; lineStartKl2TempButton->setChecked(bdata);
	stream >> bdata; lineStartPemTempButton->setChecked(bdata);
	stream >> bdata; lineStartEgTempButton->setChecked(bdata);
	stream >> bdata; lineStartSystemAirButton->setChecked(bdata);
	stream >> bdata; lineStartUzgReadyButton->setChecked(bdata);
}


	
	stream >> ddata; driveParamSgDiameterEntry->setValue(ddata, true, true);
	stream >> bdata; driveParamProportionallyButton->setChecked(bdata);

	file.close();
}
void MainWindow::tryQuit()
{
	storeSettings();
	
	if (!_lineStarted)
	{
		postWarning(Quit, -1);
		qApp->quit();
	}
	else
		MyMessageBox(QString("ВЫХОД"), QString("НАЖМИТЕ КНОПКУ 'СТОП'"));
		
}

void MainWindow::storeProcess()
{
	QSqlQuery	query;

  double sgZone1Sv = (pexline.sgZone1.runStop().isOn())?(pexline.sgZone1.temperature().presetValue().scaledValue()):0;
  double sgZone1Pv = pexline.sgZone1.temperature().currentValue().scaledValue();

  double sgZone2Sv = (pexline.sgZone2.runStop().isOn())?(pexline.sgZone2.temperature().presetValue().scaledValue()):0;
  double sgZone2Pv = pexline.sgZone2.temperature().currentValue().scaledValue();

  double e2pZone1Sv = (pexline.e2pZone1.runStop().isOn())?(pexline.e2pZone1.temperature().presetValue().scaledValue()):0;
  double e2pZone1Pv = pexline.e2pZone1.temperature().currentValue().scaledValue();
  double e2pZone2Sv = (pexline.e2pZone2.runStop().isOn())?(pexline.e2pZone2.temperature().presetValue().scaledValue()):0;
  double e2pZone2Pv = pexline.e2pZone2.temperature().currentValue().scaledValue();
  double e2pZone3Sv = (pexline.e2pZone3.runStop().isOn())?(pexline.e2pZone3.temperature().presetValue().scaledValue()):0;
  double e2pZone3Pv = pexline.e2pZone3.temperature().currentValue().scaledValue();
  double e2pZone4Sv = (pexline.e2pZone4.runStop().isOn())?(pexline.e2pZone4.temperature().presetValue().scaledValue()):0;
  double e2pZone4Pv = pexline.e2pZone4.temperature().currentValue().scaledValue();
  double e2pTraceSv = (pexline.e2pTrace.runStop().isOn())?(pexline.e2pTrace.temperature().presetValue().scaledValue()):0;
  double e2pTracePv = pexline.e2pTrace.temperature().currentValue().scaledValue();

  double e2kZone1Sv = (pexline.e2kZone1.runStop().isOn())?(pexline.e2kZone1.temperature().presetValue().scaledValue()):0;
  double e2kZone1Pv = pexline.e2kZone1.temperature().currentValue().scaledValue();
  double e2kZone2Sv = (pexline.e2kZone2.runStop().isOn())?(pexline.e2kZone2.temperature().presetValue().scaledValue()):0;
  double e2kZone2Pv = pexline.e2kZone2.temperature().currentValue().scaledValue();
  double e2kZone3Sv = (pexline.e2kZone3.runStop().isOn())?(pexline.e2kZone3.temperature().presetValue().scaledValue()):0;
  double e2kZone3Pv = pexline.e2kZone3.temperature().currentValue().scaledValue();
  double e2kTraceSv = (pexline.e2kTrace.runStop().isOn())?(pexline.e2kTrace.temperature().presetValue().scaledValue()):0;
  double e2kTracePv = pexline.e2kTrace.temperature().currentValue().scaledValue();

  double e3kZone1Sv = (pexline.e3kZone1.runStop().isOn())?(pexline.e3kZone1.temperature().presetValue().scaledValue()):0;
  double e3kZone1Pv = pexline.e3kZone1.temperature().currentValue().scaledValue();
  double e3kZone2Sv = (pexline.e3kZone2.runStop().isOn())?(pexline.e3kZone2.temperature().presetValue().scaledValue()):0;
  double e3kZone2Pv = pexline.e3kZone2.temperature().currentValue().scaledValue();
  double e3kZone3Sv = (pexline.e3kZone3.runStop().isOn())?(pexline.e3kZone3.temperature().presetValue().scaledValue()):0;
  double e3kZone3Pv = pexline.e3kZone3.temperature().currentValue().scaledValue();
  double e3kTraceSv = (pexline.e3kTrace.runStop().isOn())?(pexline.e3kTrace.temperature().presetValue().scaledValue()):0;
  double e3kTracePv = pexline.e3kTrace.temperature().currentValue().scaledValue();

  double e3pZone1Sv = (pexline.e3pZone1.runStop().isOn())?(pexline.e3pZone1.temperature().presetValue().scaledValue()):0;
  double e3pZone1Pv = pexline.e3pZone1.temperature().currentValue().scaledValue();
  double e3pZone2Sv = (pexline.e3pZone2.runStop().isOn())?(pexline.e3pZone2.temperature().presetValue().scaledValue()):0;
  double e3pZone2Pv = pexline.e3pZone2.temperature().currentValue().scaledValue();
  double e3pZone3Sv = (pexline.e3pZone3.runStop().isOn())?(pexline.e3pZone3.temperature().presetValue().scaledValue()):0;
  double e3pZone3Pv = pexline.e3pZone3.temperature().currentValue().scaledValue();
  double e3pZone4Sv = (pexline.e3pZone4.runStop().isOn())?(pexline.e3pZone4.temperature().presetValue().scaledValue()):0;
  double e3pZone4Pv = pexline.e3pZone4.temperature().currentValue().scaledValue();
  double e3pTraceSv = (pexline.e3pTrace.runStop().isOn())?(pexline.e3pTrace.temperature().presetValue().scaledValue()):0;
  double e3pTracePv = pexline.e3pTrace.temperature().currentValue().scaledValue();

  double e3mZone1Sv = (pexline.e3mZone1.runStop().isOn())?(pexline.e3mZone1.temperature().presetValue().scaledValue()):0;
  double e3mZone1Pv = pexline.e3mZone1.temperature().currentValue().scaledValue();
  double e3mZone2Sv = (pexline.e3mZone2.runStop().isOn())?(pexline.e3mZone2.temperature().presetValue().scaledValue()):0;
  double e3mZone2Pv = pexline.e3mZone2.temperature().currentValue().scaledValue();
  double e3mZone3Sv = (pexline.e3mZone3.runStop().isOn())?(pexline.e3mZone3.temperature().presetValue().scaledValue()):0;
  double e3mZone3Pv = pexline.e3mZone3.temperature().currentValue().scaledValue();
  double e3mTraceSv = (pexline.e3mTrace.runStop().isOn())?(pexline.e3mTrace.temperature().presetValue().scaledValue()):0;
  double e3mTracePv = pexline.e3mTrace.temperature().currentValue().scaledValue();

  double e2pRotate  = (pexline.e2pInvertor.runStop().isOn())?(pexline.e2pInvertor.frequency().scaledValue()):0;
  double e2pCurrent = pexline.e2pInvertor.current().currentValue().scaledValue();
  double e2kRotate  = (pexline.e2kInvertor.runStop().isOn())?(pexline.e2kInvertor.frequency().scaledValue()):0;
  double e2kCurrent = pexline.e2kInvertor.current().currentValue().scaledValue();
  double e3kRotate  = (pexline.e3kInvertor.runStop().isOn())?(pexline.e3kInvertor.frequency().scaledValue()):0;
  double e3kCurrent = pexline.e3kInvertor.current().currentValue().scaledValue();
  double e3pRotate  = (pexline.e3pInvertor.runStop().isOn())?(pexline.e3pInvertor.frequency().scaledValue()):0;
  double e3pCurrent = pexline.e3pInvertor.current().currentValue().scaledValue();
  double e3mRotate  = (pexline.e3mInvertor.runStop().isOn())?(pexline.e3mInvertor.frequency().scaledValue()):0;
  double e3mCurrent = pexline.e3mInvertor.current().currentValue().scaledValue();

  double sgRotate   = (pexline.sgInvertor.runStop().isOn())?(pexline.sgInvertor.frequency().scaledValue()):0;
  double sgCurrent  = pexline.sgInvertor.current().currentValue().scaledValue();
  double sgSpeed    = (pexline.sgInvertor.runStop().isOn())?(driveParamSgSpeedEntry->value()):0;

  double tuRotate   = 0;
  double tuCurrent  = pexline.tuInvertor.current().currentValue().scaledValue();
  double tuSpeed    = (pexline.tuInvertor.runStop().isOn())?(pexline.tuInvertor.frequency().scaledValue()):0;
  double e2pPressure = pexline.e2pPressure.pressure().currentValue().scaledValue();
  double e2kPressure = pexline.e2kPressure.pressure().currentValue().scaledValue();
  double e3kPressure = pexline.e3kPressure.pressure().currentValue().scaledValue();
  double e3pPressure = pexline.e3pPressure.pressure().currentValue().scaledValue();
  double e3mPressure = pexline.e3mPressure.pressure().currentValue().scaledValue();
  double uzgFreq    = (pexline.uzg.runStop().isOn())?(pexline.uzg.freq().scaledValue()):0;
  double uzgCurrent = (pexline.uzg.runStop().isOn())?(pexline.uzg.pwm().scaledValue()):0;
  double uzgPower   = (pexline.uzg.runStop().isOn())?(pexline.uzg.current().scaledValue()):0;
  double uzgTemp = pexline.uzg.temperature().scaledValue();
	
	query.prepare("insert into process_log( \
  dateTime, \
  e2pZone1Sv ,\
  e2pZone1Pv ,\
  e2pZone2Sv ,\
  e2pZone2Pv ,\
  e2pZone3Sv ,\
  e2pZone3Pv ,\
  e2pZone4Sv ,\
  e2pZone4Pv ,\
  e2pTraceSv ,\
  e2pTracePv ,\
  e2kZone1Sv ,\
  e2kZone1Pv ,\
  e2kZone2Sv ,\
  e2kZone2Pv ,\
  e2kZone3Sv ,\
  e2kZone3Pv ,\
  e2kTraceSv ,\
  e2kTracePv ,\
  e3kZone1Sv ,\
  e3kZone1Pv ,\
  e3kZone2Sv ,\
  e3kZone2Pv ,\
  e3kZone3Sv ,\
  e3kZone3Pv ,\
  e3kTraceSv ,\
  e3kTracePv ,\
  e3pZone1Sv ,\
  e3pZone1Pv ,\
  e3pZone2Sv ,\
  e3pZone2Pv ,\
  e3pZone3Sv ,\
  e3pZone3Pv ,\
  e3pZone4Sv ,\
  e3pZone4Pv ,\
  e3pTraceSv ,\
  e3pTracePv ,\
  e3mZone1Sv ,\
  e3mZone1Pv ,\
  e3mZone2Sv ,\
  e3mZone2Pv ,\
  e3mZone3Sv ,\
  e3mZone3Pv ,\
  e3mTraceSv ,\
  e3mTracePv ,\
  sgZone1Sv  ,\
  sgZone1Pv  ,\
  sgZone2Sv  ,\
  sgZone2Pv  ,\
  e2pRotate  ,\
  e2pCurrent ,\
  e2kRotate  ,\
  e2kCurrent ,\
  e3kRotate  ,\
  e3kCurrent ,\
  e3pRotate  ,\
  e3pCurrent ,\
  e3mRotate  ,\
  e3mCurrent ,\
  sgRotate   ,\
  sgCurrent  ,\
  sgSpeed    ,\
  tuRotate   ,\
  tuCurrent  ,\
  tuSpeed    ,\
  e2pPressure ,\
  e2kPressure ,\
  e3kPressure ,\
  e3pPressure ,\
  e3mPressure ,\
  uzgFreq    ,\
  uzgCurrent ,\
  uzgPower   ,\
  uzgTemp)\
	values(\
  :dateTime, \
  :e2pZone1Sv ,\
  :e2pZone1Pv ,\
  :e2pZone2Sv ,\
  :e2pZone2Pv ,\
  :e2pZone3Sv ,\
  :e2pZone3Pv ,\
  :e2pZone4Sv ,\
  :e2pZone4Pv ,\
  :e2pTraceSv ,\
  :e2pTracePv ,\
  :e2kZone1Sv ,\
  :e2kZone1Pv ,\
  :e2kZone2Sv ,\
  :e2kZone2Pv ,\
  :e2kZone3Sv ,\
  :e2kZone3Pv ,\
  :e2kTraceSv ,\
  :e2kTracePv ,\
  :e3kZone1Sv ,\
  :e3kZone1Pv ,\
  :e3kZone2Sv ,\
  :e3kZone2Pv ,\
  :e3kZone3Sv ,\
  :e3kZone3Pv ,\
  :e3kTraceSv ,\
  :e3kTracePv ,\
  :e3pZone1Sv ,\
  :e3pZone1Pv ,\
  :e3pZone2Sv ,\
  :e3pZone2Pv ,\
  :e3pZone3Sv ,\
  :e3pZone3Pv ,\
  :e3pZone4Sv ,\
  :e3pZone4Pv ,\
  :e3pTraceSv ,\
  :e3pTracePv ,\
  :e3mZone1Sv ,\
  :e3mZone1Pv ,\
  :e3mZone2Sv ,\
  :e3mZone2Pv ,\
  :e3mZone3Sv ,\
  :e3mZone3Pv ,\
  :e3mTraceSv ,\
  :e3mTracePv ,\
  :sgZone1Sv  ,\
  :sgZone1Pv  ,\
  :sgZone2Sv  ,\
  :sgZone2Pv  ,\
  :e2pRotate  ,\
  :e2pCurrent ,\
  :e2kRotate  ,\
  :e2kCurrent ,\
  :e3kRotate  ,\
  :e3kCurrent ,\
  :e3pRotate  ,\
  :e3pCurrent ,\
  :e3mRotate  ,\
  :e3mCurrent ,\
  :sgRotate   ,\
  :sgCurrent  ,\
  :sgSpeed    ,\
  :tuRotate   ,\
  :tuCurrent  ,\
  :tuSpeed    ,\
  :e2pPressure ,\
  :e2kPressure ,\
  :e3kPressure ,\
  :e3pPressure ,\
  :e3mPressure ,\
  :uzgFreq    ,\
  :uzgCurrent ,\
  :uzgPower   ,\
  :uzgTemp)");

	query.bindValue(":dateTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	query.bindValue("e2pZone1Sv", e2pZone1Sv);
	query.bindValue("e2pZone1Pv", e2pZone1Pv);
	query.bindValue("e2pZone2Sv", e2pZone2Sv);
	query.bindValue("e2pZone2Pv", e2pZone2Pv);
	query.bindValue("e2pZone3Sv", e2pZone3Sv);
	query.bindValue("e2pZone3Pv", e2pZone3Pv);
	query.bindValue("e2pZone4Sv", e2pZone4Sv);
	query.bindValue("e2pZone4Pv", e2pZone4Pv);
	query.bindValue("e2pTraceSv", e2pTraceSv);
	query.bindValue("e2pTracePv", e2pTracePv);
	query.bindValue("e2kZone1Sv", e2kZone1Sv);
	query.bindValue("e2kZone1Pv", e2kZone1Pv);
	query.bindValue("e2kZone2Sv", e2kZone2Sv);
	query.bindValue("e2kZone2Pv", e2kZone2Pv);
	query.bindValue("e2kZone3Sv", e2kZone3Sv);
	query.bindValue("e2kZone3Pv", e2kZone3Pv);
	query.bindValue("e2kTraceSv", e2kTraceSv);
	query.bindValue("e2kTracePv", e2kTracePv);
	query.bindValue("e3kZone1Sv", e3kZone1Sv);
	query.bindValue("e3kZone1Pv", e3kZone1Pv);
	query.bindValue("e3kZone2Sv", e3kZone2Sv);
	query.bindValue("e3kZone2Pv", e3kZone2Pv);
	query.bindValue("e3kZone3Sv", e3kZone3Sv);
	query.bindValue("e3kZone3Pv", e3kZone3Pv);
	query.bindValue("e3kTraceSv", e3kTraceSv);
	query.bindValue("e3kTracePv", e3kTracePv);
	query.bindValue("e3pZone1Sv", e3pZone1Sv);
	query.bindValue("e3pZone1Pv", e3pZone1Pv);
	query.bindValue("e3pZone2Sv", e3pZone2Sv);
	query.bindValue("e3pZone2Pv", e3pZone2Pv);
	query.bindValue("e3pZone3Sv", e3pZone3Sv);
	query.bindValue("e3pZone3Pv", e3pZone3Pv);
	query.bindValue("e3pZone4Sv", e3pZone4Sv);
	query.bindValue("e3pZone4Pv", e3pZone4Pv);
	query.bindValue("e3pTraceSv", e3pTraceSv);
	query.bindValue("e3pTracePv", e3pTracePv);
	query.bindValue("e3mZone1Sv", e3mZone1Sv);
	query.bindValue("e3mZone1Pv", e3mZone1Pv);
	query.bindValue("e3mZone2Sv", e3mZone2Sv);
	query.bindValue("e3mZone2Pv", e3mZone2Pv);
	query.bindValue("e3mZone3Sv", e3mZone3Sv);
	query.bindValue("e3mZone3Pv", e3mZone3Pv);
	query.bindValue("e3mTraceSv", e3mTraceSv);
	query.bindValue("e3mTracePv", e3mTracePv);
	query.bindValue("sgZone1Sv", sgZone1Sv);
	query.bindValue("sgZone1Pv", sgZone1Pv);
	query.bindValue("sgZone2Sv", sgZone2Sv);
	query.bindValue("sgZone2Pv", sgZone2Pv);
	query.bindValue("e2pRotate", e2pRotate);
	query.bindValue("e2pCurrent", e2pCurrent);
	query.bindValue("e2kRotate", e2kRotate);
	query.bindValue("e2kCurrent", e2kCurrent);
	query.bindValue("e3kRotate", e3kRotate);
	query.bindValue("e3kCurrent", e3kCurrent);
	query.bindValue("e3pRotate", e3pRotate);
	query.bindValue("e3pCurrent", e3pCurrent);
	query.bindValue("e3mRotate", e3mRotate);
	query.bindValue("e3mCurrent", e3mCurrent);
	query.bindValue("sgRotate", sgRotate);
	query.bindValue("sgCurrent", sgCurrent);
	query.bindValue("sgSpeed", sgSpeed);
	query.bindValue("tuRotate", tuRotate);
	query.bindValue("tuCurrent", tuCurrent);
	query.bindValue("tuSpeed", tuSpeed);
	query.bindValue("e2pPressure", e2pPressure);
	query.bindValue("e2kPressure", e2kPressure);
	query.bindValue("e3kPressure", e3kPressure);
	query.bindValue("e3pPressure", e3pPressure);
	query.bindValue("e3mPressure", e3mPressure);
	query.bindValue("uzgFreq", uzgFreq);
	query.bindValue("uzgCurrent", uzgCurrent);
	query.bindValue("uzgPower", uzgPower);
	query.bindValue("uzgTemp", uzgTemp);	

	query.exec();
}

void MainWindow::on_graphSelect1Button_clicked()
{
	_graph1Type = _graphSelectDialog->exec();
	graphSelect1Button->setText(_graphSelectDialog->graphName());
}

void MainWindow::on_graphSelect2Button_clicked()
{
	_graph2Type = _graphSelectDialog->exec();
	graphSelect2Button->setText(_graphSelectDialog->graphName());
}

void MainWindow::on_graphSelect3Button_clicked()
{
	_graph3Type = _graphSelectDialog->exec();
	graphSelect3Button->setText(_graphSelectDialog->graphName());
}


void MainWindow::on_graphUpdateButton_clicked()
{
	graphFrame->clearGraphs();
	
	QDateTime	begin(graphBeginDate->date(), graphBeginTime->time());
	QDateTime	end(graphEndDate->date(), graphEndTime->time());
	
	if (begin.secsTo(end) > 24 * 60 * 60)
	{
		MyMessageBox(QString("Графики"), QString("Допускается вывод графиков\nне более чем за 24 часа"));
		begin = end.addSecs(-24 * 60 * 60);
		graphBeginDate->setDate(begin.date());
		graphBeginTime->setTime(begin.time());
	}
	
	graphUpdate();

	QList< QPair<QDateTime, double> >	data[GraphFrame::GraphCount];
	data[0].clear();
	data[1].clear();
	data[2].clear();

	QSqlQuery	query;
	query.prepare("select * from process_log where dateTime between :begin and :end order by dateTime asc");
	query.bindValue(":begin", begin.toString("yyyy-MM-dd hh:mm:ss"));
	query.bindValue(":end", end.toString("yyyy-MM-dd hh:mm:ss"));
	query.exec();

	QDateTime	dt;
	double		value;
	
	while (query.next())
	{
		dt = query.value(0).toDateTime();
		
		if (_graph1Type)
		{
			value = query.value(_graph1Type).toDouble();
			data[0].push_back(qMakePair(dt, value));
		}
	
		if (_graph2Type)
		{
			value = query.value(_graph2Type).toDouble();
			data[1].push_back(qMakePair(dt, value));
		}
	
		if (_graph3Type)
		{
			value = query.value(_graph3Type).toDouble();
			data[2].push_back(qMakePair(dt, value));
		}
	}
	
	if (_graph1Type)
		graphFrame->setGraph(GraphFrame::RedGraph, data[0], begin, end, 0, graphMaxScale(_graph1Type));
	
	if (_graph2Type)
                graphFrame->setGraph(GraphFrame::GreenGraph, data[1], begin, end, 0, graphMaxScale(_graph2Type));

	if (_graph3Type)
                graphFrame->setGraph(GraphFrame::BlueGraph, data[2], begin, end, 0, graphMaxScale(_graph3Type));
}

void MainWindow::graphUpdate()
{
	checkVerPos();
	checkVerButtons();
	checkHorPos();
	checkHorButtons();
	
	double height1 = graphMaxScale(_graph1Type);
	double height2 = graphMaxScale(_graph2Type);
	double height3 = graphMaxScale(_graph3Type);
	
	double m1 = height1 * _graphVerPos / _graphVerScale / 2;
	double m2 = height2 * _graphVerPos / _graphVerScale / 2;
	double m3 = height3 * _graphVerPos / _graphVerScale / 2;
	
	verAxisFrame->setAxis(m1, m1 + height1 / _graphVerScale,
		m2, m2 + height2 / _graphVerScale,
		m3, m3 + height3 / _graphVerScale
	);
	
	QDateTime	begin(graphBeginDate->date(), graphBeginTime->time());
	QDateTime	end(graphEndDate->date(), graphEndTime->time());
	
	
	int totalSecs = begin.secsTo(end);
	
	QDateTime showBegin = begin.addSecs(totalSecs * _graphHorPos / _graphHorScale / 2);
	QDateTime showEnd = showBegin.addSecs(totalSecs / _graphHorScale);
	
	horAxisFrame->setAxis(showBegin, showEnd);
	
	graphFrame->setScalePos(_graphVerScale, _graphVerPos, _graphHorScale, _graphHorPos);

}

int MainWindow::graphMaxScale(int graphType)
{
	if (Graph::Select == graphType)
		return 0;
		
	if ((Graph::E2pZone1Sv <= graphType) && (graphType <= Graph::SgZone2Pv))
		return 250;
		
	if ((Graph::E2pRotate == graphType) ||
		(Graph::E2kRotate == graphType) ||
		(Graph::E3kRotate == graphType) ||
		(Graph::E3mRotate == graphType) ||
		(Graph::E3pRotate == graphType) ||
		(Graph::SgRotate == graphType) ||
		(Graph::TuRotate == graphType))
		
		return 150;

	if ((Graph::E2pCurrent == graphType) ||
		(Graph::E2kCurrent == graphType) ||
		(Graph::E3kCurrent == graphType) ||
		(Graph::E3mCurrent == graphType) ||
		(Graph::E3pCurrent == graphType) ||
		(Graph::SgCurrent == graphType) ||
		(Graph::TuCurrent == graphType))
		
		return 50;

	if ((Graph::E2pPressure == graphType) ||
		(Graph::E2kPressure == graphType) ||
		(Graph::E3kPressure == graphType) ||
		(Graph::E3mPressure == graphType) ||
		(Graph::E3pPressure == graphType))
		
		return 1000;

	if ((Graph::SgSpeed == graphType) ||
		(Graph::TuSpeed == graphType))
		
		return 25;

	if (Graph::UzgFreq == graphType)
		return 30000;
		
	if (Graph::UzgPower == graphType)
		return 750;
	
	if (Graph::UzgCurrent == graphType)
		return 10;

	if (Graph::UzgTemp == graphType)
		return 100;

	return 0;
}

void MainWindow::on_graphVer1Button_clicked()
{
	graphVer1Button->setChecked(true);
	graphVer5Button->setChecked(false);
	graphVer25Button->setChecked(false);
	
	_graphVerScale = 1;
	
	graphUpdate();
}

void MainWindow::on_graphVer5Button_clicked()
{
	graphVer1Button->setChecked(false);
	graphVer5Button->setChecked(true);
	graphVer25Button->setChecked(false);
	
	_graphVerScale = 5;
	
	graphUpdate();
}

void MainWindow::on_graphVer25Button_clicked()
{
	graphVer1Button->setChecked(false);
	graphVer5Button->setChecked(false);
	graphVer25Button->setChecked(true);
	
	_graphVerScale = 25;
	
	graphUpdate();
}

void MainWindow::on_graphHor1Button_clicked()
{
	graphHor1Button->setChecked(true);
	graphHor5Button->setChecked(false);
	graphHor25Button->setChecked(false);
	
	_graphHorScale = 1;
	
	graphUpdate();
}

void MainWindow::on_graphHor5Button_clicked()
{
	graphHor1Button->setChecked(false);
	graphHor5Button->setChecked(true);
	graphHor25Button->setChecked(false);
	
	_graphHorScale = 5;
	graphUpdate();
}

void MainWindow::on_graphHor25Button_clicked()
{
	graphHor1Button->setChecked(false);
	graphHor5Button->setChecked(false);
	graphHor25Button->setChecked(true);
	
	_graphHorScale = 25;
	
	graphUpdate();
}

void MainWindow::checkVerPos()
{
	int max_pos = _graphVerScale * 2 - 2;
	
	if (_graphVerPos > max_pos)
		_graphVerPos = max_pos;

}

void MainWindow::checkHorPos()
{
	int max_pos = _graphHorScale * 2 - 2;
	
	if (_graphHorPos > max_pos)
		_graphHorPos = max_pos;
}

void MainWindow::checkVerButtons()
{
	int max_pos = _graphVerScale * 2 - 2;

	graphUpButton->setEnabled((_graphVerScale > 1) && (_graphVerPos < max_pos));
	graphDownButton->setEnabled((_graphVerScale > 1) && (_graphVerPos > 0));
}

void MainWindow::checkHorButtons()
{
	int max_pos = _graphHorScale * 2 - 2;

	graphRightButton->setEnabled((_graphHorScale > 1) && (_graphHorPos < max_pos));
	graphLeftButton->setEnabled((_graphHorScale > 1) && (_graphHorPos > 0));
}

void MainWindow::on_graphLeftButton_clicked()
{
	_graphHorPos--;
	graphUpdate();
}

void MainWindow::on_graphRightButton_clicked()
{
	_graphHorPos++;
	graphUpdate();
}

void MainWindow::on_graphDownButton_clicked()
{
	_graphVerPos--;
	graphUpdate();
}

void MainWindow::on_graphUpButton_clicked()
{
	_graphVerPos++;
	graphUpdate();
}

void MainWindow::on_profilesUpButton_clicked()
{
	int row = profilesTable->currentRow();
	
	if (row > 0)
		row--;
		
	profilesTable->setCurrentCell(row, 0);
}

void MainWindow::on_profilesDownButton_clicked()
{
	int row = profilesTable->currentRow();
	
	if (row < profilesTable->rowCount() - 1)
		row++;
		
	profilesTable->setCurrentCell(row, 0);
}

void MainWindow::on_profilesSaveButton_clicked()
{
	QString profileName = profilesNewNameEdit->text();
	
	if (profileName.isEmpty())
	{
		MyMessageBox(QString("Сохранение профиля"), QString("Введите название профиля"));
		return;
	}
	
	QSqlQuery	query;
	
	query.prepare("insert into profiles values(?, ?,?,?,?,?, ?,?,?,?, ?,?,?,?, ?,?,?,?,?, ?,?,?,?, ?,?, ?,?,?,?,?, ?,?,?, ?)");

	query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	
	query.addBindValue(pexline.e2pZone1.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e2pZone2.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e2pZone3.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e2pZone4.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e2pTrace.temperature().presetValue().scaledValue());
	
	query.addBindValue(pexline.e2kZone1.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e2kZone2.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e2kZone3.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e2kTrace.temperature().presetValue().scaledValue());
	
	query.addBindValue(pexline.e3kZone1.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e3kZone2.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e3kZone3.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e3kTrace.temperature().presetValue().scaledValue());

	query.addBindValue(pexline.e3pZone1.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e3pZone2.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e3pZone3.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e3pZone4.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e3pTrace.temperature().presetValue().scaledValue());

	query.addBindValue(pexline.e3mZone1.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e3mZone2.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e3mZone3.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.e3mTrace.temperature().presetValue().scaledValue());

	query.addBindValue(pexline.sgZone1.temperature().presetValue().scaledValue());
	query.addBindValue(pexline.sgZone2.temperature().presetValue().scaledValue());
	
	query.addBindValue(pexline.e2pInvertor.frequency().scaledValue());
	query.addBindValue(pexline.e2kInvertor.frequency().scaledValue());
	query.addBindValue(pexline.e3kInvertor.frequency().scaledValue());
	query.addBindValue(pexline.e3pInvertor.frequency().scaledValue());
	query.addBindValue(pexline.e3mInvertor.frequency().scaledValue());

	query.addBindValue(pexline.sgInvertor.frequency().scaledValue());
	query.addBindValue(pexline.tuInvertor.frequency().scaledValue());
	query.addBindValue(driveParamSgDiameterEntry->value());
	
	query.addBindValue(profileName);

	if (query.exec())
		MyMessageBox(QString("Сохранение профиля"), QString("Профиль сохранен"));
	else
		MyMessageBox(QString("Сохранение профиля"), QString("Ошибка сохранения профиля.\nВозможно, профиль с таким названием\nуже существует"));
	
	profilesUpdateTable();
}

void MainWindow::on_profilesLoadButton_clicked()
{
	QTableWidgetItem * item = profilesTable->item(profilesTable->currentRow(), 1);
	
	if (!item)
		return;
	
	QString	profileName = item->text();

	if (1 == MyMessageBox(QString("Загрузка профиля"),
		QString("Загрузить профиль ") + profileName + QString(" ?"),
		QString("Да"), QString("Нет")))
	{
		QSqlQuery	query;
		query.prepare("select * from profiles where name = :name");
		query.bindValue(":name", profileName);
		if (query.exec())
		{
			MyMessageBox(QString("Загрузка профиля"), QString("Профиль загружен"));
		
			query.next();
			
			int i = 1;
			
			QMessageBox	box(QString("Загрузка профиля"),
				QString("Профиль устанавливается.\nПожалуйста, подождите."),
				QMessageBox::NoIcon, 0, 0, 0);
	
			QFont f = box.font();
			f.setPointSize(12);
			f.setBold(true);
			box.setFont(f);
			box.setWindowModality(Qt::WindowModal);
			box.show();
			
			pexline.e2pZone1.setTemperature(query.value(i++).toDouble());
			pexline.e2pZone2.setTemperature(query.value(i++).toDouble());
			pexline.e2pZone3.setTemperature(query.value(i++).toDouble());
			pexline.e2pZone4.setTemperature(query.value(i++).toDouble());
			pexline.e2pTrace.setTemperature(query.value(i++).toDouble());

			pexline.e2kZone1.setTemperature(query.value(i++).toDouble());
			pexline.e2kZone2.setTemperature(query.value(i++).toDouble());
			pexline.e2kZone3.setTemperature(query.value(i++).toDouble());
			pexline.e2kTrace.setTemperature(query.value(i++).toDouble());

			pexline.e3kZone1.setTemperature(query.value(i++).toDouble());
			pexline.e3kZone2.setTemperature(query.value(i++).toDouble());
			pexline.e3kZone3.setTemperature(query.value(i++).toDouble());
			pexline.e3kTrace.setTemperature(query.value(i++).toDouble());

			pexline.e3pZone1.setTemperature(query.value(i++).toDouble());
			pexline.e3pZone2.setTemperature(query.value(i++).toDouble());
			pexline.e3pZone3.setTemperature(query.value(i++).toDouble());
			pexline.e3pZone4.setTemperature(query.value(i++).toDouble());
			pexline.e3pTrace.setTemperature(query.value(i++).toDouble());
		
			pexline.e3mZone1.setTemperature(query.value(i++).toDouble());
			pexline.e3mZone2.setTemperature(query.value(i++).toDouble());
			pexline.e3mZone3.setTemperature(query.value(i++).toDouble());
			pexline.e3mTrace.setTemperature(query.value(i++).toDouble());
		
			pexline.sgZone1.setTemperature(query.value(i++).toDouble());
			pexline.sgZone2.setTemperature(query.value(i++).toDouble());
		
			pexline.e2pInvertor.setFrequency(query.value(i++).toDouble());
			pexline.e2kInvertor.setFrequency(query.value(i++).toDouble());
			pexline.e3kInvertor.setFrequency(query.value(i++).toDouble());
			pexline.e3pInvertor.setFrequency(query.value(i++).toDouble());
			pexline.e3mInvertor.setFrequency(query.value(i++).toDouble());
			
			pexline.sgInvertor.setFrequency(query.value(i++).toDouble());
			pexline.tuInvertor.setFrequency(query.value(i++).toDouble());
		
			driveParamSgDiameterEntry->setValue(query.value(i++).toDouble());
			
			box.hide();
		}
		else
			MyMessageBox(QString("Загрузка профиля"), QString("Ошибка загрузки профиля"));
	}
	
	profilesUpdateTable();
}

void MainWindow::on_profilesDeleteButton_clicked()
{
	QTableWidgetItem * item = profilesTable->item(profilesTable->currentRow(), 1);
	
	if (!item)
		return;
	
	QString	profileName = item->text();

	if (1 == MyMessageBox(QString("Удаление профиля"),
		QString("Удалить профиль ") + profileName + QString(" ?"),
		QString("Да"), QString("Нет")))
	{
		QSqlQuery	query;
		query.prepare("delete from profiles where name = :name");
		query.bindValue(":name", profileName);
		if (query.exec())
			MyMessageBox(QString("Удаление профиля"), QString("Профиль удален"));
		else
			MyMessageBox(QString("Удаление профиля"), QString("Ошибка удаления профиля"));
	}

	profilesUpdateTable();
}
	
void MainWindow::profilesUpdateTable()
{
	profilesTable->clear();
	profilesTable->setColumnCount(2);
	profilesTable->setRowCount(0);

	QStringList	headers;
	headers << QString("Дата/время") << QString("Профиль");
	profilesTable->setHorizontalHeaderLabels(headers);

	int row = 0;
	int warningCode;

	QSqlQuery	query("select dateTime, name from profiles order by dateTime");

	query.exec();

	while (query.next())
	{
		profilesTable->insertRow(row);
		profilesTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
		profilesTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
		row++;
	}

	profilesTable->resizeColumnToContents(0);
	profilesTable->setColumnWidth(1, profilesTable->width() - profilesTable->columnWidth(0));
	
	profilesTable->verticalHeader()->hide();
	profilesTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	profilesTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	int rowCount = profilesTable->rowCount();
	
	if (rowCount)
		profilesTable->setCurrentCell(rowCount - 1, 0);

	profilesNewNameEdit->setText("");
	profilesLoadButton->setEnabled(profilesTable->rowCount());
	profilesDeleteButton->setEnabled(profilesTable->rowCount());
}
	
void MainWindow::on_profilesTable_currentCellChanged(int curRow, int curCol, int prevRow, int prevCol)
{
	int rowCount = profilesTable->rowCount();

	profilesUpButton->setEnabled(curRow > 0);
	profilesDownButton->setEnabled(curRow < rowCount - 1);
}

void MainWindow::on_profilesNewNameEdit_textChanged(const QString & text)
{
	profilesSaveButton->setDisabled(text.isEmpty());
}
	
void MainWindow::on_printerMsg1Button_clicked(bool on)
{
	if (!on)
	{
		printerMsg1Button->setChecked(true);
		return;
	}	

	if (!pexline.printer.isConnected())
	{
		postWarning(PrinterDisconnected);
		return;
	}

	pexline.printer.setMsgNo(1);
		
	printerMsg2Button->setChecked(false);
	printerMsg3Button->setChecked(false);
	printerMsg4Button->setChecked(false);	
}

void MainWindow::on_printerMsg2Button_clicked(bool on)
{
	if (!on)
	{
		printerMsg2Button->setChecked(true);
		return;
	}	

	if (!pexline.printer.isConnected())
	{
		postWarning(PrinterDisconnected);
		return;
	}

	pexline.printer.setMsgNo(2);
		
	printerMsg1Button->setChecked(false);
	printerMsg3Button->setChecked(false);
	printerMsg4Button->setChecked(false);	
}
	
void MainWindow::on_printerMsg3Button_clicked(bool on)
{
	if (!on)
	{
		printerMsg3Button->setChecked(true);
		return;
	}	
		
	if (!pexline.printer.isConnected())
	{
		postWarning(PrinterDisconnected);
		return;
	}

	pexline.printer.setMsgNo(3);
		
	printerMsg1Button->setChecked(false);
	printerMsg2Button->setChecked(false);
	printerMsg4Button->setChecked(false);	
}

void MainWindow::on_printerMsg4Button_clicked(bool on)
{
	if (!on)
	{
		printerMsg4Button->setChecked(true);
		return;
	}	

	if (!pexline.printer.isConnected())
	{
		postWarning(PrinterDisconnected);
		return;
	}

	pexline.printer.setMsgNo(4);
	
	printerMsg1Button->setChecked(false);
	printerMsg2Button->setChecked(false);
	printerMsg3Button->setChecked(false);	
}
	
bool MainWindow::on_printerRunStopButton_clicked(bool state)
{
	bool new_state = state;

	if (!pexline.printer.isConnected())
	{
		postWarning(PrinterDisconnected);
		new_state = false;
	}
		
	printerRunStopButton->setChecked(new_state);
	if (!pexline.printer.setRunStop(new_state))
		return false;
		
	return state == new_state;

}
	
void MainWindow::onPrinterMsgNoChanged(int no)
{
	if (1 == no)
	{
		printerMsg1Button->setChecked(true);
		printerMsg2Button->setChecked(false);
		printerMsg3Button->setChecked(false);
		printerMsg4Button->setChecked(false);	
	}	
	else if (2 == no)
	{
		printerMsg1Button->setChecked(false);
		printerMsg2Button->setChecked(true);
		printerMsg3Button->setChecked(false);
		printerMsg4Button->setChecked(false);	
	}	
	else if (3 == no)
	{
		printerMsg1Button->setChecked(false);
		printerMsg2Button->setChecked(false);
		printerMsg3Button->setChecked(true);
		printerMsg4Button->setChecked(false);	
	}	
	else if (4 == no)
	{
		printerMsg1Button->setChecked(false);
		printerMsg2Button->setChecked(false);
		printerMsg3Button->setChecked(false);
		printerMsg4Button->setChecked(true);	
	}	
}

