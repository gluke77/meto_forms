#include "storedvalue.h"
#include <math.h>

StoredValue::StoredValue(
	int rawValue,
	double coefficient,
	int readAddr,
	int writeAddr,
	bool storable,
	bool loadable
	)
{
	_rawValue = rawValue;
	_coefficient = coefficient ? coefficient : 1.;
	_readAddr = readAddr;
	_writeAddr = writeAddr;
	_storable = storable;
	_loadable = loadable;
}

StoredValue::StoredValue(const StoredValue & sv)
{
	if (&sv == this)
		return;

	_rawValue = sv.rawValue();
	_coefficient = sv.coefficient();
	_readAddr = sv.readAddr();
	_writeAddr = sv.writeAddr();
	_storable = sv.storable();
	_loadable = sv.loadable();
}

StoredValue & StoredValue::operator=(const StoredValue & sv)
{
	if (&sv == this)
		return *this;

	_rawValue = sv.rawValue();
	_coefficient = sv.coefficient();
	_readAddr = sv.readAddr();
	_writeAddr = sv.writeAddr();
	_storable = sv.storable();
	_loadable = sv.loadable();

	return *this;
}

double StoredValue::scaledValue() const
{ 
	return _rawValue * _coefficient; 
}

int StoredValue::rawValue() const
{
	return _rawValue;
}

double StoredValue::coefficient() const
{
	return _coefficient;
}

int StoredValue::readAddr() const
{
	return _readAddr;
}

int StoredValue::writeAddr() const
{
	return _writeAddr;
}

void StoredValue::setScaledValue(double scaledValue, bool emitSignals, bool overrideEmit)
{
	setRawValue(rawForScaled(scaledValue), emitSignals, overrideEmit);
}

void StoredValue::setRawValue(int rawValue, bool emitSignals, bool overrideEmit)
{
	if ((_rawValue == rawValue) && !overrideEmit)	// NEW EMIT HERE
		return;

	_rawValue = rawValue;

	if (emitSignals)
	{
		emit valueChanged();
		emit valueChanged(*this);
		emit rawValueChanged(_rawValue);
		emit scaledValueChanged(scaledForRaw(_rawValue));
	}
}
	
void StoredValue::setCoefficient(double coefficient, bool changeRawValue, bool emitSignals)
{
	if (changeRawValue)
	{
		double tmpScaledValue = scaledValue();
		_coefficient = coefficient ? coefficient : 1.;
		_rawValue = rawForScaled(tmpScaledValue);
	}
	else
		_coefficient = coefficient ? coefficient : 1.;

	if (emitSignals)
	{
		emit valueChanged();
		emit valueChanged(*this);
		emit rawValueChanged(_rawValue);
		emit scaledValueChanged(scaledForRaw(_rawValue));
	}
}

bool StoredValue::storable() const
{
	return _storable;
}

bool StoredValue::loadable() const
{
	return _loadable;
}

double StoredValue::scaledForRaw(int rawValue)
{
	return rawValue * _coefficient;
}

int StoredValue::rawForScaled(double scaledValue)
{
	return (int)rint(scaledValue / _coefficient);
}
