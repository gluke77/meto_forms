// cpu.h: interface for the Cpu class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CPU_INCLUDED
#define _CPU_INCLUDED

#include <QObject>

#include "basedevice.h"
#include "baseport.h"
#include "storedvalue.h"

class Cpu : public BaseDevice  
{

Q_OBJECT

	enum Controls
	{
		WaterMode0 = 0,
		WaterMode1 = 1,
		CoolerPump = 2,
		ExtruderPump = 3,
		CoolerTube = 4,
		AirInTube = 5,
		Sg = 6,
		Drier = 7,
		Germo = 8,
		LampStop = 10,
		TractorLock = 11,
		BunkerMotor = 12,
		FoilLed = 13,
		SirenMode0 = 14,
		SirenMode1 = 15
	};

	enum Sensors
	{
		BathWaterLevel0 = 13,
		BathWaterLevel1 = 15,
		TankWaterLevel0 = 9,
		TankWaterLevel1 = 11,
		TankWaterLevel2 = 2,
		TankWaterLevel3 = 0,
		HeadWater = 10,
		E2Water = 8,
		E3Water = 7,
		GermoBegin = 14,
		GermoEnd = 12,
		ButtonPressure = 3,
		ButtonGermo = 5,
		ButtonStop = 17,
		FoilEncoder = 23,
		Pressure = 16,
		HeadUp = 4,
		HeadDown = 6,
		E2PEmpty = 21,
		E2KEmpty = 19,
		E3MEmpty = 22,
		E3PEmpty = 20,
		E3KEmpty = 18,
		EndOfFoil = 24,
		PrimaryReel = 25,
		SecondaryReel = 26
	};

	void setControl(Controls control_id, bool on = true);
	bool controlOn(Controls control_id);
	bool sensorOn(Sensors sensor_id);
	bool testBit(int test, int bit);

	StoredValue	_controls;
	StoredValue _sensors;

public:

	Cpu(BasePort * port = 0, int deviceId = 0, const TriggerValue & con = TriggerValue(false, 0, 0, false, false) )
		: BaseDevice(port, deviceId, con),
		_controls(StoredValue(0, 1., 0, 0, true, true)),
		_sensors(StoredValue(0, 1., 0, 0, false, true))
		{};
	virtual ~Cpu();

	Cpu(const Cpu & c)
	{
		_controls = c.controls();
		_sensors = c.sensors();

		_deviceId = c.deviceId();
		_port = c.port();
		_connected = c.tvConnected();
	};

	Cpu & operator=(const Cpu & c)
	{
		if (this == &c)
			return *this;

		_controls = c.controls();
		_sensors = c.sensors();

		_deviceId = c.deviceId();
		_port = c.port();
		_connected = c.tvConnected();

		return *this;
	};

	const StoredValue & controls() const
	{
		return _controls;
	};

	StoredValue & controls()
	{
		return _controls;
	};

	const StoredValue & sensors() const
	{
		return _sensors;
	};

	StoredValue & sensors()
	{
		return _sensors;
	};

	int waterMode();
	bool coolerPumpOn();
	bool extruderPumpOn();
	bool coolerTubeOn();
	bool airInTubeOn();
	bool headDown();
	bool drierOn();
	bool germoOn();
	bool lampStopOn();
	bool tractorLockOn();
	bool bunkerMotorOn();
	bool foilLedOn();
	int sirenMode();

	bool bathWaterLevel0On();
	bool bathWaterLevel1On();
	bool tankWaterLevel0On();
	bool tankWaterLevel1On();
	bool tankWaterLevel2On();
	bool tankWaterLevel3On();
	bool headWaterOn();
	bool e2WaterOn();
	bool e3WaterOn();
	bool germoBeginOn();
	bool germoEndOn();
	bool buttonPressureOn();
	bool buttonGermoOn();
	bool buttonStopOn();
	bool foilEncoderOn();
	bool pressureOn();
	bool headUpOn();
	bool headDownOn();
	bool e2PEmptyOn();
	bool e2KEmptyOn();
	bool e3MEmptyOn();
	bool e3PEmptyOn();
	bool e3KEmptyOn();
	bool endOfFoilOn();
	bool primaryReelOn();
	bool secondaryReelOn();

	

public slots:

	void dumbSlot() {};

	virtual bool poll(bool overrideEmit = false);

	void setWaterMode(int mode = 0);
	void setCoolerPump(bool on = true);
	void setExtruderPump(bool on = true);
	void setCoolerTube(bool on = true);
	void setAirInTube(bool on = true);
	void setHeadDown(bool on = true);
	void setDrier(bool on = true);
	void setGermo(bool on = true);
	void setLampStop(bool on = true);
	void setTractorLock(bool on = true);
	void setBunkerMotor(bool on = true);
	void setFoilLed(bool on = true);
	void setSirenMode(int mode = 0);

signals:

	void dumbSignal();

	void waterModeChanged(int);
	void coolerPumpChanged(bool);
	void extruderPumpChanged(bool);
	void coolerTubeChanged(bool);
	void airInTubeChanged(bool);
	void sgChanged(bool);
	void drierChanged(bool);
	void germoChanged(bool);
	void lampStopChanged(bool);
	void tractorLockChanged(bool);
	void bunkerMotorChanged(bool);
	void foilLedChanged(bool);
	void sirenModeChanged(int);

	void bathWaterLevel0Changed(bool);
	void bathWaterLevel1Changed(bool);
	void tankWaterLevel0Changed(bool);
	void tankWaterLevel1Changed(bool);
	void tankWaterLevel2Changed(bool);
	void tankWaterLevel3Changed(bool);
	void headWaterChanged(bool);
	void e2WaterChanged(bool);
	void e3WaterChanged(bool);
	void germoBeginChanged(bool);
	void germoEndChanged(bool);
	void buttonPressureChanged(bool);
	void buttonGermoChanged(bool);
	void buttonStopChanged(bool);
	void foilEncoderChanged(bool);
	void foilEncoderChangedInverse(bool);
	void pressureChanged(bool);
	void headUpChanged(bool);
	void headDownChanged(bool);
	void e2PEmptyChanged(bool);
	void e2KEmptyChanged(bool);
	void e3MEmptyChanged(bool);
	void e3PEmptyChanged(bool);
	void e3KEmptyChanged(bool);
	void endOfFoilChanged(bool);
	void primaryReelChanged(bool);
	void secondaryReelChanged(bool);

};

#endif // _CPU_INCLUDED
