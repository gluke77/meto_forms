#include "storedvalue.h"
#include "rangeparameter.h"
#include "basedevice.h"
#include "modbusmsg.h"
#include "baseport.h"

#include <windows.h>

#include <QMutexLocker>

using namespace std;

bool BaseDevice::storeValue(const StoredValue & sv, bool waitForReply)
{
	if (!sv.storable())
		return true;
	
	if (isDisconnected() && _stopIoIfDisconnected)
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

	ModbusMsg	msg(ModbusMsg::Command,
		ModbusMsg::Write,
		_deviceId,
		sv.writeAddr(),
		sv.rawValue());
	
	ModbusMsg	ack;
	bool		result = false;
	int			error_count = 0;
	QMutexLocker	locker(&(_port->mutex));
	
	while (error_count < MAX_ERROR_COUNT)
	{
		error_count++;
		
		Sleep(PORT_LOCK_DELAY);
	
		result = _port->writeMsg(msg);
	
		if (!result)
			continue;
				
		if (waitForReply)
			result = _port->readMsg(ack);
		else
			break;
			
		if (result)
			if ((ModbusMsg::Write == ack.cmdCode()) &&
				(ModbusMsg::Error != ack.msgType()) &&
				(msg.devId() == ack.devId()) &&
				(msg.addr() == ack.addr()) &&
				(msg.data()[0] == ack.data()[0]))
					
				break;	
				
		result = false;
	}
	
	//if (!result)
		//disconnect(true);
	
	return result;
}

bool BaseDevice::loadValue(StoredValue & sv, bool overrideEmit)
{
	if (!sv.loadable())
		return true;

	if (isDisconnected() && _stopIoIfDisconnected)
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
		sv.readAddr(),
		1);

	ModbusMsg	ack;
	bool		result = false;
	int			error_count = 0;
	QMutexLocker	locker(&(_port->mutex));
	
	while (error_count < MAX_ERROR_COUNT)
	{
		error_count++;

		Sleep(PORT_LOCK_DELAY);
	
		result = _port->writeMsg(msg);
	
		if (!result)
			continue;
				
		result = _port->readMsg(ack);
			
		if (result)
			if ((ModbusMsg::Read == ack.cmdCode()) &&
				(ModbusMsg::Error != ack.msgType()) &&
				(_deviceId == ack.devId()) &&
				(1 == ack.data().size()))
					
				break;
				
		result = false;
	}
	
	if (!result)
    {
        //disconnect(true);
    }
	else 
		sv.setRawValue(ack.data()[0], true, overrideEmit);
	
	return result;
}

