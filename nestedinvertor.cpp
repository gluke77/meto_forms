#include "nestedinvertor.h"
#include <windows.h>

#include <QMutexLocker>

bool NestedInvertor::poll(bool overrideEmit)
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
		5);
	
	ModbusMsg	ack;
	bool		result = false;
	int			error_count = 0;
	QMutexLocker	locker(&(_port->mutex));
/*	
	while (error_count < MAX_ERROR_COUNT)
	{
		error_count++;
*/
		Sleep(PORT_LOCK_DELAY);
	
		result = _port->writeMsg(msg);
	
		if (!result)
		{
			//disconnect(true);
			//continue;
			return false;
		}
				
		result = _port->readMsg(ack);
/*			
		if (result)
			if ((ModbusMsg::Read == ack.cmdCode()) &&
				(ModbusMsg::Error != ack.msgType()) &&
				(_deviceId == ack.devId()) &&
				(9 == ack.data().size()))
					
				break;
				
		result = false;
	}
*/	
	if (!result)
	{
	//	disconnect(true);
	}
	else if ((ModbusMsg::Read == ack.cmdCode()) &&
				(ModbusMsg::Error != ack.msgType()) &&
				(_deviceId == ack.devId()) &&
				(5 == ack.data().size()))
	{
		_connected.setRawValue(ack.data()[0], true /*, overrideEmit*/);
		_runStop.setRawValue(ack.data()[1], true, overrideEmit);
		_frequency.setRawValue(ack.data()[2],true, overrideEmit);
		// frequency PV = ack.data()[3]
		_current.currentValue().setRawValue(ack.data()[4], true, overrideEmit);
//		_current.hiCriticalBound().setRawValue(ack.data()[5], true /*, overrideEmit */);
//		_current.hiWarnBound().setRawValue(ack.data()[6], true /*, overrideEmit*/);
//		_current.loWarnBound().setRawValue(ack.data()[7], true /*, overrideEmit*/);
//		_current.loCriticalBound().setRawValue(ack.data()[8], true /*, overrideEmit*/);
	
		_current.checkCurrentValue(overrideEmit);
	}
	
	return result;

}

