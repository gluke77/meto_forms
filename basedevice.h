#ifndef _BASEDEVICE_INCLUDED
#define _BASEDEVICE_INCLUDED

#include <QObject>

#include "storedvalue.h"
#include "triggervalue.h"

#define	PORT_LOCK_DELAY		(150)
#define MAX_ERROR_COUNT		(5)

class BasePort;

class BaseDevice : public QObject
{
	Q_OBJECT

protected:

	BasePort * _port;
	int	_deviceId;
	TriggerValue _connected;
	bool _stopIoIfDisconnected;

public:

	virtual bool loadValue(StoredValue &, bool overrideEmit = false);
	virtual bool storeValue(const StoredValue &, bool waitForReply = true);

	virtual bool isConnected() const
	{
		return _connected.isOn();
	};

	virtual bool isDisconnected() const
	{
		return _connected.isOff();
	};

	const TriggerValue & tvConnected() const
	{
		return _connected;
	};

	TriggerValue & tvConnected()
	{
		return _connected;
	};

	BaseDevice(BasePort * port = 0, int deviceId = 0,
		const TriggerValue & tv = TriggerValue(false, 0, 0, false, false))
		: _port(port), _deviceId(deviceId), _connected(tv)
	{
		_stopIoIfDisconnected = true;
	};

	BasePort * port()
	{
		return _port;
	};

	BasePort * port() const
	{
		return _port;
	};
	
	void setPort(BasePort * port)
	{
		_port = port;
	};

	const int deviceId() const
	{
		return _deviceId;
	};

public slots:

	void dumbSlot() {};
	
	virtual void setConnected(bool on = true, bool emitSignals = true)
	{
//		if (on)
//			_stopIoIfDisconnected = false;
		
		if (on)
			_connected.setRawValue(_connected.writeOnValue(), emitSignals);
		else
			_connected.setRawValue(_connected.writeOffValue(), emitSignals);
	
		storeValue(_connected, true);
	};

	
	virtual void connect(bool emitSignals)
	{
		setConnected(true, emitSignals);
	};
	
	virtual void connect()
	{
		setConnected(true, false);
	};

	virtual void disconnect(bool emitSignals)
	{
		setConnected(false, emitSignals);
	};

	virtual void disconnect()
	{
		setConnected(false, false);
	};

	virtual bool poll()
	{
		return true;
	};

signals:

	void dumbSignal();

	void error();
	void disconnected();
	void disconnected(BaseDevice &);
	void connected();
	void connected(BaseDevice &);
	void connected(bool);
};



#endif /* _BASEDEVICE_INCLUDED */
