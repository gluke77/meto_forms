#include "uzg.h"

#include <QmutexLocker>

#define MAX_ERROR_COUNT	(3)

bool Uzg::poll(int addr, bool overrideEmit)
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
		addr,
		1);
	
	ModbusMsg ack;
	bool	result;
	
	Sleep(PORT_LOCK_DELAY);

	int error_count = 0;

	QMutexLocker	locker(&(_port->mutex));

	while (error_count < MAX_ERROR_COUNT)
	{
		if (!_port->writeMsg(msg))
			error_count++;
		else if (!_port->readMsg(ack))
			error_count++;
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

	if (0 == addr)
	{
		if (9 != ack.data().size())
			return false;

		_runStop.setRawValue(ack.data()[0], true, overrideEmit);
		_keepOn.setRawValue(ack.data()[0], true, overrideEmit);
		_busy.setRawValue(ack.data()[0], true, overrideEmit);
		_freq.setRawValue(ack.data()[1], true, overrideEmit);
		_pwm.setRawValue(ack.data()[2], true, overrideEmit); // bias_pwm
		// 3 -- power_pwm
		_temperature.setRawValue(ack.data()[4], true, overrideEmit);
		// 5 -- adc_mean_value( ADC_BIASCURRENT)
		int current = ack.data()[6];
		
		if (current < 0)
			current = 0;
			
		if (current > 32000)
			current = 0;
		
		_current.setRawValue(current, true, overrideEmit); // adc_mean_value(ADC_CURRENT)
		// 7 -- adc_mean_value( ADC_AMP)
		// 8 -- stop_mode
		}
	else if (1 == addr)
	{
		if (12 != ack.data().size())
			return false;
	
		_freqUpper.setRawValue(ack.data()[0], true, overrideEmit);
		_freqLower.setRawValue(ack.data()[1], true, overrideEmit);
		_pwmBase.setRawValue(ack.data()[2], true, overrideEmit);
		_pwmShift.setRawValue(ack.data()[3], true, overrideEmit);
		_pwmMax.setRawValue(ack.data()[4], true, overrideEmit);
		_pwmMin.setRawValue(ack.data()[5], true, overrideEmit);
		_keepFreqStep.setRawValue(ack.data()[6], true, overrideEmit);
		_keepFreqMaxDelta.setRawValue(ack.data()[7], true, overrideEmit);
		_tempStop.setRawValue(ack.data()[8], true, overrideEmit);
		_tempAlarm.setRawValue(ack.data()[9], true, overrideEmit);
		// 10 -- power_pwm_base
		// 11 -- power_pwm_shift
	}
	
	return true;
}
