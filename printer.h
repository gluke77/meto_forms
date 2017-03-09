#ifndef _PRINTER_INCLUDED
#define _PRINTER_INCLUDED

#include <QObject>

#include "basedevice.h"
#include "baseport.h"
#include "storedvalue.h"
#include "triggervalue.h"

class Printer : public BaseDevice
{
Q_OBJECT

	TriggerValue	_runStop;
	StoredValue		_msgNo;
	StoredValue		_productCount;
	StoredValue		_resetCounter;
	
public:

	TriggerValue & runStop()
	{
		return _runStop;
	};
	
	const TriggerValue & runStop() const
	{
		return _runStop;
	};

	StoredValue & msgNo()
	{
		return _msgNo;
	};
		
	const StoredValue & msgNo() const
	{
		return _msgNo;
	};

	StoredValue & productCount()
	{
		return _productCount;
	};
		
	const StoredValue & productCount() const
	{
		return _productCount;
	};
	
	StoredValue & svResetCounter()
	{
		return _resetCounter;
	};
		
	const StoredValue & svResetCounter() const
	{
		return _resetCounter;
	};

	Printer(BasePort * port = 0, int deviceId = 0, const TriggerValue & con = TriggerValue(false, 0, 0, false, false)) :
		BaseDevice(port, deviceId, con)
	{
		_runStop = TriggerValue(false, 0, 0, true, false, 0x0003, 0x0002, 0x0001, 0x0001, 0x0000);
		_msgNo = StoredValue(0, 1., 0, 0x0001, true, false);
		_productCount = StoredValue(0, 1., 0, 0, false, false);
		_resetCounter = StoredValue(0xEFFE, 1., 0, 0x0002, true, false);
	};
	
	Printer(const Printer & p)
	{
		if (this == &p)
			return;

		_runStop = p.runStop();
		_msgNo = p.msgNo();
		_productCount = p.productCount();
		_resetCounter = p.svResetCounter();

		_deviceId = p.deviceId();
		_port = p.port();
		_connected = p.tvConnected();
	};

	Printer & operator=(const Printer & p)
	{
		if (this == &p)
			return *this;

		_runStop = p.runStop();
		_msgNo = p.msgNo();
		_productCount = p.productCount();
		_resetCounter = p.svResetCounter();

		_deviceId = p.deviceId();
		_port = p.port();
		_connected = p.tvConnected();

		return *this;
	};

public slots:
	void resetCounter(void)
	{
		storeValue(_resetCounter, true);
	};
	
	bool poll(bool overrideEmit = false);
	
	bool run()
	{
		return setRunStop(true);
	};
	
	bool stop()
	{
		return setRunStop(false);
	};
	
	bool setRunStop(bool run = true)
	{
		if (run)
			_runStop.setRawValue(_runStop.writeOnValue(), false);
		else
			_runStop.setRawValue(_runStop.writeOffValue(), false);

		return storeValue(_runStop, true);
	};
	
	bool setMsgNo(int msg)
	{
	
		if (1 > msg)
			msg = 1;
			
		if (4 < msg)
			msg = 4;
	
		_msgNo.setRawValue(msg, false);
		return storeValue(_msgNo, true);
	};

};

#endif // _PRINTER_INCLUDED
