#include "pressure.h"

bool Pressure::poll(bool overrideEmit)
{
	if (!loadValue(_pressure.loCriticalBound(), overrideEmit))
		return false;
	
	if (!loadValue(_pressure.loWarnBound(), overrideEmit))
		return false;
		
	if (!loadValue(_pressure.hiCriticalBound(), overrideEmit))
		return false;
		
	if (!loadValue(_pressure.hiWarnBound(), overrideEmit))
		return false;
		
	if (!loadValue(_pressure.currentValue(), overrideEmit))
		return false;
		
	if (!loadValue(_pressure.presetValue(), overrideEmit))
		return false;

	_pressure.checkCurrentValue(overrideEmit);

	return true;
}

bool Pressure::setPressureLoCriticalBound(double bound)
{
	double tmpC = _pressure.loCriticalBound().scaledValue();
	double tmpW = _pressure.loWarnBound().scaledValue();
	
	_pressure.loCriticalBound().setScaledValue(bound, false);
	bool res = storeValue(_pressure.loCriticalBound(), true);

	if (!res)
	{
		_pressure.loCriticalBound().setScaledValue(tmpC, true);
		return false;
	}

	_pressure.loWarnBound().setScaledValue(bound, false);
	res = storeValue(_pressure.loWarnBound(), true);

	if (!res)
	{
		_pressure.loWarnBound().setScaledValue(tmpW, true);
		_pressure.loCriticalBound().setScaledValue(tmpC, true);
		return false;
	}
	
	_pressure.checkCurrentValue(true);

	return true;
}

bool Pressure::setPressureHiCriticalBound(double bound)
{
	double tmpC = _pressure.hiCriticalBound().scaledValue();
	double tmpW = _pressure.hiWarnBound().scaledValue();
	
	_pressure.hiCriticalBound().setScaledValue(bound, false);
	bool res = storeValue(_pressure.hiCriticalBound(), true);

	if (!res)
	{
		_pressure.hiCriticalBound().setScaledValue(tmpC, true);
		return false;
	}

	_pressure.hiWarnBound().setScaledValue(bound, false);
	res = storeValue(_pressure.hiWarnBound(), true);

	if (!res)
	{
		_pressure.hiWarnBound().setScaledValue(tmpW, true);
		_pressure.hiCriticalBound().setScaledValue(tmpC, true);
		return false;
	}
	
	_pressure.checkCurrentValue(true);

	return true;
}
