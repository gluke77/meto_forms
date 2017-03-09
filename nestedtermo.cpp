#include "nestedtermo.h"
#include <windows.h>

#include <iostream>

using namespace std;

#include <QMutexLocker>

bool NestedTermo::poll(bool overrideEmit)
{
//	if (isDisconnected() && _stopIoIfDisconnected)
//		return false;

	if (!_port)
	{
		disconnect(true);
		return false;
	}
	
	if (!_port->connected())
	{
		disconnect(true);
		return false;
	}

	ModbusMsg msg(ModbusMsg::Command,
		ModbusMsg::Read,
		_deviceId,
		_connected.readAddr(),
		4);
	
	ModbusMsg	ack;
	bool		result = false;
	int			error_count = 0;
	QMutexLocker	locker(&(_port->mutex));
	
//	while (error_count < MAX_ERROR_COUNT)
//	{
//		error_count++;

		Sleep(PORT_LOCK_DELAY);
	
		result = _port->writeMsg(msg);
	
		if (!result)
		{
			//disconnect(true);
			//continue;
			return false;
		}
		
		result = _port->readMsg(ack);
			
/*		if (result)
			if ((ModbusMsg::Read == ack.cmdCode()) &&
				(ModbusMsg::Error != ack.msgType()) &&
				(_deviceId == ack.devId()) &&
				(4 == ack.data().size()))
					
				break;

		result = false;
	}
*/	
	if (!result)
	{
	//		disconnect(true);
	}
	else if ((ModbusMsg::Read == ack.cmdCode()) &&
				(ModbusMsg::Error != ack.msgType()) &&
				(_deviceId == ack.devId()) &&
				(4 == ack.data().size()))
	{
		_connected.setRawValue(ack.data()[0], true /*, overrideEmit*/);
		_runStop.setRawValue(ack.data()[1], true, overrideEmit);
		_temperature.presetValue().setRawValue(ack.data()[2], true, overrideEmit);
		_temperature.currentValue().setRawValue(ack.data()[3], true, overrideEmit);
//		_temperature.hiCriticalBound().setRawValue(ack.data()[4], true /*, overrideEmit*/);
//		_temperature.hiWarnBound().setRawValue(ack.data()[5], true /*, overrideEmit*/);
//		_temperature.loWarnBound().setRawValue(ack.data()[6], true /*, overrideEmit*/);
//		_temperature.loCriticalBound().setRawValue(ack.data()[7], true /*, overrideEmit*/);
	
		setMinCritDelta(_minCritDelta);
		setMaxCritDelta(_maxCritDelta);
	
		_temperature.checkCurrentValue(overrideEmit);
	}
	
	return result;
}

