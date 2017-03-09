#include "triggervalue.h"
#include <iostream>
using namespace std;

void TriggerValue::setRawValue(int rawValue, bool emitSignals, bool overrideEmit)
{
	bool tmpOn = _on;
	
	_rawValue = rawValue;

	if ((_rawValue & _readMask) == _readOnValue)
		tmpOn = true;

	if ((_rawValue & _readMask) == _readOffValue)
		tmpOn = false;

	if (_rawValue == _writeOnValue)
		tmpOn = true;

	if (_rawValue == _writeOffValue)
		tmpOn = false;

	if ((_on != tmpOn) || overrideEmit)// NEW EMIT HERE
	{
		_on = tmpOn;
		if (emitSignals)
		{
			emit trigged(_on); 
			if (_on)
				emit triggedOn();
			else
				emit triggedOff();
		}
	}
}
