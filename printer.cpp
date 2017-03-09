#include "printer.h"

#include <stdio.h>

#include <QmutexLocker>

#define MAX_ERROR_COUNT	(3)

bool Printer::poll(bool overrideEmit)
{
	if (isDisconnected())
		return false;
		
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
		0x0000,
		6);
	
	ModbusMsg ack;
	bool	result;
	
	int error_count = 0;

	QMutexLocker	locker(&(_port->mutex));
	Sleep(PORT_LOCK_DELAY);
	
	
	while (error_count < MAX_ERROR_COUNT)
	{
		if (!_port->writeMsg(msg))
		{
			error_count++;
		}
		else if (!_port->readMsg(ack, ModbusMsg::Response))
		{
			error_count++;
		}
		else
			break;
	}
	
	if (MAX_ERROR_COUNT <= error_count)
	{
		disconnect(true);
		return false;
	}
	
	if (ModbusMsg::Read != ack.cmdCode())
		return false;

	if (_deviceId != ack.devId())
		return false;
		
	if (6 != ack.data().size())
		return false;
		
	_runStop.setRawValue(ack.data()[0], true, overrideEmit);
	_msgNo.setRawValue(ack.data()[1], true, overrideEmit);
	
	int prodCount = (ack.data()[2] & 0x0000FFFF) + (ack.data()[3] & 0x0000FFFF << 16);
	
	_productCount.setRawValue(prodCount, true, overrideEmit);
	
	// 4 - last usart1 error
	// 5 - last nak

	return true;
}
