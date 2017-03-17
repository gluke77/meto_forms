#ifndef _MAINWINDOW_INCLUDED
#define _MAINWINDOW_INCLUDED

#include <QtGui>
#include <QMutex>
#include <QMutexLocker>
#include "ui_MainWindow.h"

#include "logindialog.h"
#include "graphselectdialog.h"

#include "graph.h"

class Warning
{
public:
	QDateTime raised;
	QDateTime accepted;
	int code;
	int id;
	QString user;
	bool isAccepted;
	
	Warning(int _code = 0, int _id = 0, QString _user = QString(""), bool _isAccepted = false) :
		code(_code), id(_id), user(_user), isAccepted(_isAccepted)
	{
		raised = QDateTime::currentDateTime();
		if (isAccepted)
			accepted = QDateTime::currentDateTime();
	}
};



class MainWindow : public QWidget, public Ui_MainWindow
{
	Q_OBJECT

    QMutex warningMutex;

public:

	MainWindow(QWidget * parent = 0);
	
	enum WarningCodes
	{
		PrinterDisconnected,
		UzgDisconnected,
		CpuDisconnected,
		SgDisconnected,
		TuDisconnected,
		TankWaterLevel0,
		TankWaterLevel1,
		TankWaterLevel2,
		Pressure,
		FoilEncoder,
		SgWater,
		E2Water,
		E3Water,
		AirInTube,
		GermoBegin,
		UzgBusy,
		UzgOverheat,
		SgOvercurrent,
		TuOvercurrent,
		E2pOvercurrent,
		E2kOvercurrent,
		E3pOvercurrent,
		E3kOvercurrent,
		E3mOvercurrent,
		E2pOverpressure,
		E2kOverpressure,
		E3pOverpressure,
		E3kOverpressure,
		E3mOverpressure,
		E2pDisconnected,
		E2kDisconnected,
		E3pDisconnected,
		E3kDisconnected,
		E3mDisconnected,
		UzgStop,
		AlarmStop,
		NewUser,
		AutostopLine,
		AutostartLine,
		AutostartLineOk,
		AutostartLineCancel,
		AutostopSvarka,
		AutostartSvarka,
		AutostartSvarkaOk,
		AutostartSvarkaCancel,
		E2pTemperature,
		E2kTemperature,
		E3kTemperature,
		E3pTemperature,
		E3mTemperature,
		SgTemperature,
		DebugModeStart,
		DebugModeStop,
		EndOfFoil,
		PrimaryReel,
		SecondaryReel,
		Quit,
		E2KEmpty,
		E2PEmpty,
		E3PEmpty,
		E3KEmpty,
		E3MEmpty
	};
	
	enum ScreenNames
	{
		main = 0,
		termo1,
		termo2,
		termo3,
		warnBounds,
		deviceParams,
		svarkaStart,
		lineStart,
		operTable,
		uzg,
		printer,
		log,
		graph,
		profiles
	};
	
public slots:

	void showMenu();
	void showScreen(int);
	
	void showMainScreen();
	void showTermo1();
	void showTermo2();
	void showTermo3();
	void showDeviceParams();
	void showWarnBounds();
	void showLineStart();
	void showSvarkaStart();
	void showOperTable();
	void showUzg();
	void showPrinter();
	void showLog();
	void showGraph();
	void showProfiles();
	
	void allTermoOn1();
	void allTermoOn2();
	void allTermoOn3();
	
	void allTermoOff1();
	void allTermoOff2();
	void allTermoOff3();

	void setDebug(bool check = true);

	void setUser(QString, int);
	
	void on_operOperButton_clicked();
	void on_techOperButton_clicked();
	void on_adminOperButton_clicked();
	void on_addOperButton_clicked();
	void on_deleteOperButton_clicked();
	void on_mainClockTimer_timeout();

	void refreshOperTable();
	void refreshLogOperTable();
	void on_logUpdateButton_clicked();

	void on_logCommitButton_clicked();
	void on_logUpButton_clicked();
	void on_logDownButton_clicked();

	void on_logBeginNowButton_clicked();
	void on_logEndNowButton_clicked();
	
	void on_logUpOperButton_clicked();
	void on_logDownOperButton_clicked();

	void on_logListTable_currentCellChanged(int curRow, int curCol, int prevRow, int prevCol);
	void on_logUpListButton_clicked();
	void on_logDownListButton_clicked();
	
	void on_logFromBuffer1Button_clicked();
	void on_logFromBuffer2Button_clicked();
	void on_logToBuffer1Button_clicked();
	void on_logToBuffer2Button_clicked();

	void on_graphBeginNowButton_clicked();
	void on_graphEndNowButton_clicked();
	void on_graphFromBuffer1Button_clicked();
	void on_graphFromBuffer2Button_clicked();

	void on_graphSelect1Button_clicked();
	void on_graphSelect2Button_clicked();
	void on_graphSelect3Button_clicked();
	
	void on_graphUpdateButton_clicked();
	void graphUpdate();
	
	void on_graphLeftButton_clicked();
	void on_graphRightButton_clicked();
	void on_graphDownButton_clicked();
	void on_graphUpButton_clicked();

	void on_graphVer1Button_clicked();
	void on_graphVer5Button_clicked();
	void on_graphVer25Button_clicked();

	void on_graphHor1Button_clicked();
	void on_graphHor5Button_clicked();
	void on_graphHor25Button_clicked();

	void checkVerPos();
	void checkHorPos();

	void checkVerButtons();
	void checkHorButtons();

	void updateLogUpDownOperButtons();

	void on_profilesUpButton_clicked();
	void on_profilesDownButton_clicked();
	void on_profilesSaveButton_clicked();
	void on_profilesLoadButton_clicked();
	void on_profilesDeleteButton_clicked();
	void on_profilesTable_currentCellChanged(int curRow, int curCol, int prevRow, int prevCol);
	void on_profilesNewNameEdit_textChanged(const QString & text);
	
	void profilesUpdateTable();
	
	void postWarning(int code, int sirenMode = 0);
	void showWarning();

	void on_coolerPumpButton_clicked(bool);
	void on_extruderPumpButton_clicked(bool);
	void on_germoButton_clicked(bool);
	bool on_pushdownTractorButton_clicked(bool);
	void on_drierButton_clicked(bool);
	void on_pressureButton_clicked(bool);
	void on_waterButton_clicked();
//	void on_foilControlButton_clicked(bool);

	bool on_driveParamPe1MotorStartButton_clicked(bool);
	bool on_driveParamPe2MotorStartButton_clicked(bool);
	bool on_driveParamKl1MotorStartButton_clicked(bool);
	bool on_driveParamKl2MotorStartButton_clicked(bool);
	bool on_driveParamPemMotorStartButton_clicked(bool);
	bool on_driveParamTuMotorStartButton_clicked(bool);
	bool on_driveParamSgMotorStartButton_clicked(bool);
	bool on_driveParamSgPositionButton_clicked(bool);
	bool on_driveParamUzgStartButton_clicked(bool);

	void on_lineSvarkaSelectButton_clicked();

	void on_driveParamTuMotorEntry_valueChanged();
	void on_driveParamSgMotorEntry_valueChanged();
	void on_driveParamSgSpeedEntry_valueChanged();
	void on_driveParamSgDiameterEntry_valueChanged();
	
	bool on_printerRunStopButton_clicked(bool);
	void on_printerMsg1Button_clicked(bool);
	void on_printerMsg2Button_clicked(bool);
	void on_printerMsg3Button_clicked(bool);
	void on_printerMsg4Button_clicked(bool);
	void onPrinterMsgNoChanged(int);
//	void onPrinterDisconnected();

	
	void setWaterMode(int);
	
	void onCpuDisconnected();
	void onUzgDisconnected();

	void onTankWaterLevel0Changed(bool);
	void onTankWaterLevel1Changed(bool);
	void onTankWaterLevel2Changed(bool);
	
	void onPressureChanged(bool);
	void onFoilEncoderChanged(bool);
	void onSgRunStopChanged(bool);
	void onUzgRunStopChanged(bool);
	void onSgWaterChanged(bool);
	void onE2WaterChanged(bool);
	void onE3WaterChanged(bool);
	
	void onUzgTemperatureChanged(double);
	void onStopButtonChanged(bool);
	
	void onEndOfFoilChanged(bool);
	void onPrimaryReelChanged(bool);
	void onSecondaryReelChanged(bool);
	
	void onTuCurrentStateChanged(int);
	void onSgCurrentStateChanged(int);
	void onE2pCurrentStateChanged(int);
	void onE2kCurrentStateChanged(int);
	void onE3pCurrentStateChanged(int);
	void onE3kCurrentStateChanged(int);
	void onE3mCurrentStateChanged(int);

	void onE2pPressureStateChanged(int);
	void onE2kPressureStateChanged(int);
	void onE3pPressureStateChanged(int);
	void onE3kPressureStateChanged(int);
	void onE3mPressureStateChanged(int);
	
	void onE2PEmptyChanged(bool);
	void onE2KEmptyChanged(bool);
	void onE3MEmptyChanged(bool);
	void onE3PEmptyChanged(bool);
	void onE3KEmptyChanged(bool);
	
	void checkMnemoE2pLed(int state = 0);
	void checkMnemoE2kLed(int state = 0);
	void checkMnemoE3kLed(int state = 0);
	void checkMnemoE3pLed(int state = 0);
	void checkMnemoE3mLed(int state = 0);
	void checkMnemoSgLed(int state = 0);
	void checkMnemoTuLed(int state = 0);
	void checkMnemoBathLed(int state = 0);

	void alarmStop(int sirenMode = 1);

	bool autostartCheck(int);
	bool execute(int, bool);
	bool doAutostart(bool);
	
	void showSvarkaStartTime();
	void showSvarkaStopTime();
	void showLineStartTime();
	void showLineStopTime();
	
	void showBottomEdit();
	
	void checkAutostartLeds();
	
	void storeSettings();
	void loadSettings();
	
	void tryQuit();
	
	void storeProcess();

private:
	
	QMenu * menu;
	
	QAction * lineStartAct;
	QAction * svarkaStartAct;
	QAction * deviceParamsAct;
	QAction * warnBoundsAct;
	QAction * termo1Act;
	QAction * termo2Act;
	QAction * termo3Act;
	QAction * graphAct;
	QAction * rasxodAct;
	QAction * logAct;
	QAction * userAct;
	QAction * saveLoadAct;
	QAction * debugAct;
	QAction * uzgAct;
	QAction * printerAct;
	QAction * quitAct;

	QString	_user;
	int		_userType;

	int		_waterMode;

	bool	_debugMode;
	bool	_checkSgWater;
	bool	_checkE2Water;
	bool	_checkE3Water;
	bool	_checkFoilEncoder;
	
	bool	_lineStarted;

	LoginDialog * _loginDialog;
	GraphSelectDialog * _graphSelectDialog;
	
	QTimer * mainClockTimer;
	QTimer * storeProcessClockTimer;
	
	void createMenu();
	void createActions();
	void doConnections();
	void setupButtons();
	void fillWarningMap();
	
	int _nextWarningId;

	QList<Warning>::iterator _currentWarning;

	QMap<int, QString> _warningTextMap;
	QList<Warning> _acceptedWarningList;
	QList<Warning> _unacceptedWarningList;
	
	QDateTime	_logBuffer1DateTime;
	QDateTime	_logBuffer2DateTime;
	
	QMovie		_mnemoMovie;
	
	int	_graph1Type;
	int	_graph2Type;
	int	_graph3Type;
	
	int _graphHorScale;
	int _graphHorPos;
	int _graphVerScale;
	int _graphVerPos;
	
	int graphMaxScale(int graphType);
	
};

#endif /* _MAINWINDOW_INCLUDED */
