// baseport.h: interface for the BasePort class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _BASEPORT_INCLUDED
#define _BASEPORT_INCLUDED

#include <string>
#include <windows.h>

#include "modbusmsg.h"

#include <QObject>
#include <QMutex>
#include <list>

using namespace std;

class BasePort  
{
public:
	
	bool readMsg(ModbusMsg & msg, ModbusMsg::MsgType msgType = ModbusMsg::Response) const;
	bool writeMsg(const ModbusMsg & msg) const;

	HANDLE handle() const;
	bool connected() const;

	bool initPort();
	bool initPort(const wstring & portName, DWORD baudRate = CBR_9600, BYTE parity = NOPARITY, BYTE stopBits = ONESTOPBIT, BYTE byteSize = 8, DWORD readTimeout = 3000);
	
	BasePort();
	BasePort(const wstring & portName, DWORD baudRate = CBR_9600, BYTE parity = NOPARITY, BYTE stopBits = ONESTOPBIT, BYTE byteSize = 8, DWORD readTimeout = 3000);
	virtual ~BasePort();

	QMutex			mutex;

private:

	HANDLE _hPort;
	
	wstring _portName;
	BYTE _stopBits;
	BYTE _byteSize;
	BYTE _parity;
	DWORD _baudRate;
	DWORD _readTimeout;

	list<ModbusMsg>	msgList;

	bool write(const char * buf, const DWORD bytesToWrite) const;
	bool read(char * buf, DWORD bytesToRead) const;

	
};

#endif // _BASEPORT_INCLUDED

