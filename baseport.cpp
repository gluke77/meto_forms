// baseport.cpp: implementation of the BasePort class.
//
//////////////////////////////////////////////////////////////////////

#include "baseport.h"
#include "modbusmsg.h"
#include <windows.h>
#include <string>
#include <iostream>

#include <QMutex>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BasePort::BasePort(const wstring & portName, DWORD baudRate,
				   BYTE parity, BYTE stopBits, BYTE byteSize, DWORD readTimeout)
				   : mutex(QMutex::Recursive)
{
	_hPort = INVALID_HANDLE_VALUE;
	_baudRate = baudRate;
	_parity = parity;
	_stopBits = stopBits;
	_byteSize = byteSize;
	_portName = portName;
	_readTimeout = readTimeout;
}

BasePort::BasePort() : mutex(QMutex::Recursive)
{
	_hPort = INVALID_HANDLE_VALUE;
	_baudRate = CBR_9600;
	_parity = NOPARITY;
	_stopBits = ONESTOPBIT;
	_byteSize = 8;
	_readTimeout = 3000;
}

BasePort::~BasePort()
{
	if (connected())
	{
		::CloseHandle(_hPort);
		_hPort = INVALID_HANDLE_VALUE;
	}
}

bool BasePort::initPort(const wstring & portName, DWORD baudRate,
						BYTE parity, BYTE stopBits, BYTE byteSize, DWORD readTimeout)
{
	_baudRate = baudRate;
	_parity = parity;
	_stopBits = stopBits;
	_byteSize = byteSize;
	_portName = portName;
	_readTimeout = readTimeout;

	return initPort();
}

bool BasePort::initPort()
{
	if (connected())
	{
		::CloseHandle(_hPort);
		_hPort = INVALID_HANDLE_VALUE;
	}
	
	_hPort = CreateFile(_portName.c_str(),
		GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,NULL,NULL);

	if ( _hPort == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	DCB dcb;

	if (!GetCommState(_hPort,&dcb))
	{
		::CloseHandle(_hPort);
		_hPort = INVALID_HANDLE_VALUE;
		return false;
	}

	dcb.BaudRate = _baudRate;
	dcb.ByteSize = _byteSize;
	dcb.Parity = _parity;
	dcb.StopBits = _stopBits;
	
	if ( !SetCommState(_hPort,&dcb) )
	{
		::CloseHandle(_hPort);
		_hPort = INVALID_HANDLE_VALUE;
		return false;
	}
	
	if ( !SetupComm(_hPort,4096,4096) )
	{
		::CloseHandle(_hPort);
		_hPort = INVALID_HANDLE_VALUE;
		return false;
	}
	
	COMMTIMEOUTS CommTimeouts;

	CommTimeouts.ReadIntervalTimeout = MAXDWORD;
	CommTimeouts.ReadTotalTimeoutConstant = 0; //2;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0; //MAXDWORD;
	CommTimeouts.WriteTotalTimeoutConstant = 500;
	CommTimeouts.WriteTotalTimeoutMultiplier = 50;

	if ( !SetCommTimeouts(_hPort,&CommTimeouts) )
	{
		::CloseHandle(_hPort);
		_hPort = INVALID_HANDLE_VALUE;
		return false;
	}	

	return true;
}


bool BasePort::connected() const
{
	return _hPort != INVALID_HANDLE_VALUE;
}

HANDLE BasePort::handle() const
{
	return _hPort;
}

bool BasePort::read(char * buf, DWORD bytesToRead) const
{
	DWORD	bytesRead;
	if (!connected())
		return false;

	if (0 == ReadFile(_hPort, buf, bytesToRead, &bytesRead, NULL))
		return false;

	return bytesToRead == bytesRead;
}

bool BasePort::write(const char * buf, const DWORD bytesToWrite) const
{
	DWORD	bytesWritten;
	
	if (!connected())
		return false;
		
	if (0 == WriteFile(_hPort, buf, bytesToWrite, &bytesWritten, NULL))
		return false;

	return bytesToWrite == bytesWritten;
}

bool BasePort::writeMsg(const ModbusMsg &msg) const
{
	return write(msg.msg().data(), msg.msg().size());
}

bool BasePort::readMsg(ModbusMsg &msg, ModbusMsg::MsgType msgType) const
{
	DWORD nRead;
	char ch;
	bool msgOk = false;
	
	msg.reset(msgType);

	DWORD start_tics = ::GetTickCount();
	DWORD tics;
	DWORD delay;

	while (!msg.ready())
	{
		if (read(&ch, 1))
		{
			msgOk = msg.addChar(ch);
		}
		else
		{
			Sleep(10);
		
			tics = ::GetTickCount();
			delay = 0;

			if (tics < start_tics)
				delay = 0xFFFFFFFF - start_tics + tics;
			else
				delay = tics - start_tics;

			if (delay > _readTimeout)
				return false;
		}
	}

	return msgOk;
}

void BasePort::postMsg(ModbusMsg msg)
{
	mutex.lock();
	msgList.push_back(msg);
	mutex.unlock();
}

bool BasePort::nextMsg(ModbusMsg & msg)
{
	bool	res = false;
	
	mutex.lock();
	
	if (!msgList.empty())
	{
		msg = msgList.front();
		msgList.pop_front();
		res = true;
	}
	
	mutex.unlock();
	
	return res;
}

bool BasePort::sendNextMsg(bool waitForReply)
{
	ModbusMsg	msg;
	ModbusMsg	ack;
	
	bool		result;
	
	int			error_count = 0;
	
	result = true;
	
	if (!nextMsg(msg))
		return false;
	
	while (error_count < 3)
	{
	
		Sleep(50);
	
		result = writeMsg(msg);
	
		if (!result)
			break;
				
		if (waitForReply)
			result = readMsg(ack);
		else
			break;
			
		if (result)
			if (ModbusMsg::Write == msg.cmdCode())
				if ((ModbusMsg::Write == ack.cmdCode()) &&
					(msg.devId() == ack.devId()) &&
					(msg.addr() == ack.addr()) &&
					(msg.data()[0] == ack.data()[0]))
					
				break;

		error_count++;
	}
	
	return true;
}
