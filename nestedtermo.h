#ifndef _NESTEDTERMO_INCLUDED
#define _NESTEDTERMO_INCLUDED

#include "termo.h"

class NestedTermo : public Termo  
{
	Q_OBJECT

public:
	NestedTermo(BasePort * port = 0, const int deviceId = 0,
		const TriggerValue & con = TriggerValue(false, 0, 0, false, false),
		const TriggerValue & rs = TriggerValue(),
		const RangeParameter & rp = RangeParameter())

		: Termo(port, deviceId, con, rs, rp)

	{
//		_stopIoIfDisconnected = false;
	};

	NestedTermo(const NestedTermo & t)
	{
		if (this == &t)
			return;

		_runStop = t.runStop();
		_temperature = t.temperature();

		_connected = t.tvConnected();
		_port = t.port();
		_deviceId = t.deviceId();
		
//		_stopIoIfDisconnected = false;
	};

	NestedTermo & operator=(const NestedTermo & t)
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

	virtual bool poll(bool overrideEmit = false);
};

#endif // _NESTEDTERMO_INCLUDED
