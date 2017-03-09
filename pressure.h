#ifndef _PRESSURE_INCLUDED
#define _PRESSURE_INCLUDED

#include "basedevice.h"
#include "baseport.h"
#include "rangeparameter.h"
#include "triggervalue.h"

class Pressure : public BaseDevice  
{
	Q_OBJECT

protected:
	
	RangeParameter _pressure;

public:
	
	RangeParameter & pressure()
	{
		return _pressure;
	};
	
	const RangeParameter & pressure() const
	{
		return _pressure;
	};

	Pressure(BasePort * port = 0, const int deviceId = 0,
		const TriggerValue & tv = TriggerValue(false, 0, 0, false, false),
		const RangeParameter & rp = RangeParameter())

		: BaseDevice(port, deviceId, tv), _pressure(rp)

	{
	};

	Pressure(const Pressure & t)
	{
		if (this == &t)
			return;

		_pressure = t.pressure();
		_connected = t.tvConnected();
		_port = t.port();
		_deviceId = t.deviceId();
	};

	Pressure & operator=(const Pressure & t)
	{
		if (this == &t)
			return *this;

		_pressure = t.pressure();
		_connected = t.tvConnected();
		_port = t.port();
		_deviceId = t.deviceId();

		return *this;		
	};

public slots:

	void dumbSlot() {};

	virtual bool poll(bool overrideEmit = false);
	virtual bool setPressureLoCriticalBound(double);
	virtual bool setPressureHiCriticalBound(double);
};

#endif // _PRESSURE_INCLUDED
