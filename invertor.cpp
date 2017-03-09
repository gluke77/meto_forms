#include "invertor.h"

bool Invertor::setRunStop(bool run)
{
	int	tmp = _runStop.rawValue();
	
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

bool Invertor::poll(bool overrideEmit)
{
	if (!loadValue(_runStop, overrideEmit))
		return false;

	if (!loadValue(_current.loCriticalBound(), overrideEmit))
		return false;
		
	if (!loadValue(_current.loWarnBound(), overrideEmit))
		return false;
		
	if (!loadValue(_current.hiCriticalBound(), overrideEmit))
		return false;
	
	if (!loadValue(_current.hiWarnBound(), overrideEmit))
		return false;
		
	if (!loadValue(_current.currentValue(), overrideEmit))
		return false;
		
	if (!loadValue(_current.presetValue(), overrideEmit))
		return false;
	
	if (!loadValue(_frequency, overrideEmit))
		return false;
		
	_current.checkCurrentValue(overrideEmit);

	return true;
}

bool Invertor::setFrequency(double f)
{
	double tmp;

	tmp = _frequency.scaledValue();
	_frequency.setScaledValue(f, false);
	if (!storeValue(_frequency, true))
	{
		_frequency.setScaledValue(tmp, true);
		return false;
	}

	return true;
}

bool Invertor::setCurrentLoCriticalBound(double bound)
{
	double tmpC = _current.loCriticalBound().scaledValue();
	double tmpW = _current.loWarnBound().scaledValue();
	
	_current.loCriticalBound().setScaledValue(bound, false);
	
	bool res = storeValue(_current.loCriticalBound(), true);

	if (!res)
	{
		_current.loCriticalBound().setScaledValue(tmpC, true);
		return false;
	}
	
	_current.loWarnBound().setScaledValue(bound, false);
	res = storeValue(_current.loWarnBound(), true);

	if (!res)
	{
		_current.loCriticalBound().setScaledValue(tmpC, true);
		_current.loWarnBound().setScaledValue(tmpW, true);
		return false;
	}
	_current.checkCurrentValue(true);

	return true;
}


bool Invertor::setCurrentHiCriticalBound(double bound)
{
	double tmpC = _current.hiCriticalBound().scaledValue();
	double tmpW = _current.hiWarnBound().scaledValue();
	
	_current.hiCriticalBound().setScaledValue(bound, false);
	
	bool res = storeValue(_current.hiCriticalBound(), true);

	if (!res)
	{
		_current.hiCriticalBound().setScaledValue(tmpC, true);
		return false;
	}
	
	_current.hiWarnBound().setScaledValue(bound, false);
	res = storeValue(_current.hiWarnBound(), true);

	if (!res)
	{
		_current.hiCriticalBound().setScaledValue(tmpC, true);
		_current.hiWarnBound().setScaledValue(tmpW, true);
		return false;
	}
	_current.checkCurrentValue(true);

	return true;
}

bool SgInvertor::setFrequency(double f)
{
	double tmp;

	tmp = _frequency.scaledValue();

	int rv = _frequency.rawForScaled(f);
	
	if ((2530 < rv) && (rv < 2550))
		_frequency.setRawValue(2530, false);
	else
		_frequency.setScaledValue(f, false);
	
	if (!storeValue(_frequency, true))
	{
		_frequency.setScaledValue(tmp, true);
		return false;
	}

	return true;
}


