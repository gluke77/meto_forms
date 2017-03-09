#ifndef _TRIGGERVALUE_INCLUDED
#define _TRIGGERVALUE_INCLUDED

#include "storedvalue.h"

class TriggerValue : public StoredValue  
{
	Q_OBJECT
	
	bool _on;
		
	int _readMask;
	int _readOnValue;
	int _readOffValue;

	int _writeOnValue;
	int _writeOffValue;

public:
	bool isOn() const
	{
		return _on;
	};

	bool isOff() const
	{
		return !_on;
	};

	int writeOnValue() const
	{
		return _writeOnValue;
	};
	
	int writeOffValue() const
	{
		return _writeOffValue;
	};

	int readMask() const
	{
		return _readMask;
	};

	int readOnValue() const
	{
		return _readOnValue;
	};

	int readOffValue() const
	{
		return _readOffValue;
	};
	
	virtual void setRawValue(int rawValue, bool emitSignals, bool overrideEmit = false);

	TriggerValue(bool on = false,
		int readAddr = 0, int writeAddr = 0,
		bool storable = true, bool loadable = true,
		int rMask = 0, int rOnValue = 0, int rOffValue = 0,
		int wOnValue = 0, int wOffValue = 0)

		: StoredValue(0, 1., readAddr, writeAddr, storable, loadable),
		_on(on), _readMask(rMask), _readOnValue(rOnValue), _readOffValue(rOffValue),
		_writeOnValue(wOnValue), _writeOffValue(wOffValue)
	{
	};

	TriggerValue(const TriggerValue &tv)
		: StoredValue(tv)
	{
		if (this == &tv)
			return;
		
		_on = tv.isOn();

		_readMask = tv.readMask();
		_readOnValue = tv.readOnValue();
		_readOffValue = tv.readOffValue();

		_writeOnValue = tv.writeOnValue();
		_writeOffValue = tv.writeOffValue();
	};

	TriggerValue & operator=(const TriggerValue & tv)
	{
		if (this == &tv)
			return *this;
		
		_on = tv.isOn();
		_rawValue = tv.rawValue();
		_coefficient = tv.coefficient();
		_readAddr = tv.readAddr();
		_writeAddr = tv.writeAddr();
		_storable = tv.storable();
		_loadable = tv.loadable();
		
		_readMask = tv.readMask();
		_readOnValue = tv.readOnValue();
		_readOffValue = tv.readOffValue();

		_writeOnValue = tv.writeOnValue();
		_writeOffValue = tv.writeOffValue();

		return *this;
	};

signals:

	void dumbSignal();

	void trigged(bool);
	void triggedOff();
	void triggedOn();
};

#endif // _TRIGGERVALUE_INCLUDED
