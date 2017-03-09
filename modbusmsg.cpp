#include "modbusmsg.h"

#include <string>
#include <vector>
#include <ostream>

using namespace std;

static inline int lo_byte(int i)
{
	return i & 0x000000FF;
}

static inline int hi_byte(int i)
{
	return (i & 0x0000FF00) >> 8;
}

static inline int lo_tetr(int i)
{
	return i & 0x0000000F;
}

static inline int hi_tetr(int i)
{
	return (i & 0x000000F0) >> 4;
}

static inline char int2char(int i)
{
	return (0x0A > lo_tetr(i)) ? lo_tetr(i) + '0' : lo_tetr(i) - 0x0A + 'A'; 
}

static inline int char2int(char ch)
{
	if (('0' <= ch) && (ch <= '9'))
		return ch - '0';
	else if (('A' <= ch) && (ch <= 'F'))
		return ch - 'A' + 0x0A;
	else
		return 0;
}

ModbusMsg::ModbusMsg()
{
}

ModbusMsg::ModbusMsg(MsgType msgType, CmdCode cmdCode, int devId, int addr, vector<int> & data)
{
	_devId = devId;
	_cmdCode = cmdCode;
	_addr = addr;
	_error = Ok;
	_msgType = msgType;
	_data = data;

	toMsg();
}

ModbusMsg::ModbusMsg(MsgType msgType, CmdCode cmdCode, int devId, int addr, int data)
{
	_devId = devId;
	_cmdCode = cmdCode;
	_addr = addr;
	_data.clear();
	_data.push_back(data);
	_error = Ok;
	_msgType = msgType;

	toMsg();
}

void ModbusMsg::makeCmd(MsgType msgType, CmdCode cmdCode, int devId, int addr, vector<int> & data)
{
	_devId = devId;
	_cmdCode = cmdCode;
	_addr = addr;
	_error = Ok;
	_msgType = msgType;
	_data = data;

	toMsg();
}

void ModbusMsg::makeCmd(MsgType msgType, CmdCode cmdCode, int devId, int addr, int data)
{
	_devId = devId;
	_cmdCode = cmdCode;
	_addr = addr;
	_data.clear();
	_data.push_back(data);
	_error = Ok;
	_msgType = msgType;

	toMsg();
}

int ModbusMsg::devId() const
{
	return _devId;
}

int ModbusMsg::addr() const
{
	return _addr;
}

ModbusMsg::CmdCode ModbusMsg::cmdCode() const
{
	return _cmdCode;
}

vector<int> ModbusMsg::data() const
{
	return _data;
}

string ModbusMsg::msg() const 
{
	return _msg;
}

ModbusMsg::ErrCode ModbusMsg::error() const
{
	return _error;
}

ModbusMsg::MsgType ModbusMsg::msgType() const
{
	return _msgType;
}

void ModbusMsg::reset(ModbusMsg::MsgType msgType)
{
	_devId = 0;
	_addr = 0;
	_cmdCode = Read;
	_error = Ok;
	_msgType = msgType;
	_msg.erase();
	_data.clear();
}

bool ModbusMsg::addChar(char ch)
{
	if (':' == ch)
		_msg.erase();
	
	_msg += ch;
	
	if (!ready())
		return false;

	return toCmd();
}

bool ModbusMsg::ready()
{
	_error = MsgNotReady;
	
	if (3 > _msg.size())
		return false;
		
	if ((':' == _msg.at(0)) &&
		('\r' == _msg.at(_msg.size() - 2)) &&
		('\n' == _msg.at(_msg.size() - 1)))
		{
			_error = Ok;
			return true;
		}

	return false;
}

int ModbusMsg::crc(vector<int> & buf)
{
	int res = 0;
	
	for (vector<int>::iterator i = buf.begin(); i != buf.end(); i++)
		res += *i;

	res = 0xFF - lo_byte(res);
	res++;

	return res;
}

void ModbusMsg::toMsg()
{
	vector<int>	buf;

	buf.push_back(_devId);

	vector<int>::iterator di;
	vector<int>::iterator di_end;

	switch (_msgType)
	{
	case Command:
		
		buf.push_back(_cmdCode);
	
		switch (_cmdCode)
		{
		case Read:
		case Write:
			buf.push_back(hi_byte(_addr));
			buf.push_back(lo_byte(_addr));
			buf.push_back(hi_byte(_data.front()));
			buf.push_back(lo_byte(_data.front()));
			break;
		case WriteMulti:
			buf.push_back(hi_byte(_addr));
			buf.push_back(lo_byte(_addr));
			
			buf.push_back(hi_byte(_data.size()));
			buf.push_back(lo_byte(_data.size()));
			
			buf.push_back(lo_byte(_data.size()) * 2);
			
			di = _data.begin();
			di_end = _data.end();
			
			while (di != di_end)
			{
				buf.push_back(hi_byte(*di));
				buf.push_back(lo_byte(*di));
				di++;
			}
			break;
			
		}
		break;
		
	case Response:
	
		buf.push_back(_cmdCode);
		
		switch (_cmdCode)
		{
		case WriteMulti:
		case Write:
			buf.push_back(hi_byte(_addr));
			buf.push_back(lo_byte(_addr));
			buf.push_back(hi_byte(_data.front()));
			buf.push_back(lo_byte(_data.front()));
			break;
			
		case Read:
			buf.push_back(lo_byte(_data.size()));
			di = _data.begin();
			di_end = _data.end();
			
			while (di != di_end)
			{
				buf.push_back(hi_byte(*di));
				buf.push_back(lo_byte(*di));
				di++;
			}
			break;
		}
		break;

	case Error:
		
		buf.push_back((int)_cmdCode | 0x80);
		buf.push_back(hi_byte(_data.front()));
		buf.push_back(lo_byte(_data.front()));
		break;
	}
	
	buf.push_back(crc(buf));	
	
	_msg.erase();
	_msg += ':';
	
	for (vector<int>::iterator i = buf.begin(); i != buf.end(); i++)
	{
		_msg += int2char(hi_tetr(*i));
		_msg += int2char(lo_tetr(*i));
	}
	_msg += '\r';
	_msg += '\n';
	
}

bool ModbusMsg::toCmd()
{
	if (!ready())
		return false;

	string tmp_msg(_msg);
	
	tmp_msg.erase(tmp_msg.begin());
	tmp_msg.erase(tmp_msg.end() - 1);
	tmp_msg.erase(tmp_msg.end() - 1);
	
	if (0 != tmp_msg.size() % 2)
	{
		_error = BadMsgSize;
		return false;
	}
	
	_data.clear();

	vector<int> buf;
	int tmp;
	
	string::iterator si = tmp_msg.begin();
	string::iterator si_end = tmp_msg.end();
	
	while (si != si_end)
	{
		tmp = char2int(*si++) << 4;
		tmp += char2int(*si++);
		buf.push_back(tmp);
	}
		
	tmp = buf.back();
	buf.pop_back();

	if (tmp != crc(buf))
	{
		_error = BadCrc;
		return false;
	}
		
	vector<int>::iterator li = buf.begin();
	vector<int>::iterator li_end = buf.end();

	_devId = *li++;

	int tmpCmdCode = *li++;

	if (0x80 & tmpCmdCode)
	{
		tmpCmdCode &= 0x7F;
		_msgType = Error;
	}


	switch (tmpCmdCode)
	{
	case Read:
		
		_cmdCode = Read;
		
		switch (_msgType)
		{
		
		case Command:
		
			if (6 != buf.size())
			{
				_error = BadDataSize;
				return false;
			}

			_addr = *li++ << 8;
			_addr += *li++;
			
			tmp = *li++ << 8;
			tmp += *li++;

			_data.push_back(tmp);

			break;
		
		case Response:
		
			tmp = *li++;
			
			if (3 + tmp != buf.size())
			{
				_error = BadDataSize;
				return false;
			}
			
			while (li != li_end)
			{
				tmp = *li++ << 8;
				tmp += *li++;
				_data.push_back(tmp);
			}
			break;
	
		case Error:
		
	
			if (3 != buf.size())
			{
				_error = BadDataSize;
				return false;
			}

			tmp = *li++ << 8;
			tmp += *li++;

			_data.push_back(tmp);

			break;
		}
		break;

	case Write:
		
		_cmdCode = Write;
		
		switch (_msgType)
		{
		
		case Command:
		case Response:
		
			if (6 != buf.size())
			{
				_error = BadDataSize;
				return false;
			}

			_addr = *li++ << 8;
			_addr += *li++;
			
			tmp = *li++ << 8;
			tmp += *li++;

			_data.push_back(tmp);
			break;
	
		case Error:
		
			if (3 != buf.size())
			{
				_error = BadDataSize;
				return false;
			}

			tmp = *li++ << 8;
			tmp += *li++;

			_data.push_back(tmp);

			break;
		}
		break;
	
	case WriteMulti:
	
		_cmdCode = WriteMulti;
	
		switch (_msgType)
		{
		
		case Command:
			
			if (7 > buf.size())
			{
				_error = BadDataSize;
				return false;
			}
			
			_addr = *li++ << 8;
			_addr += *li++;
			
			tmp = *li++ << 8;
			tmp += *li++;

			if (tmp * 2 != *li++)
			{
				_error = BadDataSize;
				return false;
			}

			if (7 + tmp * 2 != buf.size())
			{
				_error = BadDataSize;
				return false;
			}
			
			while (li != li_end)
			{
				tmp = *li++ << 8;
				tmp += *li++;
				_data.push_back(tmp);
			}
			break;
			
		case Response:
		
			if (6 != buf.size())
			{
				_error = BadDataSize;
				return false;
			}

			_addr = *li++ << 8;
			_addr += *li++;
			
			tmp = *li++ << 8;
			tmp += *li++;

			_data.push_back(tmp);
			break;
		
		case Error:
		
			if (3 != buf.size())
			{
				_error = BadDataSize;
				return false;
			}

			tmp = *li++ << 8;
			tmp += *li++;

			_data.push_back(tmp);

			break;

		}

		break;
	
	default:
		_error = BadCmdCode;
		return false;
	}
		
	return true;
}

template <class T> ostream & operator << (ostream & os, vector<T> v)
{
	os << "( ";

	for (typename vector<T>::iterator i = v.begin(); i != v.end(); i++)
		os << *i << " ";

	os << ")";

	return os;
}

ostream & operator << (ostream & os, ModbusMsg & msg)
{
	os << "msgType = " << msg.msgType()
		<< " cmdCode = " << msg.cmdCode()
		<< " devId = " << msg.devId()
		<< " addr = " << msg.addr()
		<< " data = " << msg.data() << endl
		<< " msg = " << msg.msg();

	return os;
}


