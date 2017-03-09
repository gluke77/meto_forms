#ifndef _UZG_INCLUDED
#define _UZG_INCLUDED

#include <QObject>

#include "basedevice.h"
#include "baseport.h"
#include "storedvalue.h"
#include "triggervalue.h"

#include <math.h>
/*
class StoredValueWithOffset : public StoredValue
{
Q_OBJECT
	double	_offset;
	
public:

	double offset() const
	{
		return _offset;
	};
	
	StoredValueWithOffset(int rawValue = 0, double coefficient = 1., double offset = 0.,
		int readAddr = 0, int writeAddr = 0,
		bool storable = true, bool loadable = true) :
		
		StoredValue(rawValue, coefficient, readAddr, writeAddr, storable, loadable),
		_offset(offset)
	{
	};
	
	StoredValueWithOffset(const StoredValueWithOffset & svwo) :
		StoredValue(svwo)
	{
		if (this == &svwo)
			return;
			
		_offset = svwo.offset();
	};
	
	StoredValueWithOffset & operator=(const StoredValueWithOffset & svwo)
	{
		if (&svwo == this)
		return *this;

		_rawValue = svwo.rawValue();
		_coefficient = svwo.coefficient();
		_readAddr = svwo.readAddr();
		_writeAddr = svwo.writeAddr();
		_storable = svwo.storable();
		_loadable = svwo.loadable();
		_offset = svwo.offset();

		return *this;
	};
	
	virtual double scaledValue() const
	{
		return _rawValue * _coefficient + _offset;
	};
	
	virtual int rawForScaled(double scaledValue)
	{
		return (int)rint((scaledValue - _offset) / _coefficient);
	};
	
	virtual double scaledForRaw(int rawValue)
	{
		return rawValue * _coefficient + _offset;
	};
};
*/

class Uzg : public BaseDevice
{
Q_OBJECT

	TriggerValue	_runStop;
	TriggerValue	_keepOn;
	TriggerValue 	_busy;
	StoredValue		_freq;
	StoredValue		_pwm;
	StoredValue		_current;
	//StoredValueWithOffset		_temperature;
	StoredValue		_temperature;
	StoredValue		_freqUpper;
	StoredValue		_freqLower;
	StoredValue		_pwmBase;
	StoredValue		_pwmShift;
	StoredValue		_pwmMax;
	StoredValue		_pwmMin;
	StoredValue		_keepFreqStep;
	StoredValue		_keepFreqMaxDelta;
	StoredValue		_tempAlarm;
	StoredValue		_tempStop;
	
	StoredValue		_storeToEE;
	
public:

	TriggerValue & runStop()
	{
		return _runStop;
	};
	
	const TriggerValue & runStop() const
	{
		return _runStop;
	};
	
	TriggerValue & keepOn()
	{
		return _keepOn;
	};
	
	const TriggerValue & keepOn() const
	{
		return _keepOn;
	};
		
	TriggerValue & busy()
	{
		return _busy;
	};
	
	const TriggerValue & busy() const
	{
		return _busy;
	};
	
	StoredValue & freq()
	{
		return _freq;
	};
		
	const StoredValue & freq() const
	{
		return _freq;
	};

	StoredValue & pwm()
	{
		return _pwm;
	};
		
	const StoredValue & pwm() const
	{
		return _pwm;
	};

	StoredValue & current()
	{
		return _current;
	};
		
	const StoredValue & current() const
	{
		return _current;
	};

	//StoredValueWithOffset & temperature()
	StoredValue & temperature()
	{
		return _temperature;
	};
		
	//const StoredValueWithOffset & temperature() const
	const StoredValue & temperature() const
	{
		return _temperature;
	};

	StoredValue & freqUpper()
	{
		return _freqUpper;
	};
		
	const StoredValue & freqUpper() const
	{
		return _freqUpper;
	};

	StoredValue & freqLower()
	{
		return _freqLower;
	};
		
	const StoredValue & freqLower() const
	{
		return _freqLower;
	};

	StoredValue & pwmBase()
	{
		return _pwmBase;
	};
		
	const StoredValue & pwmBase() const
	{
		return _pwmBase;
	};

	StoredValue & pwmShift()
	{
		return _pwmShift;
	};
		
	const StoredValue & pwmShift() const
	{
		return _pwmShift;
	};

	StoredValue & pwmMax()
	{
		return _pwmMax;
	};
		
	const StoredValue & pwmMax() const
	{
		return _pwmMax;
	};

	StoredValue & pwmMin()
	{
		return _pwmMin;
	};
		
	const StoredValue & pwmMin() const
	{
		return _pwmMin;
	};

	StoredValue & keepFreqStep()
	{
		return _keepFreqStep;
	};
		
	const StoredValue & keepFreqStep() const
	{
		return _keepFreqStep;
	};

	StoredValue & keepFreqMaxDelta()
	{
		return _keepFreqMaxDelta;
	};
		
	const StoredValue & keepFreqMaxDelta() const
	{
		return _keepFreqMaxDelta;
	};

	StoredValue & tempAlarm()
	{
		return _tempAlarm;
	};
		
	const StoredValue & tempAlarm() const
	{
		return _tempAlarm;
	};

	StoredValue & tempStop()
	{
		return _tempStop;
	};
		
	const StoredValue & tempStop() const
	{
		return _tempStop;
	};
	
	StoredValue & storeToEE()
	{
		return _storeToEE;
	};
		
	const StoredValue & storeToEE() const
	{
		return _storeToEE;
	};

	Uzg(BasePort * port = 0, int deviceId = 0, const TriggerValue & con = TriggerValue(false, 0, 0, false, false)) :
		BaseDevice(port, deviceId, con)
	{
		_runStop = TriggerValue(false, 0, 0, true, false, 0x0003, 0x0002, 0x0001, 0x0002, 0x0001);
		_keepOn = TriggerValue(false, 0, 0, true, false, 0x0030, 0x0020, 0x0010, 0x0020, 0x0010);
		_busy = TriggerValue(false, 0, 0, 0, 0, 0x0040, 0x0040, 0x0000, 0, 0);
		_freq = StoredValue(0, 1., 0, 1, true, false);
		_pwm = StoredValue(0, 6./255., 0, 2, true, false);	// g_bias_pwm
		//_current = StoredValue(0, 385./10., 0, 0, false, false);
		_current = StoredValue(0, .05, 0, 0, false, false);	//adc_mean_value(ADC_CURRENT)
		//_temperature = StoredValueWithOffset(0, 0.6912, -140., 0, 0, false, false);
		_temperature = StoredValue(0, 0.1, 0, 0, false, false);
		
		_freqUpper = StoredValue(0, 1., 0, 3, true, false);
		_freqLower = StoredValue(0, 1., 0, 4, true, false);
		_pwmBase = StoredValue(0, 6./255., 0, 5, true, false);
		_pwmShift = StoredValue(0, 6./255., 0, 6, true, false);
		_pwmMax = StoredValue(0, 6./255., 0, 7, true, false);
		_pwmMin = StoredValue(0, 6./255., 0, 8, true, false);
		_keepFreqStep = StoredValue(0, 1., 0, 9, true, false);
		_keepFreqMaxDelta = StoredValue(0, 1., 0, 10, true, false);
		_tempStop = StoredValue(0, 1., 0, 11, true, false);
		_tempAlarm = StoredValue(0, 1., 0, 12, true, false);
		
		_storeToEE = StoredValue(0xFFFF, 1., 0, 0xFFFF, true, false);
	};
	
	Uzg(const Uzg & u)
	{
		if (this == &u)
			return;

		_runStop = u.runStop();
		_keepOn = u.keepOn();
		_busy = u.busy();
		_freq = u.freq();
		_pwm = u.pwm();
		_current = u.current();
		_temperature = u.temperature();
		
		_freqUpper = u.freqUpper();
		_freqLower = u.freqLower();
		_pwmBase = u.pwmBase();
		_pwmShift = u.pwmShift();
		_pwmMax = u.pwmMax();
		_pwmMin = u.pwmMin();
		_keepFreqStep = u.keepFreqStep();
		_keepFreqMaxDelta = u.keepFreqMaxDelta();
		_tempStop = u.tempStop();
		_tempAlarm = u.tempAlarm();
		
		_storeToEE = u.storeToEE();

		_deviceId = u.deviceId();
		_port = u.port();
		_connected = u.tvConnected();

	};

	Uzg & operator=(const Uzg & u)
	{
		if (this == &u)
			return *this;

		_runStop = u.runStop();
		_keepOn = u.keepOn();
		_busy = u.busy();
		_freq = u.freq();
		_pwm = u.pwm();
		_current = u.current();
		_temperature = u.temperature();
		
		_freqUpper = u.freqUpper();
		_freqLower = u.freqLower();
		_pwmBase = u.pwmBase();
		_pwmShift = u.pwmShift();
		_pwmMax = u.pwmMax();
		_pwmMin = u.pwmMin();
		_keepFreqStep = u.keepFreqStep();
		_keepFreqMaxDelta = u.keepFreqMaxDelta();
		_tempStop = u.tempStop();
		_tempAlarm = u.tempAlarm();
		
		_storeToEE = u.storeToEE();

		_deviceId = u.deviceId();
		_port = u.port();
		_connected = u.tvConnected();

		return *this;
	};

public slots:
	void storeSettings(void)
	{
		storeValue(_storeToEE, true);
	};
	
	bool poll(int addr = 0, bool overrideEmit = false);
	
	bool run()
	{
		return setRunStop(true);
	};
	
	bool stop()
	{
		return setRunStop(false);
	};
	
	bool setRunStop(bool run = true)
	{
		if (run)
			_runStop.setRawValue(_runStop.writeOnValue(), false);
		else
			_runStop.setRawValue(_runStop.writeOffValue(), false);

		return storeValue(_runStop, true);
	};
	
	bool setKeepOn(bool keepOn = true)
	{
		if (keepOn)
			_keepOn.setRawValue(_keepOn.writeOnValue(), false);
		else
			_keepOn.setRawValue(_keepOn.writeOffValue(), false);

		return storeValue(_keepOn, true);
	};

	bool setFreq(double freq)
	{
		_freq.setScaledValue(freq, false);
		return storeValue(_freq, true);
	};

	bool setFreqUpper(double freqUpper)
	{
		_freqUpper.setScaledValue(freqUpper, false);
		return storeValue(_freqUpper, true);
	};

	bool setFreqLower(double freqLower)
	{
		_freqLower.setScaledValue(freqLower, false);
		return storeValue(_freqLower, true);
	};

	bool setPwm(double pwm)
	{
		_pwm.setScaledValue(pwm, false);
		return storeValue(_pwm, true);
	};
	
	bool setPwmBase(double pwmBase)
	{
		_pwmBase.setScaledValue(pwmBase, false);
		return storeValue(_pwmBase, true);
	};

	bool setPwmShift(double pwmShift)
	{
		_pwmShift.setScaledValue(pwmShift, false);
		return storeValue(_pwmShift, true);
	};

	bool setPwmMax(double pwmMax)
	{
		_pwmMax.setScaledValue(pwmMax, false);
		return storeValue(_pwmMax, true);
	};

	bool setPwmMin(double pwmMin)
	{
		_pwmMin.setScaledValue(pwmMin, false);
		return storeValue(_pwmMin, true);
	};

	bool setKeepFreqStep(double freqStep)
	{
		_keepFreqStep.setScaledValue(freqStep, false);
		return storeValue(_keepFreqStep, true);
	};

	bool setKeepFreqMaxDelta(double freqMaxDelta)
	{
		_keepFreqMaxDelta.setScaledValue(freqMaxDelta, false);
		return storeValue(_keepFreqMaxDelta, true);
	};

	bool setTempAlarm(double tempAlarm)
	{
		_tempAlarm.setScaledValue(tempAlarm, false);
		return storeValue(_tempAlarm, true);
	};

	bool setTempStop(double tempStop)
	{
		_tempStop.setScaledValue(tempStop, false);
		return storeValue(_tempStop, true);
	};

};

#endif // _UZG_INCLUDED
