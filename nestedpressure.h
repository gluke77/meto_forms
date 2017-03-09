#ifndef _NESTEDPRESSURE_INCLUDED
#define _NESTEDPRESSURE_INCLUDED

#include "pressure.h"

class NestedPressure : public Pressure  
{
	Q_OBJECT

public:
	NestedPressure(BasePort * port = 0, const int deviceId = 0,
		const TriggerValue & tv = TriggerValue(false, 0, 0, false, false),
		const RangeParameter & rp = RangeParameter())

		: Pressure(port, deviceId, tv, rp)

	{
//		_stopIoIfDisconnected = false;
	};

	NestedPressure(const NestedPressure & t)
	{
		if (this == &t)
			return;

		_pressure = t.pressure();
		_connected = t.tvConnected();
		_port = t.port();
		_deviceId = t.deviceId();
		
//		_stopIoIfDisconnected = false;
	};

	NestedPressure & operator=(const NestedPressure & t)
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
};

#endif // _NESTEDPRESSURE_INCLUDED
