#ifndef _INVERTOR_INCLUDED
#define _INVERTOR_INCLUDED

#include "baseport.h"
#include "basedevice.h"
#include "rangeparameter.h"
#include "triggervalue.h"
#include "storedvalue.h"

class Invertor : public BaseDevice  
{
	Q_OBJECT

protected:

	TriggerValue _runStop;
	RangeParameter _current;
	StoredValue _frequency;

public:
	
	TriggerValue & runStop()
	{
		return _runStop;
	};

	RangeParameter & current()
	{
		return _current;
	};

	StoredValue & frequency()
	{
		return _frequency;
	};
	
	const TriggerValue & runStop() const
	{
		return _runStop;
	};

	const RangeParameter & current() const
	{
		return _current;
	};

	const StoredValue & frequency() const
	{
		return _frequency;
	};
	
	Invertor(BasePort * port = 0, const int deviceId = 0,
		const TriggerValue & con = TriggerValue(false, 0, 0, false, false),
		const TriggerValue & rs = TriggerValue(),
		const RangeParameter & rp = RangeParameter(),
		const StoredValue & sv = StoredValue())

		: BaseDevice(port, deviceId,con), _runStop(rs), _current(rp), _frequency(sv)

	{
	};

	Invertor(const Invertor & t)
	{
		if (this == &t)
			return;

		_runStop = t.runStop();
		_current = t.current();
		_frequency = t.frequency();

		_connected = t.tvConnected();
		_port = t.port();
		_deviceId = t.deviceId();
	};

	Invertor & operator=(const Invertor & t)
	{
		if (this == &t)
			return *this;

		_runStop = t.runStop();
		_current = t.current();
		_frequency = t.frequency();

		_connected = t.tvConnected();
		_port = t.port();
		_deviceId = t.deviceId();

		return *this;		
	};

public slots:

	void dumbSlot() {};

	virtual bool setRunStop(bool run = true);

	virtual bool run()
	{
		return setRunStop(true);
	};

	virtual bool stop()
	{
		return setRunStop(false);
	};

	virtual bool poll(bool overrideEmit = false);

	virtual bool setFrequency(double);
	virtual bool setCurrentLoCriticalBound(double);
	virtual bool setCurrentHiCriticalBound(double);

};

class SgInvertor : public Invertor
{
	Q_OBJECT
	
public:
	SgInvertor(BasePort * port = 0, const int deviceId = 0,
		const TriggerValue & con = TriggerValue(false, 0, 0, false, false),
		const TriggerValue & rs = TriggerValue(),
		const RangeParameter & rp = RangeParameter(),
		const StoredValue & sv = StoredValue())

		: Invertor(port, deviceId, con, rs, rp, sv)

	{
	};

public slots:

	virtual bool setFrequency(double);

};

#endif // _INVERTOR_INCLUDED
