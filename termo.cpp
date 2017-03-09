#include "termo.h"

bool Termo::setRunStop(bool run)
{
	int tmp = _runStop.rawValue();

	if (run)
		_runStop.setRawValue(_runStop.writeOnValue(), false);
	else
		_runStop.setRawValue(_runStop.writeOffValue(), false);

	if (!storeValue(_runStop, true))
	{
		_runStop.setRawValue(tmp, true);
		return false;
	}
	
	return true;
}

bool Termo::setTemperature(double t)
{
	double minc = _minCritDelta;
	double maxc = _maxCritDelta;
	double maxw = _maxWarnDelta;
	double minw = _minWarnDelta;

	if (t < 0.)
		t = 0.;

	double pr = _temperature.presetValue().scaledValue();

	_temperature.presetValue().setScaledValue(t, false);
	if (!storeValue(_temperature.presetValue(), true))
	{
		_temperature.presetValue().setScaledValue(pr, true);
		return false;
	}

	setMinCritDelta(minc);
	setMaxCritDelta(maxc);

	_temperature.checkCurrentValue();

	return true;
}

bool Termo::setLoCritBound(double t)
{
	double tmp;

	if (t < 0.)
		t = 0.;

	tmp = _temperature.loCriticalBound().scaledValue();

	if (t != tmp)
	{
		_temperature.loCriticalBound().setScaledValue(t, false);
		if (!storeValue(_temperature.loCriticalBound(), true))
		{
//			_temperature.loCriticalBound().setScaledValue(tmp, true);
//			return false;
		}	
	}
	
	_temperature.checkCurrentValue();

	return true;
}

bool Termo::setHiCritBound(double t)
{
	double tmp;

	if (t < 0.)
		t = 0.;

	tmp = _temperature.hiCriticalBound().scaledValue();
	
	if (t != tmp)
	{
		_temperature.hiCriticalBound().setScaledValue(t, false);
		if (!storeValue(_temperature.hiCriticalBound(), true))
		{
//			_temperature.hiCriticalBound().setScaledValue(tmp, true);
//			return false;
		}
	}
	
	_temperature.checkCurrentValue();

	return true;
}

bool Termo::setLoWarnBound(double t)
{
	double tmp;

	if (t < 0.)
		t = 0.;

	tmp = _temperature.loWarnBound().scaledValue();

	if (t != tmp)
	{
		_temperature.loWarnBound().setScaledValue(t, false);
		if (!storeValue(_temperature.loWarnBound(), true))
		{
//			_temperature.loWarnBound().setScaledValue(tmp, true);
//			return false;
		}
	}
	
	_temperature.checkCurrentValue();

	return true;
}

bool Termo::setHiWarnBound(double t)
{
	double tmp;
	
	if (t < 0.)
		t = 0.;

	tmp = _temperature.hiWarnBound().scaledValue();
	
	if (t != tmp)
	{
		_temperature.hiWarnBound().setScaledValue(t, false);
		if (!storeValue(_temperature.hiWarnBound(), true))
		{
//			_temperature.hiWarnBound().setScaledValue(tmp, true);
//			return false;
		}
	}
	
	_temperature.checkCurrentValue();

	return true;
}

bool Termo::setMinCritDelta(double t)
{
	_minCritDelta = t;
	setMinWarnDelta(t);
	return setLoCritBound(_temperature.presetValue().scaledValue() - t);
}

bool Termo::setMaxCritDelta(double t)
{
	_maxCritDelta = t;
	setMaxWarnDelta(t);
	
	return setHiCritBound(_temperature.presetValue().scaledValue() + t);
}

bool Termo::setMinWarnDelta(double t)
{
	_minWarnDelta = t;
	return setLoWarnBound(_temperature.presetValue().scaledValue() - t);
}

bool Termo::setMaxWarnDelta(double t)
{
	_maxWarnDelta = t;
	return setHiWarnBound(_temperature.presetValue().scaledValue() + t);
}


bool Termo::poll(bool overrideEmit)
{
	if (!loadValue(_runStop, overrideEmit))
		return false;
	
//	if (!loadValue(_temperature.loCriticalBound(), overrideEmit))
//		return false;
	
//	if (!loadValue(_temperature.loWarnBound(), overrideEmit))
//		return false;
	
//	if (!loadValue(_temperature.hiCriticalBound(), overrideEmit))
//		return false;
	
//	if (!loadValue(_temperature.hiWarnBound(), overrideEmit))
//		return false;
	
	if (!loadValue(_temperature.currentValue(), overrideEmit))
			return false;
	
	if (!loadValue(_temperature.presetValue(), overrideEmit))
		return false;
	else
	{
		setMinCritDelta(_minCritDelta);
		setMaxCritDelta(_maxCritDelta);
	}
	
	_temperature.checkCurrentValue(overrideEmit);

	return true;
}

#define LOCAL_MAX_ERROR_COUNT	(2)

bool Termo::loadValue(StoredValue & sv, bool overrideEmit)
{
	if (!sv.loadable())
		return true;

	if (isDisconnected() && _stopIoIfDisconnected)
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
		sv.readAddr(),
		1);

	ModbusMsg	ack;
	bool		result = false;
	int			error_count = 0;
	QMutexLocker	locker(&(_port->mutex));
	
	while (error_count < LOCAL_MAX_ERROR_COUNT) // igk
	{
		error_count++;

		Sleep(PORT_LOCK_DELAY);
	
		result = _port->writeMsg(msg);
	
		if (!result)
			continue;
				
		result = _port->readMsg(ack);
			
		if (result)
			if ((ModbusMsg::Read == ack.cmdCode()) &&
				(ModbusMsg::Error != ack.msgType()) &&
				(_deviceId == ack.devId()) &&
				(1 == ack.data().size()))
					
				break;
				
		result = false;
	}
	
	if (!result)
	{
//		disconnect(true);
	}
	else if ((ModbusMsg::Read == ack.cmdCode()) &&
				(ModbusMsg::Error != ack.msgType()) &&
				(_deviceId == ack.devId()) &&
				(1 == ack.data().size()))
	
		sv.setRawValue(ack.data()[0], true, overrideEmit);
	
	return result;
}


