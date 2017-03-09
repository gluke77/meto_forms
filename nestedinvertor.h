#ifndef _NESTEDINVERTOR_INCLUDED
#define _NESTEDINVERTOR_INCLUDED

#include "invertor.h"

class NestedInvertor : public Invertor  
{
public:
	NestedInvertor(BasePort * port = 0, const int deviceId = 0,
		const TriggerValue & con = TriggerValue(false, 0, 0, false, false),
		const TriggerValue & rs = TriggerValue(),
		const RangeParameter & rp = RangeParameter(),
		const StoredValue & sv = StoredValue())

		: Invertor(port, deviceId, con, rs, rp, sv)

	{
//		_stopIoIfDisconnected = false;
	};

	NestedInvertor(const NestedInvertor & t)
	{
		if (this == &t)
			return;

		_runStop = t.runStop();
		_current = t.current();
		_frequency = t.frequency();
		
		_connected = t.tvConnected();
		_port = t.port();
		_deviceId = t.deviceId();
		
//		_stopIoIfDisconnected = false;
	};

	NestedInvertor & operator=(const NestedInvertor & t)
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

	virtual bool poll(bool overrideEmit = false);

};

#endif // _NESTEDINVERTOR_INCLUDED
