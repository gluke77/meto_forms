#ifndef _TERMO_INCLUDED
#define _TERMO_INCLUDED

#include "baseport.h"
#include "basedevice.h"
#include "rangeparameter.h"
#include "triggervalue.h"

class Termo : public BaseDevice  
{
	Q_OBJECT

protected:

	TriggerValue _runStop;
	RangeParameter _temperature;
	
	double _minWarnDelta;
	double _maxWarnDelta;
	double _minCritDelta;
	double _maxCritDelta;

public:
	
	TriggerValue & runStop()
	{
		return _runStop;
	};

	RangeParameter & temperature()
	{
		return _temperature;
	};
	
	const TriggerValue & runStop() const
	{
		return _runStop;
	};

	const RangeParameter & temperature() const
	{
		return _temperature;
	};

	Termo(BasePort * port = 0, const int deviceId = 0,
		const TriggerValue & con = TriggerValue(false, 0, 0, false, false),
		const TriggerValue & rs = TriggerValue(),
		const RangeParameter & rp = RangeParameter())

		: BaseDevice(port, deviceId, con), _runStop(rs), _temperature(rp)

	{
		_minCritDelta = 0;
		_maxCritDelta = 0;
		_minWarnDelta = 0;
		_maxWarnDelta = 0;
	};

	Termo(const Termo & t)
	{
		if (this == &t)
			return;

		_runStop = t.runStop();
		_temperature = t.temperature();

		_connected = t.tvConnected();
		_port = t.port();
		_deviceId = t.deviceId();
	};

	Termo & operator=(const Termo & t)
	{
		if (this == &t)
			return *this;

		_runStop = t.runStop();
		_temperature = t.temperature();

		_connected = t.tvConnected();
		_port = t.port();
		_deviceId = t.deviceId();

		return *this;		
	};

public slots:

	void dumbSlot() {};

	virtual bool loadValue(StoredValue &, bool overrideEmit = false);

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

	virtual bool setTemperature(double);

	virtual bool setLoCritBound(double);
	virtual bool setHiCritBound(double);
	virtual bool setLoWarnBound(double);
	virtual bool setHiWarnBound(double);
	
	virtual bool setMinCritDelta(double);
	virtual bool setMaxCritDelta(double);
	virtual bool setMinWarnDelta(double);
	virtual bool setMaxWarnDelta(double);

	virtual double minCritDelta()
	{
		return _minCritDelta;
	};

	virtual double maxCritDelta()
	{
		return _maxCritDelta;
	};	

};

#endif // _TERMO_INCLUDED
