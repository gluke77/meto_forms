 
#ifndef _MODBUSMSG_INCLUDED
#define _MODBUSMSG_INCLUDED

#include <vector>
#include <string>
#include <ostream>

using namespace std;

class ModbusMsg
{
public:
	enum CmdCode
	{
		Read = 0x03,
		Write = 0x06,
		WriteMulti = 0x10
	};

	enum MsgType
	{
		Command = 0,
		Response,
		Error
	};

	enum ErrCode
	{
		Ok = 0,
		MsgNotReady,
		BadMsgSize,
		BadDataSize,
		BadCrc,
		BadCmdCode,
		ErrResponse
	};

private:
	string	_msg;

	int	_devId;
	CmdCode	_cmdCode;
	int	_addr;
	ErrCode	_error;
	MsgType	_msgType;
	
	vector<int>	_data;

public:
	
	ModbusMsg();
	ModbusMsg(MsgType /* msgType */, CmdCode /* cmdCode */, int /* devId */, int /* addr */, vector<int> & /* data */);
	ModbusMsg(MsgType /* msgType */, CmdCode /* cmdCode */, int /* devId */, int /* addr */, int /* data */);

	void makeCmd(MsgType /* msgType */, CmdCode /* cmdCode */, int /* devId */, int /* addr */, vector<int> & /* data */);
	void makeCmd(MsgType /* msgType */, CmdCode /* cmdCode */, int /* devId */, int /* addr */, int /* data */);
	
	bool	toCmd();
	void	toMsg();
	bool	ready();
	bool	addChar(char /* _ch */);

	int	devId() const;
	int	addr() const;
	vector<int>		data() const;
	string			msg() const;

	void	reset(ModbusMsg::MsgType msgType = ModbusMsg::Command);

	ErrCode	error() const;
	CmdCode	cmdCode() const;
	MsgType	msgType() const;

	static int	crc(vector<int> & /* buf */);
};

ostream & operator << (ostream & os, ModbusMsg & msg);

#endif /* _MODBUSMSG_INCLUDED */
