#include "cpu.h"
#include "baseport.h"
#include "modbusmsg.h"
#include "basedevice.h"
#include <windows.h>

#include <QMutexLocker>

Cpu::~Cpu()
{

}

void Cpu::setControl(enum Cpu::Controls control_id, bool on)
{
	if (WaterMode0 == control_id || WaterMode1 == control_id ||
		SirenMode0 == control_id || SirenMode1 == control_id)

		return;

	int id = (int)control_id;

    QMutexLocker locker(&mutex);
	int ctrls = _controls.rawValue();
	
	if (on)
		ctrls |= 1 << id;
	else
		ctrls &= ~(1 << id);

	if (ctrls == _controls.rawValue()) 
        return;

    _controls.setRawValue(ctrls, false);
    storeValue(_controls, true);
}

bool Cpu::controlOn(Cpu::Controls control_id)
{
	if (WaterMode0 == control_id || WaterMode1 == control_id ||
		SirenMode0 == control_id || SirenMode1 == control_id)

		return false;

	return _controls.rawValue() & (1 << (int)control_id);
}

bool Cpu::coolerPumpOn()
{
	return controlOn(CoolerPump);
}

bool Cpu::extruderPumpOn()
{
	return controlOn(ExtruderPump);
}

bool Cpu::coolerTubeOn()
{
	return controlOn(CoolerTube);
}

bool Cpu::airInTubeOn()
{
	return controlOn(AirInTube);
}

bool Cpu::headDown()
{
	return controlOn(Sg);
}

bool Cpu::drierOn()
{
	return controlOn(Drier);
}

bool Cpu::germoOn()
{
	return controlOn(Germo);
}

bool Cpu::lampStopOn()
{
	return controlOn(LampStop);
}

bool Cpu::tractorLockOn()
{
	return controlOn(TractorLock);
}

bool Cpu::bunkerMotorOn()
{
	return controlOn(BunkerMotor);
}

bool Cpu::foilLedOn()
{
	return controlOn(FoilLed);
}

int Cpu::sirenMode()
{
	int mask = (1 << (int)SirenMode0) | (1 << (int)SirenMode1);
	int shift = (int)SirenMode0;

	return (_controls.rawValue() & mask) >> shift;
}

int Cpu::waterMode()
{
	int mask = (1 << (int)WaterMode0) | (1 << (int)WaterMode1);
	int shift = (int)WaterMode0;

	return (_controls.rawValue() & mask) >> shift;

}

void Cpu:: setCoolerPump(bool on)
{
	setControl(CoolerPump, on);
}

void Cpu:: setExtruderPump(bool on)
{
	setControl(ExtruderPump, on);
}

void Cpu:: setCoolerTube(bool on)
{
	setControl(CoolerTube, on);
}

void Cpu:: setAirInTube(bool on)
{
	setControl(AirInTube, on);
}

void Cpu:: setHeadDown(bool on)
{
	setControl(Sg, on);
}

void Cpu:: setDrier(bool on)
{
	setControl(Drier, on);
}

void Cpu:: setGermo(bool on)
{
	setControl(Germo, on);
}

void Cpu:: setLampStop(bool on)
{
	setControl(LampStop, on);
}

void Cpu:: setTractorLock(bool on)
{
	setControl(TractorLock, on);
}

void Cpu:: setBunkerMotor(bool on)
{
	if (on)
		setControl(BunkerMotor, on);
}

void Cpu:: setFoilLed(bool on)
{
	setControl(FoilLed, on);
}

void Cpu:: setSirenMode(int mode)
{
	if (0 > mode || mode > 3)
		mode = 0;

	QMutexLocker locker(&mutex);
    
    int ctrls = _controls.rawValue();

	int mask = (1 << (int)SirenMode0) | (1 << (int)SirenMode1);
	int shift = (int)SirenMode0;

	ctrls &= ~mask;
	ctrls |= mode << shift;

    if (ctrls == _controls.rawValue())
        return;

	_controls.setRawValue(ctrls, true);
	storeValue(_controls, false);
}

void Cpu:: setWaterMode(int mode)
{
	if (0 > mode || mode > 2)
		mode = 0;

	QMutexLocker locker(&mutex);

	int ctrls = _controls.rawValue();

	int mask = (1 << (int)WaterMode0) | (1 << (int)WaterMode1);
	int shift = (int)WaterMode0;

	ctrls &= ~mask;
	ctrls |= mode << shift;

    if (ctrls == _controls.rawValue())
        return;

	_controls.setRawValue(ctrls, true);
	storeValue(_controls, false);
}

bool Cpu::sensorOn(Cpu::Sensors sensor_id)
{
	return ~_sensors.rawValue() & (1 << (int)sensor_id);
}

bool Cpu::bathWaterLevel0On()
{
	return sensorOn(BathWaterLevel0);
}

bool Cpu::bathWaterLevel1On()
{
	return sensorOn(BathWaterLevel1);
}

bool Cpu::tankWaterLevel0On()
{
	return sensorOn(TankWaterLevel0);
}

bool Cpu::tankWaterLevel1On()
{
	return sensorOn(TankWaterLevel1);
}

bool Cpu::tankWaterLevel2On()
{
	return sensorOn(TankWaterLevel2);
}

bool Cpu::tankWaterLevel3On()
{
	return sensorOn(TankWaterLevel3);
}

bool Cpu::headWaterOn()
{
	return sensorOn(HeadWater);
}

bool Cpu::e2WaterOn()
{
	return sensorOn(E2Water);
}

bool Cpu::e3WaterOn()
{
	return sensorOn(E3Water);
}

bool Cpu::germoBeginOn()
{
	return sensorOn(GermoBegin);
}

bool Cpu::germoEndOn()
{
	return sensorOn(GermoEnd);
}

bool Cpu::buttonPressureOn()
{
	return sensorOn(ButtonPressure);
}

bool Cpu::buttonGermoOn()
{
	return sensorOn(ButtonGermo);
}

bool Cpu::buttonStopOn()
{
	return sensorOn(ButtonStop);
}

bool Cpu::foilEncoderOn()
{
	return sensorOn(FoilEncoder);
}

bool Cpu::pressureOn()
{
	return sensorOn(Pressure);
}

bool Cpu::headUpOn()
{
	return sensorOn(HeadUp);
}

bool Cpu::headDownOn()
{
	return sensorOn(HeadDown);
}

bool Cpu::e2PEmptyOn()
{
	return sensorOn(E2PEmpty);
}

bool Cpu::e2KEmptyOn()
{
	return sensorOn(E2KEmpty);
}

bool Cpu::e3MEmptyOn()
{
	return sensorOn(E3MEmpty);
}

bool Cpu::e3PEmptyOn()
{
	return sensorOn(E3PEmpty);
}

bool Cpu::e3KEmptyOn()
{
	return sensorOn(E3KEmpty);
}

bool Cpu::endOfFoilOn()
{
	return sensorOn(EndOfFoil);
}

bool Cpu::primaryReelOn()
{
	return sensorOn(PrimaryReel);
}

bool Cpu::secondaryReelOn()
{
	return sensorOn(SecondaryReel);
}

bool Cpu::poll(bool overrideEmit)
{
	if (isDisconnected())
		return false;

	if (!_port)
	{
		disconnect(true);
		return false;
	}
	
	if (!_port->connected())
	{
		disconnect(true);
		return false;
	}

	ModbusMsg msg(ModbusMsg::Command,
		ModbusMsg::Read,
		_deviceId,
		_controls.readAddr(),
		3);

    mutex.lock();
    _port->mutex.lock();

	Sleep(PORT_LOCK_DELAY);

	if (!_port->writeMsg(msg))
	{
		disconnect(true);
        _port->mutex.unlock();
        mutex.unlock();
		return false;
	}

	ModbusMsg resp(ModbusMsg::Response,
		ModbusMsg::Read,
		_deviceId,
		_controls.readAddr(),
		3);

	if (!_port->readMsg(resp))
	{
//		disconnect(true);
        _port->mutex.unlock();
        mutex.unlock();
		return false;
	}

    _port->mutex.unlock();
	
    if (ModbusMsg::Read != resp.cmdCode())
    {
        mutex.unlock();
		return false;
    }

	if (_deviceId != resp.devId())
    {
        mutex.unlock();
        return false;
    }

//	if (sv->readAddr() != resp.addr())
//		return false;

	if (3 != resp.data().size())
    {
        mutex.unlock();
        return false;
    }
	
	int new_controls = resp.data()[0];
	int new_sensors = resp.data()[1] | (resp.data()[2] << 16);
	
	int old_controls = _controls.rawValue();
	int old_sensors = _sensors.rawValue();

	int old_water_mode = waterMode();
	int old_siren_mode = sirenMode();
	
	_controls.setRawValue(new_controls, true);
	_sensors.setRawValue(new_sensors, true);
    
    mutex.unlock();

	int changed_controls;

	if (overrideEmit)
		changed_controls = 0x7FFFFFFF;
	else
		changed_controls = new_controls ^ old_controls; // NEW EMIT HERE

	if (testBit(changed_controls, CoolerPump))
		emit coolerPumpChanged(coolerPumpOn());

	if (testBit(changed_controls, ExtruderPump))
		emit extruderPumpChanged(extruderPumpOn());

	if (testBit(changed_controls, CoolerTube))
		emit coolerTubeChanged(coolerTubeOn());

	if (testBit(changed_controls, AirInTube))
		emit airInTubeChanged(airInTubeOn());

	if (testBit(changed_controls, Sg))
		emit sgChanged(headDown());

	if (testBit(changed_controls, Drier))
		emit drierChanged(drierOn());

	if (testBit(changed_controls, Germo))
		emit germoChanged(germoOn());

	if (testBit(changed_controls, LampStop))
		emit lampStopChanged(lampStopOn());

//	if (lampStopOn())
//		emit lampStopChanged(true);

	if (testBit(changed_controls, TractorLock))
		emit tractorLockChanged(tractorLockOn());

	if (testBit(changed_controls, BunkerMotor))
		emit bunkerMotorChanged(bunkerMotorOn());

	if (testBit(changed_controls, FoilLed))
		emit foilLedChanged(foilLedOn());

	if ((waterMode() != old_water_mode))
		emit waterModeChanged(waterMode());

	if ((sirenMode() != old_siren_mode))
		emit sirenModeChanged(sirenMode());

	int changed_sensors;
	
	if (overrideEmit)
		changed_sensors = 0x7FFFFFFF;
	else
		changed_sensors = new_sensors ^ old_sensors;

	if (testBit(changed_sensors, BathWaterLevel0))
		emit bathWaterLevel0Changed(bathWaterLevel0On());
	
	if (testBit(changed_sensors, BathWaterLevel1))
		emit bathWaterLevel1Changed(bathWaterLevel1On());
	
	if (testBit(changed_sensors, TankWaterLevel0))
		emit tankWaterLevel0Changed(tankWaterLevel0On());

	if (testBit(changed_sensors, TankWaterLevel1))
		emit tankWaterLevel1Changed(tankWaterLevel1On());

	if (testBit(changed_sensors, TankWaterLevel2))
		emit tankWaterLevel2Changed(tankWaterLevel2On());

	if (testBit(changed_sensors, TankWaterLevel3))
		emit tankWaterLevel3Changed(tankWaterLevel3On());

	if (testBit(changed_sensors, HeadWater))
		emit headWaterChanged(headWaterOn());

	if (testBit(changed_sensors, E2Water))
		emit e2WaterChanged(e2WaterOn());

	if (testBit(changed_sensors, E3Water))
		emit e3WaterChanged(e3WaterOn());

	if (testBit(changed_sensors, GermoBegin))
		emit germoBeginChanged(germoBeginOn());

	if (testBit(changed_sensors, GermoEnd))
		emit germoEndChanged(germoEndOn());

	if (testBit(changed_sensors, ButtonPressure))
		emit buttonPressureChanged(buttonPressureOn());

	if (testBit(changed_sensors, ButtonGermo))
		emit buttonGermoChanged(buttonGermoOn());

	if (testBit(changed_sensors, ButtonStop))
		emit buttonStopChanged(buttonStopOn());

	if (testBit(changed_sensors, FoilEncoder))
	{
		emit foilEncoderChanged(foilEncoderOn());
		emit foilEncoderChangedInverse(!foilEncoderOn());
	}

	if (testBit(changed_sensors, Pressure))
		emit pressureChanged(pressureOn());

	if (testBit(changed_sensors, HeadUp))
		emit headUpChanged(headUpOn());

	if (testBit(changed_sensors, HeadDown))
		emit headDownChanged(headDownOn());

	if (testBit(changed_sensors, E2PEmpty))
		emit e2PEmptyChanged(e2PEmptyOn());

	if (testBit(changed_sensors, E2KEmpty))
		emit e2KEmptyChanged(e2KEmptyOn());

	if (testBit(changed_sensors, E3MEmpty))
		emit e3MEmptyChanged(e3MEmptyOn());

	if (testBit(changed_sensors, E3KEmpty))
		emit e3KEmptyChanged(e3KEmptyOn());

	if (testBit(changed_sensors, E3PEmpty))
		emit e3PEmptyChanged(e3PEmptyOn());

	if (testBit(changed_sensors, EndOfFoil))
		emit endOfFoilChanged(endOfFoilOn());

	if (testBit(changed_sensors, PrimaryReel))
		emit primaryReelChanged(primaryReelOn());

	if (testBit(changed_sensors, SecondaryReel))
		emit secondaryReelChanged(secondaryReelOn());

	return true;
}

bool Cpu::testBit(int test, int bit)
{
	return test & (1 << bit);
}

