#include "storedvalue.h"
#include "rangeparameter.h"

RangeParameter::RangeParameter(const StoredValue & currentValue, const StoredValue & presetValue,
	const StoredValue & hiWarnBound, const StoredValue & loWarnBound,
	const StoredValue & hiCriticalBound, const StoredValue & loCriticalBound)
{
	_currentValue = currentValue;
	connect(&_currentValue, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	_presetValue = presetValue;
	connect(&_presetValue, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	_hiWarnBound = hiWarnBound;
	connect(&_hiWarnBound, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	_loWarnBound = loWarnBound;
	connect(&_loWarnBound, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	_hiCriticalBound = hiCriticalBound;
	connect(&_hiCriticalBound, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	_loCriticalBound = loCriticalBound;
	connect(&_loCriticalBound, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	checkCurrentValue();
}

RangeParameter::RangeParameter(const RangeParameter & parameter)
{
	if (this == &parameter)
		return;

	setCurrentValue(parameter.currentValue());
	setPresetValue(parameter.presetValue());
	setLoWarnBound(parameter.loWarnBound());
	setHiWarnBound(parameter.hiWarnBound());
	setLoCriticalBound(parameter.loCriticalBound());
	setHiCriticalBound(parameter.hiCriticalBound());
}


RangeParameter & RangeParameter::operator= (const RangeParameter & parameter)
{
	if (this == &parameter)
		return *this;
	
	setCurrentValue(parameter.currentValue());
	setPresetValue(parameter.presetValue());
	setLoWarnBound(parameter.loWarnBound());
	setHiWarnBound(parameter.hiWarnBound());
	setLoCriticalBound(parameter.loCriticalBound());
	setHiCriticalBound(parameter.hiCriticalBound());

	return *this;
}

RangeParameter::~RangeParameter()
{
	disconnect(&_currentValue, 0, this, 0);
	disconnect(&_presetValue, 0, this, 0);
	disconnect(&_hiWarnBound, 0, this, 0);
	disconnect(&_loWarnBound, 0, this, 0);
	disconnect(&_hiCriticalBound, 0, this, 0);
	disconnect(&_loCriticalBound, 0, this, 0);
}

void RangeParameter::setCurrentValue(const StoredValue & value)
{
	disconnect(&_currentValue, 0, this, 0);
	_currentValue = value;
	connect(&_currentValue, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	checkCurrentValue();
}

void RangeParameter::setPresetValue(const StoredValue & value)
{
	disconnect(&_presetValue, 0, this, 0);
	_presetValue = value;
	connect(&_presetValue, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	checkCurrentValue();
}

void RangeParameter::setHiWarnBound(const StoredValue & value)
{
	disconnect(&_hiWarnBound, 0, this, 0);
	_hiWarnBound = value;
	connect(&_hiWarnBound, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	checkCurrentValue();
}

void RangeParameter::setLoWarnBound(const StoredValue & value)
{
	disconnect(&_loWarnBound, 0, this, 0);
	_loWarnBound = value;
	connect(&_loWarnBound, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	checkCurrentValue();
}

void RangeParameter::setHiCriticalBound(const StoredValue & value)
{
	disconnect(&_hiCriticalBound, 0, this, 0);
	_hiCriticalBound = value;
	connect(&_hiCriticalBound, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	checkCurrentValue();
}

void RangeParameter::setLoCriticalBound(const StoredValue & value)
{
	disconnect(&_loCriticalBound, 0, this, 0);
	_loCriticalBound = value;
	connect(&_loCriticalBound, SIGNAL(valueChanged()), this, SLOT(checkCurrentValue()));

	checkCurrentValue();
}

RangeParameter::State RangeParameter::state() const
{
	return _state;
}

bool RangeParameter::ok() const
{
	return _state == Ok;
}

bool RangeParameter::warning() const
{
	return _state == Warning;
}

bool RangeParameter::critical() const
{
	return _state == Critical;
}

#include <iostream>
using namespace std;

void RangeParameter::checkCurrentValue(bool overrideEmit)
{
	State tmpState;
	
	tmpState = Ok;

	if (_currentValue.rawValue() < _loWarnBound.rawValue())
		tmpState = Warning;

	if (_currentValue.rawValue() > _hiWarnBound.rawValue())
		tmpState = Warning;

	if (_currentValue.rawValue() < _loCriticalBound.rawValue())
		tmpState = Critical;

	if (_currentValue.rawValue() > _hiCriticalBound.rawValue())
		tmpState = Critical;

	if ((_state != tmpState) || overrideEmit)
		switch (tmpState)
		{
		case Ok:
			emit stateChanged(Ok);
			emit valueOk();
			emit valueOk(this);
			break;

		case Warning:
			emit stateChanged(Warning);
			emit valueWarning();
			emit valueWarning(this);
			break;

		case Critical:
			emit stateChanged(Critical);
			emit valueCritical();
			emit valueCritical(this);
			break;
		}

	_state = tmpState;
}
